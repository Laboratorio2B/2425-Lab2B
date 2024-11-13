#define _GNU_SOURCE   // avverte che usiamo le estensioni GNU 
#include <stdio.h>    // permette di usare scanf printf etc ...
#include <stdlib.h>   // conversioni stringa/numero exit() etc ...
#include <stdbool.h>  // gestisce tipo bool (per variabili booleane)
#include <assert.h>   // permette di usare la funzione assert
#include <string.h>   // funzioni di confronto/copia/etc di stringhe


// prototipi delle funzioni che appaiono dopo il main()
void stampa_array(int *a, int n);
void termina(char *messaggio);



// funzione che prende in input un intero n e restituisce
// l'array di tutti i primi <=n
// in *num_elementi scrivo quanto elementi ci sono nel'array
// implementata in elenco_primi.s
int *array_primi(int n, int *num_elementi);



// legge un intero N, costruisce array dei primi <=N, e lo stampa
int main(int argc, char *argv[])
{
  int n;
  
  if(argc!=2) {
    printf("Uso: %s nmax\n",argv[0]);
    return 1;
  }
  n = atoi(argv[1]);

  if(n<2)
    termina("L'array che ti interesa è vuoto");
     
  int *a;  // array che conterrà i primi
  assert(sizeof(a)==4); // verifichiamo che i puntatori siano 32 bit 
  int messi;
  a = array_primi(n,&messi);

  stampa_array(a,messi);
    
  // dealloco l'array (da fare solo dopo che ho finito di usarlo!)
  free(a);   
  return 0;
}



// visualizza elementi array di un qualsiasi 
// array di int sul terminale
void stampa_array(int *a, int n)
{
  // stampo il contenuto dell'array
  for(int i=0;i<n;i++)
    printf("%8d",a[i]); // stampo gli elementi in un campo di 8 caratteri
  printf("\nIn totale l'array contiene %d interi\n",n);           
}


// visualizzo messaggio d'errore e termina il programma
void termina(char *messaggio)
{
  puts(messaggio);
  exit(1);
}

#if 0
// modello per la funzione in assembler
int *modello_array_primi(int n, int *num) {
  assert(n>=2);
  int *a = malloc(((n+1)/2)*sizeof(int));
  if(a==NULL) termina("out of mem");
  a[0]=2;       // inserimo 2 tra i primi 
  int messi=1;   
  for(int i=3; i<=n; i+=2) {
    if(primo(i)) {
      // se l'intero i è primo lo inserisco
      a[messi] = i;
      messi += 1;
    }
  }
  // tabella completata
  assert(messi<(n+1)/2);
  // riduco array alla dimensione minima
  a = realloc(a,messi*sizeof(int));
  if(a==NULL) termina("Realloc fallita");
  *num=messi;
  return a;
}
#endif
