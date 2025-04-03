#include "xerrori.h"
#define QUI __LINE__,__FILE__

// esempio base di gestione segnali in maniera sincrona
// con sigwait e quindi senza handler
// i segnali attesi da sigwait() devo essere bloccati
// perché sigwait() gestisce i segnali pending 

// confrontare con segnali.c che esegue una gestione
// simile ma in maniera asincrona.

// Si noti che entrambi gli esempi assumono che il main()
// non svolga altra attività che quella di attendere i segnali.
// In un programma reale il main() solitamente svolge altre
// attività e quindi la chiamata a sigwait() è fatta in un
// thread dedicato. 

// se si definisce SIGWAITINFO si usa sigwaitinfo()
// per attendere il segnale: questo permette ad esempio di
// sapere chi ha inviato il segnale (pid del processo)
#define SIGWAITINFO 1 


int main(int argc, char *argv[])
{
  // definisco l'insieme dei segnali da gestire con sigwait()
  sigset_t set;
  sigemptyset(&set);
  sigaddset(&set, SIGINT);
  sigaddset(&set, SIGUSR1);
  sigaddset(&set, SIGUSR2);
 
  // blocco questi segnali per questo thread (ma è l'unico...)
  int e=0; 
  e= pthread_sigmask(SIG_BLOCK, &set, NULL);
  if (e != 0) xtermina("Errore pthread_sigmask", QUI);

  // visualizza il pid
  printf("Se vuoi mandarmi dei segnali il mio pid e': %d\n", getpid());
  
  // le variabili che erano globali ora solo locali
  int tot_segnali = 0;
  int continua = 1;

  do {   // loop di attesa dei segnali
    int s; // segnale ricevuto
    #ifndef SIGWAITINFO 
    // sigwait() restituisce il segnale ricevuto in s
    e = sigwait(&set, &s); 
    if (e != 0) xtermina("Errore sigwait", QUI);
    if(s!=SIGUSR1) {
      kill(getpid(),SIGUSR1); // manda SIGUSR1 a se stesso  
    }
    // qui è safe usare la printf, non siamo in un handler
    printf("Segnale %d ricevuto dal processo %d\n", s, getpid());
    #else
    siginfo_t siginfo;
    s = sigwaitinfo(&set, &siginfo);
    if (s <0) xtermina("Errore sigwaitinfo", QUI);
    printf("Segnale %d ricevuto dal processo %d e inviato da %d\n", s, getpid(), siginfo.si_pid);
    #endif

    tot_segnali++;
    if(s!=SIGUSR1) {
      kill(getpid(),SIGUSR1); // manda SIGUSR1 a se stesso  
    }
    if(s==SIGUSR2) {
      // forza uscita dal loop infinito del main()
      continua = 0;
    }
  } while(continua!=0); 
  printf("Ricevuti: %d segnali (uscito dal loop)\n", tot_segnali);
  return 0;
}

