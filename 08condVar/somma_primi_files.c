/*
 * Esempio paradigma produttore consumatori
 * in cui abbiamo più di un produttore e più di consumatore
 * 
 * Soluzione che utilizza le condition variabiles invece dei semafori
 * Notare che si usano due condition variables associate 
 * allo stesso mutex
 * 
 * I produttori leggono gli interi dai file e li scrivono 
 * nel buffer, i consumatori calcolano il numero di 
 * primi e la loro somma
 * 
 * */
#include "xerrori.h"

#define QUI __LINE__,__FILE__
#define Buf_size 10

// funzione per stabilire se n e' primo  
bool primo(int n)
{
  if(n<2) return false;
  if(n%2==0) return (n==2);
  for (int i=3; i*i<=n; i += 2)
      if(n%i==0) return false;
  return true;
}

// struct contenente i parametri di input 
// per i thread consumatori 
typedef struct {
  int quanti;    // parametro di output
  long somma;    // parametro di output
  int *buffer; 
  int *pcindex;
  int *pdati;
  pthread_mutex_t *mutex;
  pthread_cond_t *empty;
  pthread_cond_t *full;
} dati_consumatori;

// struct contenente i parametri di input 
// per i thread produttori
typedef struct {
  char *nomefile;  
  int *buffer; 
  int *ppindex;
  int *pdati;
  pthread_mutex_t *mutex;
  pthread_cond_t *empty;
  pthread_cond_t *full;
} dati_produttori;



// funzione eseguita dai thread consumer
void *cbody(void *arg)
{  
  dati_consumatori *a = (dati_consumatori *)arg; 

  a->quanti = 0;
  a->somma = 0;
  fprintf(stderr,"[C] consumatore partito\n");
  int n;
  do {
    xpthread_mutex_lock(a->mutex,QUI);
    while(*(a->pdati)==0) {
      // attende fino a quando il buffer è vuoto
      xpthread_cond_wait(a->empty,a->mutex,QUI);
    }
    n = a->buffer[*(a->pcindex) % Buf_size];
    *(a->pcindex) += 1;
    *(a->pdati) -= 1;
    // segnala che il buffer non è più pieno
    xpthread_cond_signal(a->full,QUI);
    xpthread_mutex_unlock(a->mutex,QUI);
    if(n>0 && primo(n)) {
      a->quanti++;
      a->somma += n;    // calcola la solita somma dei primi
    }
  } while(n!=-1);
  pthread_exit(NULL); 
}     

// funzione eseguita dai thread producer
void *pbody(void *arg)
{  
  dati_produttori *a = (dati_produttori *)arg; 

  fprintf(stderr,"[P] produttore con file %s partito\n",a->nomefile);
  // apro il file che ho ricevuto da main
  FILE *f = fopen(a->nomefile,"rt");
  if(f==NULL) pthread_exit(NULL); 
  int e,n;
  do {
    e = fscanf(f,"%d",&n);
    if(e!=1) break;
    xpthread_mutex_lock(a->mutex,QUI);
    while(*(a->pdati)==Buf_size) {
      // attende fino a quando il buffer rimane pieno 
      xpthread_cond_wait(a->full,a->mutex,QUI);
    }
    a->buffer[*(a->ppindex) % Buf_size] = n;
    *(a->ppindex) += 1;
    *(a->pdati) += 1;
    // segnala che il buffer non è più vuoto
    xpthread_cond_signal(a->empty,QUI);
    xpthread_mutex_unlock(a->mutex,QUI);
  } while(true);
  fclose(f);
  pthread_exit(NULL); 
}     


int main(int argc, char *argv[])
{
  // leggi input
  if(argc<2) {
    printf("Uso\n\t%s file1 [file2 file3 ... filek]\n", argv[0]);
    exit(1);
  }
  int tc = 3;  // numero thread consumatori
  assert(tc>0);
  int tp = argc-1;  // numero thread produttori
  assert(tp>0);
  int tot_primi = 0;
  long tot_somma = 0;

  // buffer produttori-consumatori
  int buffer[Buf_size];
  int pindex=0, cindex=0, dati=0;
  pthread_mutex_t mu = PTHREAD_MUTEX_INITIALIZER;
  pthread_cond_t empty = PTHREAD_COND_INITIALIZER;
  pthread_cond_t full = PTHREAD_COND_INITIALIZER;

  // dati per i thread
  dati_produttori ap[tp];
  dati_consumatori ac[tc];
  pthread_t prod[tp];       // id thread produttori
  pthread_t cons[tc];       // id thread consumatori 

  // creo tutti i consumatori
  for(int i=0;i<tc;i++) {
    ac[i].pcindex = &cindex;
    ac[i].pdati = &dati;
    ac[i].buffer = buffer;
    ac[i].mutex = &mu;
    ac[i].empty = &empty;
    ac[i].full = &full;
    xpthread_create(&cons[i], NULL, &cbody, &ac[i],QUI);     
  }
  fprintf(stderr,"Thread consumatori creati");
  // creo tutti i produttori
  for(int i=0;i<tp;i++) {
    ap[i].ppindex = &pindex;
    ap[i].pdati = &dati;
    ap[i].buffer = buffer;
    ap[i].mutex = &mu;
    ap[i].empty = &empty;
    ap[i].full = &full;
    ap[i].nomefile = argv[i+1];
    xpthread_create(&prod[i], NULL, &pbody, &ap[i],QUI);     
  }

  // attendo i produttori
  for(int i=0;i<tp;i++) 
    pthread_join(prod[i],NULL);

  // comunico ai consumatori che possono terminare
  for(int i=0;i<tc;i++) {
    xpthread_mutex_lock(&mu,QUI);
    while(dati==Buf_size) {
      xpthread_cond_wait(&full,&mu,QUI);
    }
    buffer[pindex % Buf_size] = -1;
    pindex += 1;
    dati += 1;
    xpthread_cond_signal(&empty,QUI);
    xpthread_mutex_unlock(&mu,QUI);
  }
  // attendo i consumatori 
  for(int i=0;i<tc;i++) {
    pthread_join(cons[i],NULL);
    tot_primi += ac[i].quanti;
    tot_somma += ac[i].somma;
  }
  // deallocazione, saluti, etc....
  xpthread_mutex_destroy(&mu,QUI);
  xpthread_cond_destroy(&empty,QUI);
  xpthread_cond_destroy(&full,QUI);
  fprintf(stderr,"Trovati %d primi con somma %ld\n",tot_primi,tot_somma);
  return 0;
}
