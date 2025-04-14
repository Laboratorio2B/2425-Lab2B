/*
 * Esempio paradigma produttore consumatori
 * in cui abbiamo più di un produttore e 
 * più di consumatore
 * 
 * Produttori e consumatori fanno operazioni inutili e veloci 
 * perché lo scopo è misurare l'overhead del paradigma
 * 
 * In questa versione se Mutexe!=0 si utilizzano due
 * diversi mutex per le due diverse CV: purtroppo 
 * il programma entra in deadlock se si usa più di prod 
 * o consumatore anche se non ho ben capito dove:
 * studiare i valori scritti su stderr
 * 
 * */
#include "xerrori.h"
#include <sys/times.h>

#define QUI __LINE__,__FILE__
#define Buf_size 10


#ifdef USACV
#warning "Usa Condition Variables"
#define Metodo " ( CV) "
// se Mutexe!=0 allora vengono usati due mutex distinti:
// uno per la CV empty e l'altro per la CV full, ma il programma
// si blocca se ci sono più produttori o più consumatori
#define Mutexe 1
#else
#define Metodo " (Sem) "
#endif

// funzione per trovare il più piccolo divisore>1 
int divisore(int n)
{
  assert(n>1);
  for (int i=2; i*i<=n; i++) 
      if(n%i==0) return i;
  return n;
}



// struct contenente i parametri di input 
// per i thread consumatori 
typedef struct {
  int *buffer; 
  int *pcindex;
#ifdef USACV
  int *pdati;   // puntatore a # tot dati disponibili  
  pthread_mutex_t *mutex;
  pthread_mutex_t *mutexe;
  pthread_cond_t *empty;
  pthread_cond_t *full;
#else  
  pthread_mutex_t *pmutex_buf;
  sem_t *sem_free_slots;
  sem_t *sem_data_items;
#endif
  long risultato;  
} dati_consumatori;

// struct contenente i parametri di input 
// per i thread produttori
typedef struct {
  int *buffer; 
  int *ppindex;
#ifdef USACV
  int *pdati;
  pthread_mutex_t *mutex;
  pthread_mutex_t *mutexe;
  pthread_cond_t *empty;
  pthread_cond_t *full;
#else
  pthread_mutex_t *pmutex_buf;
  sem_t *sem_free_slots;
  sem_t *sem_data_items;
#endif
  int num_interi;  
} dati_produttori;




// funzione eseguita dai thread consumer
void *cbody(void *arg)
{  
  dati_consumatori *a = (dati_consumatori *)arg; 

  int n;
  do {
#ifdef USACV
    xpthread_mutex_lock(a->mutexe,QUI);
    while(*(a->pdati)==0) {
      // attende fino a quando il buffer è vuoto
      fprintf(stderr,"C %d\n",*(a->pdati));
      xpthread_cond_wait(a->empty,a->mutexe,QUI);
    }
    *(a->pdati) -= 1;    
    fprintf(stderr," C %d\n",*(a->pdati));
#else
    xsem_wait(a->sem_data_items,QUI);
    xpthread_mutex_lock(a->pmutex_buf,QUI);
#endif
    // parte comune a CV e semafori: lettura dal buffer
    n = a->buffer[*(a->pcindex) % Buf_size];
    *(a->pcindex) +=1;

#ifdef USACV
    #if Mutexe
    // se mutexe!=mutex allora unlock(mutexe)+lock(mutex)
    xpthread_mutex_unlock(a->mutexe,QUI);
    xpthread_mutex_lock(a->mutex,QUI);
    #endif  
    // segnala che il buffer non è più pieno
    xpthread_cond_signal(a->full,QUI);
    xpthread_mutex_unlock(a->mutex,QUI);
#else    
    xpthread_mutex_unlock(a->pmutex_buf,QUI);
    xsem_post(a->sem_free_slots,QUI);
#endif
    if(n == -1) break;
    a->risultato += divisore(n);
  } while(true);
  pthread_exit(NULL); 
}     

// funzione eseguita dai thread producer
void *pbody(void *arg)
{  
  dati_produttori *a = (dati_produttori *)arg; 

  for(int i=2;i<=a->num_interi;i++) {
#ifdef USACV
    xpthread_mutex_lock(a->mutex,QUI);
    while(*(a->pdati)==Buf_size) {
      // attende fino a quando il buffer rimane pieno 
      fprintf(stderr,"P %d\n",*(a->pdati));
      xpthread_cond_wait(a->full,a->mutex,QUI);
    }
    *(a->pdati) += 1;
    fprintf(stderr," P %d\n",*(a->pdati));
#else
    xsem_wait(a->sem_free_slots,QUI);
    xpthread_mutex_lock(a->pmutex_buf,QUI);
#endif
    a->buffer[*(a->ppindex) % Buf_size] = i;
    *(a->ppindex) +=1;
#ifdef USACV
    // segnala che il buffer non è più vuoto
    #if Mutexe
    // se mutexe!=mutex allora unlock(mutex)+lock(mutexe)
    xpthread_mutex_unlock(a->mutex,QUI);
    xpthread_mutex_lock(a->mutexe,QUI);
    #endif  
    xpthread_cond_signal(a->empty,QUI);
    xpthread_mutex_unlock(a->mutexe,QUI);
#else
    xpthread_mutex_unlock(a->pmutex_buf,QUI);
    xsem_post(a->sem_data_items,QUI);
#endif
  }
  pthread_exit(NULL);
}     


int main(int argc, char *argv[])
{
  // leggi input
  if(argc!=4) {
    printf("Uso\n\t%s interi #prod #cons\n", argv[0]);
    exit(1);
  }
  // numero di thread prod e consumatori
  int num = atoi(argv[1]);
  int tp = atoi(argv[argc-2]);
  int tc = atoi(argv[argc-1]);
  assert(tp>0);
  assert(tc>0);
  clock_t start = times(NULL);


  // buffer produttori-consumatori
  int buffer[Buf_size];
  int pindex=0, cindex=0;
#ifdef USACV
  int dati=0;
  pthread_mutex_t mu = PTHREAD_MUTEX_INITIALIZER;
  pthread_mutex_t mue = PTHREAD_MUTEX_INITIALIZER;
  pthread_cond_t empty = PTHREAD_COND_INITIALIZER;
  pthread_cond_t full = PTHREAD_COND_INITIALIZER;
#else
  pthread_mutex_t mupbuf = PTHREAD_MUTEX_INITIALIZER;
  pthread_mutex_t mucbuf = PTHREAD_MUTEX_INITIALIZER;
  sem_t sem_free_slots, sem_data_items;
  xsem_init(&sem_free_slots,0,Buf_size,QUI);
  xsem_init(&sem_data_items,0,0,QUI);
#endif


  // dati per i thread
  dati_produttori ap[tp];
  dati_consumatori ac[tc];
  pthread_t prod[tp];       // id thread produttori
  pthread_t cons[tc];       // id thread consumatori 

  // creo tutti i produttori
  for(int i=0;i<tp;i++) {
    ap[i].ppindex = &pindex;
    ap[i].buffer = buffer;
#ifdef USACV
    ap[i].pdati = &dati;
    ap[i].mutex = &mu; 
    #if Mutexe   
    ap[i].mutexe = &mue;
    #else
    ap[i].mutexe = &mu;
    #endif        
    ap[i].empty = &empty;
    ap[i].full = &full;
#else
    ap[i].pmutex_buf = &mupbuf;
    ap[i].sem_data_items = &sem_data_items;
    ap[i].sem_free_slots = &sem_free_slots;
#endif
    ap[i].num_interi = num;
    xpthread_create(&prod[i], NULL, &pbody, &ap[i],QUI);     
  }
  // creo tutti i consumatori
  for(int i=0;i<tc;i++) {
    ac[i].pcindex = &cindex;
    ac[i].buffer = buffer;
#ifdef USACV
    ac[i].pdati = &dati;
    ac[i].mutex = &mu;
    #if Mutexe   
    ac[i].mutexe = &mue;
    #else
    ac[i].mutexe = &mu;
    #endif
    ac[i].empty = &empty;
    ac[i].full = &full;
#else
    ac[i].pmutex_buf = &mucbuf;
    ac[i].sem_data_items = &sem_data_items;
    ac[i].sem_free_slots = &sem_free_slots;        
#endif
    ac[i].risultato = 0;
    xpthread_create(&cons[i], NULL, &cbody, &ac[i],QUI);     
  }

  // attendo i produttori
  for(int i=0;i<tp;i++) 
    pthread_join(prod[i],NULL);

  // comunico ai consumatori che possono terminare
  for(int i=0;i<tc;i++) { 
#ifdef USACV
    xpthread_mutex_lock(&mu,QUI);
    while(dati==Buf_size) {
      xpthread_cond_wait(&full,&mu,QUI);
    }
    buffer[pindex % Buf_size] = -1;
    pindex += 1;
    dati += 1;
    xpthread_cond_signal(&empty,QUI);
    xpthread_mutex_unlock(&mu,QUI);
#else
    xsem_wait(&sem_free_slots,QUI);
    buffer[pindex % Buf_size] = -1;
    pindex +=1;
    xsem_post(&sem_data_items,QUI);
#endif
  }

  // calcola risultato sommando contributo dei thread
  long tot=0;
  for(int i=0;i<tc;i++) { 
    pthread_join(cons[i],NULL);
    tot += ac[i].risultato;
  }
  printf(Metodo "Totale: %ld (%ld per produttore)\n",tot,tot/tp);
  // deallocazione, distruzione, etc....
#ifdef USACV
  xpthread_mutex_destroy(&mu,QUI);
  xpthread_mutex_destroy(&mue,QUI);
  xpthread_cond_destroy(&empty,QUI);
  xpthread_cond_destroy(&full,QUI);
#else  
  xpthread_mutex_destroy(&mupbuf, QUI);
  xpthread_mutex_destroy(&mucbuf, QUI);
  xsem_destroy(&sem_free_slots, QUI);
  xsem_destroy(&sem_data_items, QUI);
#endif
  double tot_time = (double) (times(NULL)-start)/sysconf(_SC_CLK_TCK);
  printf(Metodo "Elapsed time: %.3lf secs (%.3lf musec x intero)\n",tot_time,1000000*tot_time/(num*tp));
  return 0;
}

