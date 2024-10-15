#define _GNU_SOURCE   // avverte che usiamo le estensioni GNU 
#include <stdio.h>    // permette di usare scanf printf etc ...
#include <stdlib.h>   // conversioni stringa/numero exit() etc ...
#include <stdbool.h>  // gestisce tipo bool (per variabili booleane)
#include <assert.h>   // permette di usare la funzione assert
#include <string.h>   // funzioni di confronto/copia/etc di stringhe
#include <errno.h>    // rischiesto per usare errno


// scopo del programma:
//   mostrare l'uso di asprintf per concatenare stringhe
//   delegando l'allocazione della stringa destinazione


// stampa un messaggio d'errore su stderr e termina il programma
void termina(const char *messaggio);


int main(int argc, char *argv[])
{
  int a;
  char *z=NULL;
  puts("Inserisci un numero e uno stringa"); 
  int e = scanf("%d %ms",&a,&z);
  if(e!=2) termina("Errore scanf");

  // per  i=1..n crea i file nome.i.txt 
  // nell'iesimo file scrivo l'intero i in formato testo 
  for(int i=1;i<=a;i++) {
    char *nome=NULL;
    int e = asprintf(&nome,"%s.%d.txt",z,i);
    if(e== -1) 
      termina("Allocazione in asprintf fallita");
    // crea nome file
    fprintf(stderr, "Ora apro il file: %s\n",nome);
    // apre file
    FILE *f =fopen(nome,"wt");
    // scrivo i dentro il file
    fprintf(f,"%d\n",i);
    // chiudo file e dealloco la stringa nome
    fclose(f);
    free(nome);
  }

  // libero memoria allocata da scanf
  free(z);
  
  return 0;
}


void termina(const char *messaggio)
{
  perror(messaggio);
  exit(1);
}

