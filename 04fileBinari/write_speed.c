#define _GNU_SOURCE   // avverte che usiamo le estensioni GNU 
#include <stdio.h>    // permette di usare scanf printf etc ...
#include <stdlib.h>   // conversioni stringa/numero exit() etc ...
#include <stdbool.h>  // gestisce tipo bool (per variabili booleane)
#include <assert.h>   // permette di usare la funzione assert
#include <string.h>   // funzioni di confronto/copia/etc di stringhe
#include <errno.h>    // richiesto per usare errno
// richesti dalla open
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
// richiesta da time
#include <time.h>

// Scopo del programma:
//  mostrare la differenza di prestazioni fra fwrite e write
//  quando vengono eseguite scritture multiple di pochi byte
//  mostrare come si misura il tempo di esecuzione 
//  di una parte di un programma


void termina(const char *messaggio);



int main(int argc, char *argv[])
{
  // verifica siano stati forniti esattamente 2 parametri 
  if(argc!=2) {
    printf("Uso: %s interi_da_scrivere\n",argv[0]);
    return 1;
  }
  // converte il primo parametro in un intero
  int n = atoi(argv[1]);
  if(n<=0) termina("Il parametro n deve essere positivo");

  // scrittura con fwrite 
  time_t start = time(NULL); // numero di secondi da 1/1/1970
  FILE *f = fopen("fwrite.out","wb");
  if(f==NULL) termina("Errore apertura file");
  for(int i=0;i<n;i++)
    if(fwrite(&i,sizeof(i),1,f)!=1)
      termina("Errore scrittura fwrite");
  fclose(f);    
  printf("Tempo fwrite: %ld secs\n",time(NULL)-start);

  // scrittura con write
  start = time(NULL);
  int fd = open("write_out",O_WRONLY|O_CREAT,0666);
  if(fd<0) termina("Apertura file fallita");
  for(int i=0;i<n;i++)
    if(write(fd,&i,sizeof(i))!=sizeof(int))
      termina("Errore scrittura write");
  close(fd);    
  printf("Tempo write: %ld secs\n",time(NULL)-start);
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
