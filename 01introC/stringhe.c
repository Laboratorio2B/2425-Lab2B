#define _GNU_SOURCE   // avverte che usiamo le estensioni GNU 
#include <stdio.h>    // permette di usare scanf printf etc ...
#include <stdlib.h>   // conversioni stringa/numero exit() etc ...
#include <stdbool.h>  // gestisce tipo bool (per variabili booleane)
#include <assert.h>   // permette di usare la funzione assert
#include <string.h>   // funzioni di confronto/copia/etc di stringhe


// Scopo del programma:
// mostrare alcuni esempi di funzioni che prendono
// come input stringhe (cioè array char)

// stampa un messaggio d'errore e termina il programma
void termina(char *messaggio);


int main(int argc, char *argv[])
{
  // verifica siano stati forniti almeno 2 parametri 
  if(argc<3) {
    printf("Uso: %s stringa1 string2 [altre stringhe]\n",argv[0]);
    return 1;
  }

  // esempi di funzioni per lunghezza, copia, concatenazione di stringhe 


  // dealloca se necessario
 
  // Nota: gli argv[i] non vanno dellocati perchè non sono
  //       stati creati con malloc() 
  return 0;
}


// stampa un messaggio d'errore e termina il programma
void termina(char *messaggio)
{
  perror(messaggio);
  exit(1);
}


