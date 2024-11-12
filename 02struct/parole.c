#define _GNU_SOURCE   // avverte che usiamo le estensioni GNU 
#include <stdio.h>    // permette di usare scanf printf etc ...
#include <stdlib.h>   // conversioni stringa exit() etc ...
#include <stdbool.h>  // gestisce tipo bool
#include <assert.h>   // permette di usare la funzione ass
#include <string.h>   // funzioni per stringhe
#include <errno.h>    // rischiesto per usare errno
#include <math.h>
#include "strint.h"


// prototipi delle funzioni che appaiono dopo il main()
void termina(const char *messaggio);

int main(int argc, char *argv[])
{
  if(argc<2) {
    printf("Uso: %s [s1 s2 ...]\n",argv[0]);
    exit(1);
  } 

  // inserisce le stringhe passate sulla linea di comando
  // nell'abr con intero consecutivo 
  strint *root = NULL;
  for(int i=1;i<argc;i++) {
    strint *si = strint_crea(argv[i],i-1);
    root = abr_inserisci(root,si);
  }
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

