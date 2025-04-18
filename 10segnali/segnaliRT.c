#include "xerrori.h"
#define QUI __LINE__,__FILE__

// esempio di invio di segnali real-time e di
// invio di una informazione insieme al segnale. 
// l'informazione (un int) è memorizzata in una union 

// Per leggere l'informazione la sigwait è rimpiazzata da sigwaitinfo 
// è possibile leggere l'informazione del segnale anche usando il campo
// sa_sigaction quando si chiama sigaction() per definire un
// handler di gestione dei segnali asincroni


// thread nullafacente
void *tbody(void *v) {
  while(true) {
    sleep(50);
    printf("====== Thread :%d svegliato\n",gettid());
  }
  return NULL;
}


// thread che effettua la gestione di tutti i segnali
// usa sigwaitinfo per leggere l'informazione associata ai segnali
void *tgestore(void *v) {
  sigset_t mask;
  sigfillset(&mask);
  sigdelset(&mask,SIGQUIT); // elimino sigquit dall'insieme
  int s;
  siginfo_t sinfo;
  while(true) {
    int e = sigwaitinfo(&mask,&sinfo);
    if(e<0) perror("Errore sigwaitinfo");
    s = sinfo.si_signo;
    printf("Thread %d ricevuto segnale %d da %d",gettid(),s,sinfo.si_pid);
    printf(" con valore %d\n",sinfo.si_value.sival_int);
    if (s == SIGUSR2) {
      // manda a se stesso un misto di segnali real-time e standard 
      // possibilemente con valore associato al segnale 
      union sigval v;  // union inviata al gestore di segnali

      v.sival_int = 1; // invio l'intero 1 insieme al segnale
      e = pthread_sigqueue(pthread_self(), SIGINT, v);
      if (e != 0) xperror(e, "errore pthread_sigqueue");

      v.sival_int++; // invio l'intero 2
      e = pthread_sigqueue(pthread_self(), SIGRTMAX, v);
      if (e != 0) xperror(e, "errore pthread_sigqueue");

      v.sival_int++; // invio 3, 4, etc etc
      e = pthread_sigqueue(pthread_self(), SIGRTMIN, v);
      if (e != 0) xperror(e, "errore pthread_sigqueue");

      v.sival_int++;
      // nota: raise(SIGINT) equivalente a pthread_kill(pthread_self(),SIGINT);
      e = raise(SIGRTMIN); // questo segnale non ha un valore associato, a me appare 0
      if (e != 0) perror("errore raise"); // raise salva errore in errno

      v.sival_int++; // questo segnale viene perso perché è un secondo SIGINT
      e = pthread_sigqueue(pthread_self(), SIGINT, v);
      if (e != 0) xperror(e, "errore pthread_sigqueue");

      v.sival_int++;
      e = pthread_sigqueue(pthread_self(), SIGRTMAX, v);
      if (e != 0) xperror(e, "errore pthread_sigqueue");

      v.sival_int++;
      // e = sigqueue(getpid(), SIGRTMIN + 1, v); // in questo modo mando il segnale con sigqueue()
      // if (e != 0) perror("errore sigqueue"); // sigqueue salva errore in errno
      e = pthread_sigqueue(pthread_self(), SIGRTMIN+1, v);
      if (e != 0) xperror(e, "errore pthread_sigqueue");

      // è l'ultimo segnale inviato ma verrà gestito prima di tutti i real time 
      v.sival_int++;
      e = pthread_sigqueue(pthread_self(), SIGUSR1, v);
      if (e != 0) xperror(e, "errore pthread_sigqueue");
    }
  }
  return NULL;
}


int main (void) {
  // blocco i segnali tranne SIGQUIT
  sigset_t mask;
  sigfillset(&mask);  // insieme di tutti i segnali
  sigdelset(&mask,SIGQUIT); // elimino sigquit dall'insieme
  pthread_sigmask(SIG_BLOCK,&mask,NULL); // blocco tutto tranne sigquit

  // visualizza il pid
  printf("Se vuoi mandarmi dei segnali il mio pid e': %d\n", getpid());

  // creo nuovi thread che ereditano i settaggi dei segnali
  pthread_t t[3];
  xpthread_create(&t[0],NULL,tbody,NULL,QUI);     // thread nullafacenti
  xpthread_create(&t[1],NULL,tbody,NULL,QUI);
  xpthread_create(&t[2],NULL,tgestore,NULL,QUI);  // thread gestore segnali

  while(true) {
    sleep(40);
    puts("Main svegliato");
  }
  return 0 ;
}

