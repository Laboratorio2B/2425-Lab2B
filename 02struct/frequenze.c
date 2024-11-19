#define _GNU_SOURCE   // avverte che usiamo le estensioni GNU 
#include <stdio.h>    // permette di usare scanf printf etc ...
#include <stdlib.h>   // conversioni stringa exit() etc ...
#include <stdbool.h>  // gestisce tipo bool
#include <assert.h>   // permette di usare la funzione ass
#include <string.h>   // funzioni per stringhe
#include <errno.h>    // richiesto per usare errno
#include <math.h>
#include "strint.h"

// definiamo come costannte di compilazione
// la stringa di tutti i delimitatori tra le parole 
#define Delimitatori " .,:;!?<>\"\n-"


// prototipi delle funzioni che appaiono dopo il main()
void termina(const char *messaggio);

strint *crea_albero(FILE *f, int *pdistinte, int *ptot)
{
  assert(f!=NULL);
  strint *root = NULL;
  *pdistinte = *ptot = 0;
  // ciclo di lettura delle linee del file
  char *buffer=NULL;    // usata da getline()
  size_t n=0;           // usata da getline()
  while(true) {
    ssize_t e = getline(&buffer,&n,f); // leggi singola linea
    if(e<0) {        // se e<0 assumiamo sia finito il file
      free(buffer);  // dealloco il buffer usato per le linee 
      break;
    }
    // esegue la tokenizzazione di buffer: legge primo token 
    char *s = strtok(buffer,Delimitatori);
    while(s!=NULL) {
      if(strlen(s)>0) {
        *ptot +=1;
        strint *nodo = abr_ricerca(root,s);  // cerca se la stringa è gia presente
        if(nodo!=NULL)   // se è presente incrementa intero associato
          nodo->n +=1;
        else {
          strint *si = strint_crea(s,1);
          root = abr_inserisci(root,si);
          *pdistinte += 1;
        }
      }
      // legge il prossimo token
      s = strtok(NULL,Delimitatori);
    }
  } // end while del getline
  return root;
}

int riempi_array_nodi(strint *root, strint *a[])
{
  if(root!=NULL) {
    int messisx = riempi_array_nodi(root->left, a);
    a[messisx] = root;
    int messidx = riempi_array_nodi(root->right, 
                                   &a[messisx+1]);
    return 1 + messisx + messidx;
  }
  return 0;
}

int confronta_freq(strint **a, strint **b)
{
  strint *pa = *a;
  strint *pb = *b;
  return pb->n - pa->n;
}


int main(int argc, char *argv[])
{
  if(argc!=2) {
    printf("Uso: %s nomefile\n",argv[0]);
    exit(1);
  } 

  FILE *f = fopen(argv[1],"rt");
  if(f==NULL) termina("Errore apertura input file");
  // crea albero con le stringhe lette dal file
  int totparole, distinte;
  strint *root = crea_albero(f,&distinte,&totparole);
  fclose(f);
  fprintf(stderr,"Distinte: %d, #nodi: %d\n",distinte,abr_strint_totnodi(root));
  fprintf(stderr,"#parole: %d, somma nodi: %d\n",totparole,abr_strint_sommanodi(root));

  // creo array di puntatore ai nodi
  strint **a = malloc(distinte*sizeof(*a));
  if(a==NULL) termina("Non riesco a crere array di nodi");
  int n = riempi_array_nodi(root,a);
  assert(n==distinte);
  qsort(a,n,sizeof(*a), (__compar_fn_t) &confronta_freq);
  puts("---- stampa per frequenza ----");
  for(int i=0;i<n;i++) 
    strint_stampa(a[i],stdout); 
  free(a);   // dealloca a[]: nota l'albero esiste ancora!

  puts("--- inizio visita ---");
  abr_strint_stampa(root,stdout);
  puts("--- fine visita ---");

  abr_strint_distruggi(root);
  return 0;
}



// stampa su stderr il  messaggio che gli passo
// se errno!=0 stampa anche il messaggio d'errore associato 
// a errno. dopo queste stampe termina il programma
void termina(const char *messaggio)
{
  if(errno==0) 
     fprintf(stderr,"%s\n",messaggio);
  else 
    perror(messaggio);
  exit(1);
}

