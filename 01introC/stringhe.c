#define _GNU_SOURCE   // avverte che usiamo le estensioni GNU 
#include <stdio.h>    // permette di usare scanf printf etc ...
#include <stdlib.h>   // conversioni stringa/numero exit() etc ...
#include <stdbool.h>  // gestisce tipo bool (per variabili booleane)
#include <assert.h>   // permette di usare la funzione assert
#include <string.h>   // funzioni di confronto/copia/etc di stringhe


// Scopo del programma:
// mostrare alcuni esempi di funzioni che prendono
// come input stringhe (cioè array di char)

// stampa un messaggio d'errore e termina il programma
void termina(char *messaggio);


// restituisce la lunghezza di una stringa
// equivalente alla funzione di libreria strlen()
// la keyword const indica che i caratteri di s[] sono
// read only: il compilatore segnala errore se li cambiamo 
// ad esempio se scriviamo s[0] = 'x'
int str_lunghezza(const char *s)
{
  assert(s!=NULL);
  int i=0;
  while(s[i]!=0) {
    i = i+1;
  }
  assert(s[i]==0);
  return i;
}

// funzione per costruire la concatenazione di due stringhe
// uso apposta i due diversi formati con [] e  * perché 
// sono equivalenti. Attenzione il formato [] può essere usato
// solo nei prototipi, non nel corpo della funzione 
char *str_concatena(char s1[], char *s2)
{
  // richiediamo che s1 e s2 non siano null
  assert(s1!=NULL);
  assert(s2!=NULL);
  
  // calcolo lunghezza stringa concatenata ricordandomi lo 0
  int nuovalun = 1 + str_lunghezza(s1) + str_lunghezza(s2);
  
  // array dinamico di nuovalun caratteri
  char *s3 = malloc(nuovalun*sizeof(char));
  if(s3==NULL) termina("memoria insufficiente");
  
  // array statico con lo stesso numero di caratteri
  // non va usato!!!! il compilatore correttamente
  // segnala che è un errore restituirlo con return
  // char s3[nuovalun];
  
  int i=0; // indice dentro s3
  for(int j=0;s1[j]!=0;j++) { // copio s1 in s3
    s3[i] = s1[j];
    i++;
  }
  for(int j=0;s2[j]!=0;j++) { // copio s2 in s3
    s3[i++] = s2[j];   // ho messo incremento di i dentro assegnamento
  }
  // scrivo terminatore per la stringa s3
  s3[i] = 0; // oppure che è lo stesso s3[i]='\0'

  // verifico le lunghezze: uso strlen() della libreria C
  assert(strlen(s3)==strlen(s1)+strlen(s2));
  // verifico che corrispondano anche i caratteri allocati
  assert(nuovalun == strlen(s3)+1);
  // restituisco la stringa contenente la concatenazione
  // (cioè il puntatore al primo elemento)   
  return s3;
}



int main(int argc, char *argv[])
{
  // verifica siano stati forniti 2 parametri 
  if(argc!=3) {
    printf("Uso: %s stringa1 string2\n",argv[0]);
    return 1;
  }

  // esempi di funzioni per lunghezza e concatenazione di stringhe 
  int l1 = str_lunghezza(argv[1]);
  printf("lunghezza <%s> = %d\n",argv[1],l1);
  
  char *z = str_concatena(argv[1],argv[2]);
  printf("Concatenazione: %s\n",z);
  
  // dealloca concatenazione
  free(z);
 
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


