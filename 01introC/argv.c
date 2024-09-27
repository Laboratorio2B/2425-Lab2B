#define _GNU_SOURCE   // avverte che usiamo le estensioni GNU 
#include <stdio.h>    // permette di usare scanf printf etc ...
#include <stdlib.h>   // conversioni stringa/numero exit() etc ...
#include <stdbool.h>  // gestisce tipo bool (per variabili booleane)
#include <assert.h>   // permette di usare la funzione assert
#include <string.h>   // funzioni di confronto/copia/etc di stringhe

// Nota: per compilare questo programma è necessario il 
// define iniziale di _GNU_SOURCE che deve necessariamente
// precedere tutti gli include


// Scopo del programma:
//  il funzionamento dell'array argv contenente i parametri
//    passati sulla linea di comando
//  mostrare la differenza nel copiare il puntatore e copiare
//  tutti i singoli caratteri della stringa

// stampa un messaggio d'errore e termina il programma
void termina(char *messaggio)
{
  puts(messaggio);
  exit(1);
}


int main(int argc, char *argv[])
{
  
  // stampo gli elementi di argv[] come stringhe
  for(int i=0;i<argc;i++)
    printf("argv[%d]:  %s \n", i, argv[i]);
  puts("---- fine ----");
  
  if(argc<2) 
    termina("Per procedere servono almeno un argomento oltre al nome del programma");
  
  // copio il puntatore
  char *copia = argv[0];
  printf("copia: %s\n",copia);
  printf("orginale: %s\n",argv[0]);
  // strdup copia i singoli caratteri della stringa 
  copia  = strdup(argv[1]);
  copia[0] = 'B';
  printf("copia: %s\n",copia);
  printf("orginale: %s\n",argv[1]);
  // dato che strdup() ottiene lo spazio con malloc() devo deallocare con free()
  free(copia);
 
  // Nota: gli argv[i] non vanno dellocati perchè non sono
  //       stati creati con malloc() 
  return 0;
}

