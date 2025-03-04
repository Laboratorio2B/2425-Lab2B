#define _GNU_SOURCE   // avverte che usiamo le estensioni GNU 
#include <stdio.h>    // permette di usare scanf printf etc ...
#include <stdlib.h>   // conversioni stringa/numero exit() etc ...
#include <stdbool.h>  // gestisce tipo bool
#include <assert.h>   // permette di usare la funzione assert
#include <string.h>   // funzioni per stringhe
#include "xerrori.h"



// prototipi delle funzioni che appaiono dopo il main()
void merge(int a[], int na, int c[], int nc, int b[]);
int *random_array(int n, int seed);
int intcmp(const void *a, const void *b);

// struct contenente i parametri di input e output di ogni thread 
typedef struct {
  int *a;  // indirizzo array
  int m;   // dimensione array 
} dati;


// funzione eseguita dal thread ausiliario
void *tbody(void *arg)
{  
  dati *d = (dati *)arg; 
  qsort(d->a, d->m, sizeof(int), &intcmp);
  pthread_exit(NULL);
} 


// ordina gli interi passati sulla linea di comando
int main(int argc, char *argv[])
{
  if(argc!=3) {
    fprintf(stderr, "Uso:\n\t   %s num_interi seed\n", argv[0]);
    exit(1);
  }
  int n = atoi(argv[1]);
  assert(n>0);
  // genera array random di n elementi
  int *a = random_array(n,atoi(argv[2]));
  // calcola somma prima dell'ordinamento
  long somma=0;
  for(int i=0;i<n;i++) somma += a[i];
  
  // esegue l'ordinamento
  // creiamo i due array di lunghezza n/2 circa 
  if(n>1) {
    // se n==1 l'array è ordinato
    int n1 = n/2;
    int n2 = n-n1;
    int *a1 = malloc(n1*sizeof(int));  
    int *a2 = malloc(n2*sizeof(int));
    assert(a1!=NULL && a2!=NULL);
    // copio gli elementi a[] -> a1[] e a[2]
    for(int i=0;i<n1;i++) 
      a1[i] = a[i];   // prima metà 
    for(int i=0;i<n2;i++) 
      a2[i] = a[i+n1];   // seconda metà 
    // preparo il lancio per il thread ausiliario     
    dati d;
    d.a = a2; d.m = n2;
    pthread_t t;
    // eseguo il lancio dell'ordinamento della seconda metà
    xpthread_create(&t,NULL,tbody,&d,__LINE__,__FILE__);
    // in parallelo ordino la prima metà
    qsort(a1,n1,sizeof(int),&intcmp);
    xpthread_join(t,NULL,__LINE__,__FILE__);
    // merge delle due metà
    merge(a1, n1, a2, n2, a);
    free(a1); free(a2);
  }
  // controlla ordinamento
  long somma2 = a[0];
  for(int i=1;i<n;i++) {
    somma2 += a[i];
    if(a[i-1]>a[i]) {
      printf("Array non ordinato alla posizione %d (%d>%d)\n",i,a[i-1],a[i]);
      exit(1);
    }
  }
  if(somma!=somma2) {
    printf("Somma degli elementi non corrisponde: %ld vs %ld\n",somma,somma2);
    exit(1);
  }
  else 
    puts("Array ordinato e somma mantenuta, sorting probabilmente OK");

  // dealloco l'array e termino
  free(a);   
  return 0;
}




// funzione per il merge di due array in un terzo array già allocato
// merge di a[0...n1-1] e c[0... n2-1] dentro b[]
// Soluzione proposta da co-pilot apparentemente corretta
void merge(int a[], int na, int c[], int nc, int b[])
{
  assert(a!=NULL);
  assert(c!=NULL);
  assert(b!=NULL);
  assert(na>0);
  assert(nc>0);
  
  int i=0; // indice per a[]
  int j=0; // indice per c[]
  int k=0; // indice per b[]
  
  // scorro a[] e c[] e copio il minore in b[]
  while(i<na && j<nc) {
    if(a[i]<c[j]) {
      b[k] = a[i];
      i++;
    } else {
      b[k] = c[j];
      j++;
    }
    k++;
  }
  
  // copio il resto di a[] in b[]
  while(i<na) {
    b[k] = a[i];
    i++;
    k++;
  }
  
  // copio il resto di c[] in b[]
  while(j<nc) {
    b[k] = c[j];
    j++;
    k++;
  }
  // asserzioni aggiunte da GM perché non si sa mai
  assert(i==na);
  assert(j==nc);
  assert(k==na+nc); 
}

// genera array di n elementi con interi random tra 0 e 999999 
int *random_array(int n, int seed)
{
  assert(n>0);
  srandom(seed);
  int *a = malloc(n* sizeof(int));
  assert(a!=NULL);
  for(int i=0;i<n;i++)
    a[i] = (int) random() % 1000000;
  return a;
}

// funzione di confronto tra interi passata a qsort
int intcmp(const void *a, const void *b)
{
  return *((int *) a) - *((int *) b);
}


