#define _GNU_SOURCE   // avverte che usiamo le estensioni GNU 
#include <stdio.h>    // permette di usare scanf printf etc ...
#include <stdlib.h>   // conversioni stringa/numero exit() etc ...
#include <stdbool.h>  // gestisce tipo bool (per variabili booleane)
#include <assert.h>   // permette di usare la funzione assert
#include <string.h>   // funzioni di confronto/copia/etc di stringhe

// scopo del programma: illustrare il duplice scopo delle 
// variabili char e introdurre il concetto di stringa come
// array di caratteri che termina al primo byte uguale a 0


// stampa un messaggio d'errore e termina il programma
void termina(char *messaggio)
{
  puts(messaggio);
  exit(1);
}


int main(int argc, char *argv[])
{
  
  char a,b;

  // inzializza le due variabili char a e b l'intero 70
  // e con il codice ascii del carattere x
  a = 70;
  b = 'x';
  // stampo ogni char interpretando codice ascii (modificatore %c) 
  // e come intero (modificatore %d) 
  printf("a= %c %d, b=%c %d\n",a,a,b,b);
  
  // creo una copia della stringa "the answer is 42"
  char *z = strdup("the answer is 42");
  printf("originale: %s, lunghezza %zd\n",z,strlen(z));
  // sia per la printf() che per la strlen()
  // la stringa termina quando si incontra il byte 0 in z[5] 
  z[5] = 0;
  printf("originale: %s, lunghezza %zd\n",z,strlen(z));
  // la strdup esegue una malloc() quindi devo deallocare con free()
  free(z);
  return 0;
}

