#include "xerrori.h"
#include <stdatomic.h>

// versione modificata di contaprimi.c
// nella quale viene mostrato l'utilizzo 
// di una variabile atomic_int che
// per aggiornare in simultanea la somma dei primi 
// contenuta in *(d->somma)


// Il codice ha solo interesse didattico: non c'è motivo
// per incrementare continuamente una variabile condivisa
// i singoli thread potrebbero incrementare una variabile private
// e calcolare la somma complessiva solo alla fine 


//Prototipo test di primalità
bool primo(int n);

// struct usata per passare argomenti ai thread
typedef struct {
  int start;            // intervallo dove cercare i primo 
  int end;              // parametri di input
  atomic_int *somma;    // parametro di output 
} dati;

// funzione passata a pthred_create
void *tbody(void *v) {
  dati *d = (dati *) v;
  fprintf(stderr, "Conto i primi fra %d e %d\n", d->start, d->end);
  // cerco i primi nell'intervallo assegnato
  for(int j=d->start;j<d->end;j++) {
      if(primo(j)) {
        // quando trovo un primo incemento di uno *(d->somma)
        atomic_fetch_add(d->somma,1);  // incrementa in maniera atomica 
        // *(d->somma) += 1;           // anche questo è atomico ma è meno chiaro 
        //*(d->somma) = *(d->somma) + 1;   // attenzione questo non atomico
      }
  }
  fprintf(stderr, "Il thread che partiva da %d ha terminato\n", d->start);
  pthread_exit(NULL);
}

int main(int argc,char *argv[])
{
  if(argc!=3) {
    fprintf(stderr,"Uso\n\t%s m num_threads\n", argv[0]);
    exit(1);
  }
  // conversione input
  int m= atoi(argv[1]);
  if(m<1) termina("limite primi non valido");
  int p= atoi(argv[2]);
  if(p<=0) termina("numero di thread non valido");

  // creazione thread ausiliari
  atomic_int somma = 0; 
  pthread_t t[p];   // array di p indentificatori di thread 
  dati d[p];        // array di p struct che passerò ai p thread
  for(int i=0; i<p; i++) {
    int n = m/p;  // quanti numeri verifica ogni thread + o - 
    d[i].start = n*i; // inizio range thread i
    d[i].end = (i==p-1) ? m : n*(i+1);
    d[i].somma = &somma;
    xpthread_create(&t[i], NULL, &tbody, &d[i],__LINE__, __FILE__); 
  }
  // attendo che i thread abbiano finito
  for(int i=0;i<p;i++) {
    xpthread_join(t[i],NULL,__LINE__, __FILE__);
  }
  // restituisce il risultato 
  printf("Numero primi tra 1 e %d (escluso): %d\n",m,somma);
  return 0;
}



// restituisce true/false a seconda che n sia primo o composto
bool primo(int n)
{
  if(n<2) return false;
  if(n%2==0) {
    if(n==2)  return true;
    else return false; 
  }
  for (int i=3; i*i<=n; i += 2) 
    if(n%i==0) return false;
  return true;
}

