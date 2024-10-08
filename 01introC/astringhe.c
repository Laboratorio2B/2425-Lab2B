#define _GNU_SOURCE   // avverte che usiamo le estensioni GNU 
#include <stdio.h>    // permette di usare scanf printf etc ...
#include <stdlib.h>   // conversioni stringa/numero exit() etc ...
#include <stdbool.h>  // gestisce tipo bool (per variabili booleane)
#include <assert.h>   // permette di usare la funzione assert
#include <string.h>   // funzioni di confronto/copia/etc di stringhe

// Scopo del programma:
//   mostrare la costruzinoe e deallocazione di 
//   un array di stringhe


// stampa un messaggio d'errore e termina il programma
void termina(char *messaggio)
{
  perror(messaggio);
  exit(1);
}

// esegue una copia della stringa s
// e ne ribalta i carattero 
char *copia_ribalta(const char *s)
{
  // crea copia
  char *t = strdup(s);
  // ribalta caratteri
  for(int i=0;i<strlen(s);i++) 
    t[i] = s[strlen(s)-i-1];
  // retiruisce la nuova stringa  
  return t;  
}


int main(int argc, char **argv)// oppure char *argv[]
{    
  if(argc<2) 
    termina("Per procedere servono almeno un argomento oltre al nome del programma");

  // creo array di stringhe ribaltate
  int n=argc-1;  // lunghezza array
  char **a = malloc(n*sizeof(char *)); 
  if(a==NULL) termina("Memoria esaurita");
  // copio e ribalto le stringeh su argv
  for(int i=0;i<n;i++)
    a[i] = copia_ribalta(argv[i+1]);
    
  // stampa contenuto di a
  for(int i=0;i<n;i++)
    puts(a[i]);
    
  // dealloco completamente a[]
  for(int i=0;i<n;i++)
    free(a[i]);  // dealloco le singole stringhe
  free(a);       // dealloco l'array a[]

  return 0;
}

