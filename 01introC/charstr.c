#define _GNU_SOURCE   // avverte che usiamo le estensioni GNU 
#include <stdio.h>    // permette di usare scanf printf etc ...
#include <stdlib.h>   // conversioni stringa/numero exit() etc ...
#include <stdbool.h>  // gestisce tipo bool (per variabili booleane)
#include <assert.h>   // permette di usare la funzione assert
#include <string.h>   // funzioni di confronto/copia/etc di stringhe

// le istruzioni qui sopra leggono i prototipi di alcune funzioni di libreria



// stampa un messaggio d'errore e termina il programma
void termina(char *messaggio)
{
  puts(messaggio);
  exit(1);
}


int main(int argc, char *argv[])
{
  
  char a,b;
  
  a = 70;
  b = 'x';
  printf("a= %c %d, b=%c %d\n",a,a,b,b);

  return 0;
}

