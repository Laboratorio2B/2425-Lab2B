// Nuovo esempio di sorting parallelo 
// si procede partizionando l'array a metà e ordinando 
// le due metà in parallelo in maniera ricorsiva.
// la ricorsione termina quando la dimensione
// dell'array è inferiore ad una certa soglia passata da linea di comando
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
void pmergesort(int *a, int m, int soglia);


// struct contenente la descrizione di un array
// e una soglia per la ricorsione
typedef struct {
  int *a;     // indirizzo array
  int m;      // dimensione array 
  int soglia; 
} array;



// funzione eseguita dai thread ausiliari
// ordina con qsort oppure usa divide array 
// e oridna in parallelo con un thread ausiliario 
void *tbody(void *arg)
{  
  array *d = (array *)arg;
  // se l'array ha meno elementi di soglia
  // ordino con qsort
  if (d->m <= d->soglia)
    qsort(d->a,d->m,sizeof(int),&intcmp);
  else {
    // divido il problema in due
    assert(d->m>1);
    int n1 = d->m/2;
    int n2 = d->m - n1;
    // creo thread con seconda metà array da ordinare
    fprintf(stderr,"Creo thread che esegue sorting su %d elementi\n",n2);
    array g;
    g.a = &d->a[n1];
    g.m = n2;
    g.soglia = d->soglia;
    pthread_t t;
    xpthread_create(&t,NULL,&tbody,&g,__LINE__,__FILE__);
    // sort prima metà
    pmergesort(d->a,n1,d->soglia);
    xpthread_join(t,NULL,__LINE__,__FILE__);
    // merge con array ausiliario
    int *tmp = malloc(d->m*sizeof(int));
    assert(tmp!=NULL);
    merge(d->a,n1,g.a,g.m,tmp);
    // copia da array ausiliario e deallocalo 
    for(int i=0;i<d->m;i++)
      d->a[i] = tmp[i];
    free(tmp);
  }
  pthread_exit(NULL);
} 

// entry point dell'algoritmo di mergesort parallelo
// gli array di lunghezza <= soglia sono ordinati con qsort()
// gli altri sono divisi a metà e ordinati in parallelo
// usando un thread ausiliario
void pmergesort(int *a, int m, int soglia)
{  
  // se l'array ha meno elementi di soglia
  // ordino con qsort
  if (m <= soglia)
    qsort(a,m,sizeof(int),&intcmp);
  else {
    // creo thread con seconda metà array da ordinare
    assert(m>1);
    int n1 = m/2;
    int n2 = m - n1;
    fprintf(stderr,"Creo thread che esegue sorting su %d elementi\n",n2);
    array g;
    g.a = &a[n1];
    g.m = n2;
    g.soglia = soglia;
    pthread_t t;
    xpthread_create(&t,NULL,&tbody,&g,__LINE__,__FILE__);
    // sort prima metà con chiamata ricorsiva
    pmergesort(a,n1,soglia);
    xpthread_join(t,NULL,__LINE__,__FILE__);
    // merge con array ausiliario
    int *tmp = malloc(m*sizeof(int));
    assert(tmp!=NULL);
    merge(a,n1,g.a,g.m,tmp);
    for(int i=0;i<m;i++)
      a[i] = tmp[i];
    free(tmp);
  }
} 

 
// ordina un array di argv[1] interi random generati con 
// seed argv[2] termimando la ricorsione quando la dimensione
// degli array raggiunge argv[3]
int main(int argc, char *argv[])
{
  if(argc!=4) {
    fprintf(stderr, "Uso:\n\t   %s num_interi seed soglia\n", argv[0]);
    exit(1);
  }
  int n = atoi(argv[1]);
  assert(n>0);
  int soglia = atoi(argv[3]);
  assert(soglia>1);
  // genera array random di n elementi
  int *a = random_array(n,atoi(argv[2]));
  // calcola somma prima dell'ordinamento
  long somma=0;
  for(int i=0;i<n;i++) somma += a[i];
  
  // esegue l'ordinamento
  pmergesort(a,n,soglia);
  
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
void merge(int a[], int na, int c[], int nc, int b[])
{
  assert(a!=NULL && c!=NULL && b!=NULL);
  assert(na>0 && nc>0);
  
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
    i++; k++;
  }
  
  // copio il resto di c[] in b[]
  while(j<nc) {
    b[k] = c[j];
    j++; k++;
  }
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


