// Algoritmo Mergesort che sfrutta la notazione &a[n1] per ottenere 
// il puntatore all'inizio dell'array contenente al seconda metà
// dell'input.

#define _GNU_SOURCE   // avverte che usiamo le estensioni GNU 
#include <stdio.h>    // permette di usare scanf printf etc ...
#include <stdlib.h>   // conversioni stringa/numero exit() etc ...
#include <stdbool.h>  // gestisce tipo bool
#include <assert.h>   // permette di usare la funzione assert
#include <string.h>   // funzioni per stringhe

// prototipi delle funzioni che appaiono dopo il main()
void stampa_array(int *a, int n, FILE *f);
void termina(char *messaggio);


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

// funzione mergesort ricorsiva
void mergesort(int *a, int n)
{
  assert(a!=NULL);
  assert(n>0);
  
  // caso base
  if(n==1) return;
  
  int n1 = n/2;       // dimensione prima parte
  int n2 = n - n1 ;   // dimensione seconda parte
  assert(n1+n2==n);   // verifico che sto considerando tutti gli elementi
  
  // ordino a[0...n1-1]
  mergesort(a,n1);
  // ordino a[n1...n-1]
  mergesort(&a[n1],n2); // &a[n1] potevo scriverlo a+n1
  
  // ora ho le due metà ordinate devo fare il merge
  int *b = malloc(n*sizeof(*b));
  if(b==NULL) termina("malloc fallita nel merge");
  merge(a,n1,&a[n1],n2,b);  
  // copio il risultato da b[] ad a[]
  for(int i=0;i<n;i++)
    a[i] = b[i];
  
  free(b); // dealloco array temporaneao b[]
}


// ordina gli interi passati sulla linea di comando
int main(int argc, char *argv[])
{

  if(argc<2) {
    fprintf(stderr, "Uso:\n\t   %s i1 [i2 i3 i4 ... ]\n", argv[0]);
    fprintf(stderr, "dove i valori  i1 i2 ...  sono interi\n");
    exit(1);
  }
  int n = argc - 1;
  int *a = malloc(n * sizeof(int));
  if (a == NULL) {
    termina("Non è possibile allocare lo spazio necessario per l'input.");
  }
  // conversione argomenti da riga di comando in interi da inserire in a[]
  for (int i = 0; i < n; i++)
    a[i] = atoi(argv[i+1]);
    
  // -------------------------
  // Esempio di visualizzazione di un puntatore 
  int *q = &n; // variabile puntatore p contenente l'indirizzo di n
  // in printf i puntatori si indicano con %p  
  printf("Puntatore %p, valore %d\n", q, *q); // %p visualizza il puntatore in esadecimale


  // --- altre considerazioni sui puntatori ---------
  // la variabile a è un puntatore a a[0]
  // la variabile q è un puntatore a n
  
  // per vedere il valore a cui punta q scrivo *q
  // per vedere il valore a cui punta a scrivo a[0]
  //   ma posso scrivere anche *a che quindi è ==a[0]
  // per leggere *q posso anche scrivere q[0]
  // conclusione: 
  //   è equivalente scrivere *a e a[0]
  //   qualsiasi sia la variabile a
  // Altra osservazione: l'indirizzo di a[0] è a 
  //   quindi è equivalente scrivere &a[0] e a
  //   e in generale è equivalente scrivere &a[i] e a+i
  // La scrittura a+i è un esempio di "aritmentica dei puntatori"
  //   in cui sommiamo un puntatore a un intero
  // Il programma mergesort mostra l'uso di &a[i]
  //   per creare un nuovo array che inizia dalla
  //   posizione i-esima dell'array a[]   
  // -----------------------------
    
  // ordina array a[] in place (cioè spostando gli elementi) 
  mergesort(a,n);
  
  // stampo array e chiudo il file
  stampa_array(a,n,stdout);
  
  // dealloco l'array e termino
  free(a);   
  return 0;
}


// stampa un messaggio d'errore su stderr e termina il programma
void termina(char *messaggio)
{
  perror(messaggio);
  exit(1);
}


// scrive elementi di un qualsiasi 
// array di int sul file f
void stampa_array(int *a, int n, FILE *f)
{
  assert(a!=NULL);
  // stampo il contenuto dell'array
  for(int i=0;i<n;i++)
    fprintf(f,"%8d",a[i]); // stampo gli elementi in un campo di 8 caratteri
  fprintf(f,"\nIn totale l'array contiene %d interi\n",n);
}
