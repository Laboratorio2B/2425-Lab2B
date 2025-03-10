

## Esercizi



### Sort parallelo (27/2/25)

Il sorgente qui sotto utilizza `qsort` per ordinare un array di interi. Modificarlo in maniera da velocizzare l'ordinamento utilizzando *un* thread ausiliario. L'ordinamento deve essere fatto secondo il seguente schema:

1. l'array da ordinare viene logicamente diviso in due: la prima metà viene ordinata dal thread ausiliario, la seconda metà dal theread principale. Gli ordinamenti delle due metà possono essere fatte con `qsort`

2. combinare le due metà ordinate per ottenre il risultato finale utilizzando la funzione `merge()` che trovate nel sorgente. 


Completata questa prima parte, scrivere una nuova versione che utilizza *tre* thread ausiliari (quindi quattro in totale). Una coppia di thread deve effettuare il sorting della prima metà utilizzando la strategia vista sopra, una seconda coppia ordina la seconda metà e alla fine il thread principale effettua il merge finale.

E' possibile risolvere l'esercizio utilizzando solo le funzione `pthread_create` e `pthread_join`, ma è comunque ammesso l'uso di mutex e semafori. 

```C
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
  qsort(a,n,sizeof(int),&intcmp);
  
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
  assert(a!=NULL && c!=NULL & b!=NULL);
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
```



## Sort parallelo 2 (6/3/25)

Nel programma `pmergesort.c` (vedere la descrizione all'interno del sorgente), le due funzioni `pmergesort()` e `tbody()` svolgono esenzialmente le stesse operazioni: ordinare l'array con `qsort()` se la dimensione è minore di soglia, oppure partizionare e ordinare in parallelo le due metà.   Dato che duplicare il codice non è mai una buona idea, modificare il programma in maniera da mantenere solamente una delle due funzioni (eventualmente con piccole modifiche) e cancellare completamente l'altra.



## Partitioning parallelo (6/3/25)  

Il programma `quicksort.c` contiene una implementazione standard dell'algoritmo `quicksort` per ordinare un array di interi. Come ricorderete la parte più complessa dell'algoritmo è la procedura `di partizionamento` che viene realizzata utilizzando con due indici, uno crescente e l'altro decrescente. 

Modificare il programma per ottenere una versione nella quale il partizionamento viene fatto con l'ausilio di un thread ausiliario che provvede a gestire l'indice decrescente, mentre il thread principale provvede a gestire l'indice crescente. Scegliete il metodo di sincronizzazione dei thread che ritenete necessario, l'importante è che per array di grandi dimensioni il vostro programma risulti piu veloce di quello sequenziale. 

Per semplicità utilizzate il thread ausiliario solamente quando la dimensione dell'array da partizionare è minore di una certa soglia, ad esempio 100; array sotto la soglia partizionateli con la procedura standard. 
