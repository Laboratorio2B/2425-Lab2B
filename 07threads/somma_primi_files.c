/*
 * Esempio paradigma produttori consumatori
 * con produttori e consumatori multipli
 * 
 * I produttori leggono interi dai file e i consumatori
 * cacolano il numero e la somma dei primi
 * 
 * il numero di produttori è uguale al numero di file di input
 * il numero di consumatori è stabilito all'inizo del main() 
 *  
 * Usare il numeri.py per generare lunghi elenchi di interi positivi su cui testare il programma
 *  * 
 * */
#include "xerrori.h"

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

// struct contenente i parametri dei thread consumatori
typedef struct {
  int quanti;    // parametro di output
  long somma;    // parametro di output
  int *buffer;   // variabili per schema prod/consumatori
  int *pcindex;  // puntatore a cindex 
  pthread_mutex_t *pmutex;
  sem_t *sem_free_slots;
  sem_t *sem_data_items;  
} datic;


// struct contenente i parametri dei thread produttori
typedef struct {
  char *nomefile;  // nome file da gestire 
  int *buffer;     // variabili per schema prod/consumatori
  int *ppindex;    // puntatore a pindex
  pthread_mutex_t *pmutex;
  sem_t *sem_free_slots;
  sem_t *sem_data_items;  
} datip;



// funzione eseguita dai thread consumatori
void *cbody(void *arg)
{  
  datic *a = (datic *)arg; 
  a->quanti = 0;
  a->somma = 0;
  fprintf(stderr,"[C] consumatore partito\n");
  int n;
  do {
    xsem_wait(a->sem_data_items,__LINE__,__FILE__);
    xpthread_mutex_lock(a->pmutex,__LINE__,__FILE__);
    n = a->buffer[*(a->pcindex) % Buf_size];
    *(a->pcindex) +=1;
    xpthread_mutex_unlock(a->pmutex,__LINE__,__FILE__);
    xsem_post(a->sem_free_slots,__LINE__,__FILE__);
    if(n>0 && primo(n)) {
      a->quanti++;
      a->somma += n;
    }
  } while(n!= -1);
  pthread_exit(NULL); 
}     


// funzione eseguita dai thread produttori
void *pbody(void *arg)
{  
  datip *a = (datip *)arg; 
  fprintf(stderr,"[P] produttore con file %s partito\n",a->nomefile);
  // apro il file che ho ricevuto da main
  FILE *f = fopen(a->nomefile,"rt");
  if(f==NULL) pthread_exit(NULL); 
  int e,n;
  while(true) {
    e = fscanf(f,"%d", &n);
    if(e!=1) break; // se il valore e' letto correttamente e==1
    assert(n>0);    // i valori del file devono essere positivi
    xsem_wait(a->sem_free_slots,__LINE__,__FILE__);
    xpthread_mutex_lock(a->pmutex,__LINE__,__FILE__);
    a->buffer[(*a->ppindex)++ % Buf_size]= n;
    xpthread_mutex_unlock(a->pmutex,__LINE__,__FILE__);
    xsem_post(a->sem_data_items,__LINE__,__FILE__);
  }
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
  int cons = 3;  // numero thread consumatori
  assert(cons>0);
  int prod = argc-1;  // numero thread produttori
  assert(prod>0);
  
  int tot_primi = 0;
  long tot_somma = 0;
  // threads related
  int buffer[Buf_size];
  int cindex=0, pindex=0;
  // mutex x consumatori
  pthread_mutex_t cmu = PTHREAD_MUTEX_INITIALIZER;
  // mutex x produttori
  pthread_mutex_t pmu = PTHREAD_MUTEX_INITIALIZER;
  // semafori x produttori e consumatori 
  sem_t sem_free_slots, sem_data_items;
  xsem_init(&sem_free_slots,0,Buf_size,__LINE__,__FILE__);
  xsem_init(&sem_data_items,0,0,__LINE__,__FILE__);
  pthread_t tc[cons];
  datic ac[cons];
  for(int i=0;i<cons;i++) {
    // faccio partire il thread i
    ac[i].buffer = buffer;
    ac[i].pcindex = &cindex;
    ac[i].pmutex = &cmu;
    ac[i].sem_data_items = &sem_data_items;
    ac[i].sem_free_slots = &sem_free_slots;
    xpthread_create(&tc[i],NULL,cbody,&ac[i],__LINE__,__FILE__);
  }
  fprintf(stderr,"Thread consumatori creati");
  pthread_t tp[prod];
  datip ap[prod];
  for(int i=0;i<prod;i++) {
    // faccio partire il thread produttore i
    ap[i].buffer = buffer;
    ap[i].ppindex = &pindex;
    ap[i].pmutex = &pmu;
    ap[i].sem_data_items = &sem_data_items;
    ap[i].sem_free_slots = &sem_free_slots;
    ap[i].nomefile = argv[i+1];
    xpthread_create(&tp[i],NULL,pbody,&ap[i],__LINE__,__FILE__);
  }
  fprintf(stderr,"Thread produttori creati");
  // join dei thread produttori
  for(int i=0;i<prod;i++) {
    xpthread_join(tp[i],NULL,__LINE__,__FILE__);
  }
  // terminazione thread consumatori
  for(int i=0;i<cons;i++) {
    xsem_wait(&sem_free_slots,__LINE__,__FILE__);
    buffer[pindex++ % Buf_size]= -1;
    xsem_post(&sem_data_items,__LINE__,__FILE__);
  }
  puts("Valori di terminazione scritti");
  // join dei thread consumatori e calcolo risulato
  for(int i=0;i<cons;i++) {
    xpthread_join(tc[i],NULL,__LINE__,__FILE__);
    tot_primi += ac[i].quanti;
    tot_somma += ac[i].somma;
  }
  xpthread_mutex_destroy(&pmu,__LINE__,__FILE__);
  xpthread_mutex_destroy(&cmu,__LINE__,__FILE__);
  xsem_destroy(&sem_data_items,__LINE__,__FILE__);
  xsem_destroy(&sem_free_slots,__LINE__,__FILE__);
  fprintf(stderr,"Trovati %d primi con somma %ld\n",tot_primi,tot_somma);
  return 0;
}

