/*
 * Esempio paradigma 1 produttore - p consumatori
 * Il produttore legge interi dai file sulla linea di
 * comando e li mette sul buffer. i consumatori calcolano 
 * il numero e la somma dei primi che hanno visto
 * 
 * Il produttore quando tutti i consumatori hanno
 * finito su di un certo file calcola e stampa
 * il numero totale e la somma totale dei primi 
 * nel file.
 * 
 * Vengono usati 2 semafori per sincronizzare
 * produttori e consumatori all'inizio e alla fine
 * di ogni file.
 * 
 * Se viene definita la variabile USE_BARRIER 
 * la sincronizzazione viene effettuata usando
 * una barriera; vedere le pagine man di: 
 *    pthread_barrier_init
 *    pthread_barrier_wait
 * 
 * Vedere nel makefile come vengono generati gli eseguibili
 * per entrambe le versioni (semafori o barriera)
 * 
 * */
#include "xerrori.h"

// dimensione buffer prod/consumatore
#define Buf_size 10

// costante usata per compilazione condizionale
// #define USE_BARRIER 1

#ifdef USE_BARRIER
#warning "Sincronizzazione con barriere"
#else 
#warning "Sincronizzazione con semafori"
#endif

// funzione per stabilire se n e' primo  
bool primo(int n)
{
  if(n<2) return false;
  if(n%2==0) return (n==2);
  for (int i=3; i*i<=n; i += 2)
      if(n%i==0) return false;
  return true;
}

// struct contenente i parametri di input e output di ogni thread 
typedef struct {
  int numfiles; // input 
  int quanti;   // output
  long somma;   // output
  int *buffer; 
  int *pcindex;
  pthread_mutex_t *pmutex;
  sem_t *sem_free_slots;
  sem_t *sem_data_items;  
  #ifdef USE_BARRIER
  pthread_barrier_t *fine_file;  // compilato solo se USE_BARRIER definito 
  #else 
  sem_t *sem_contatore;         // compilato se USE_BARRIER non definito
  sem_t *sem_contatore2;
  #endif  
} dati;

// funzione eseguita dai thread consumer
void *tbody(void *arg)
{  
  dati *a = (dati *)arg; 
  fprintf(stderr,"[C] consumatore partito\n");
  for(int j=0;j<a->numfiles;j++) {
    #ifndef USE_BARRIER
    // attende che il produttore segnali che si può
    // iniziare con questo file con una post su sem_contatore2 
    xsem_wait(a->sem_contatore2,__LINE__,__FILE__);
    #endif
    int n, quanti = 0;
    long somma = 0;  
    do {
      xsem_wait(a->sem_data_items,__LINE__,__FILE__);
      xpthread_mutex_lock(a->pmutex,__LINE__,__FILE__);
      n = a->buffer[*(a->pcindex) % Buf_size];
      *(a->pcindex) +=1;
      xpthread_mutex_unlock(a->pmutex,__LINE__,__FILE__);
      xsem_post(a->sem_free_slots,__LINE__,__FILE__);
      if(n>0 && primo(n)) {
        quanti++;
        somma += n;
      }
    } while(n!= -1);
    fprintf(stderr,"[C] segnale fine file ricevuto\n");
    // copia i valori dove il produttore li puo leggere
    a->quanti = quanti; a->somma=somma;
    #ifdef USE_BARRIER
    int e = pthread_barrier_wait(a->fine_file);
    fprintf(stderr,"[C] barrier_wait() restituisce %d\n",e);
    #else
    // segnala al produttore con una post 
    // che questo thread ha finito con questo file
    xsem_post(a->sem_contatore,__LINE__,__FILE__);
    #endif
  } 
  return NULL;  // equivalente a pthread_exit(NULL); 
}     


int main(int argc, char *argv[])
{
  // leggi input
  if(argc<2) {
    printf("Uso\n\t%s file1 [file2 file3 ....]\n", argv[0]);
    exit(1);
  }
  int p = 3;  // numero thread ausiliari (consumatori)
  assert(p>=0);
  int tot_primi = 0;
  long tot_somma = 0;
  int e,n,cindex=0;    
  // inizializzazione dei thread e dei loro argomenti
  int buffer[Buf_size];
  int pindex=0;
  pthread_mutex_t mu = PTHREAD_MUTEX_INITIALIZER;
  pthread_t t[p];
  dati a[p];
  sem_t sem_free_slots, sem_data_items; 
  xsem_init(&sem_free_slots,0,Buf_size,__LINE__,__FILE__);
  xsem_init(&sem_data_items,0,0,__LINE__,__FILE__);
  #ifdef USE_BARRIER
  pthread_barrier_t bar_fine_file;
  pthread_barrier_init(&bar_fine_file, NULL, p+1);
  #else
  sem_t sem_contatore,sem_contatore2;
  xsem_init(&sem_contatore,0,0,__LINE__,__FILE__);
  xsem_init(&sem_contatore2,0,0,__LINE__,__FILE__);
  #endif
  for(int i=0;i<p;i++) {
    // faccio partire il thread i
    a[i].numfiles = argc-1;
    a[i].buffer = buffer;
    a[i].pcindex = &cindex;
    a[i].pmutex = &mu;
    a[i].sem_data_items = &sem_data_items;
    a[i].sem_free_slots = &sem_free_slots;
    #ifdef USE_BARRIER
    a[i].fine_file = &bar_fine_file;
    #else
    a[i].sem_contatore = &sem_contatore;
    a[i].sem_contatore2 = &sem_contatore2;
    #endif
    xpthread_create(&t[i],NULL,tbody,&a[i],__LINE__,__FILE__);
  }
  fprintf(stderr, "[P] %d thread ausiliari creati\n",p);
  // considero tutti i file sulla linea di comando
  for(int j=1;j<argc;j++) {
    fprintf(stderr,"[P] Lavoro su %s\n", argv[j]);    
    FILE *f = xfopen(argv[j],"r",__LINE__,__FILE__);
    // uso sem_contatore2 per segnalare che i produttori
    // possono inziare su questo file
    #ifndef USE_BARRIER 
    for(int i=0;i<p;i++)
      xsem_post(&sem_contatore2,__LINE__,__FILE__);
    #endif
    // leggo i dati del file  
    while(true) {
      e = fscanf(f,"%d", &n);
      if(e!=1) break; // se il valore e' letto correttamente e==1
      assert(n>0);    // i valori del file devono essere positivi
      xsem_wait(&sem_free_slots,__LINE__,__FILE__);
      buffer[pindex++ % Buf_size]= n;
      xsem_post(&sem_data_items,__LINE__,__FILE__);
    }
    fclose(f);
    fprintf(stderr,"[P] Dati del file %s scritti\n",argv[j]);
    // scrivo p copie del valore di terminazione file
    for(int i=0;i<p;i++) {
      xsem_wait(&sem_free_slots,__LINE__,__FILE__);
      buffer[pindex++ % Buf_size]= -1;
      xsem_post(&sem_data_items,__LINE__,__FILE__);
    }
    // attendo che tutti i consumatori abbiano terminato
    // con il file corrente
    #ifdef USE_BARRIER
    e = pthread_barrier_wait(&bar_fine_file);
    fprintf(stderr,"[P] barrier_wait() restituisce: %d\n",e);
    #else
    for(int i=0;i<p;i++)
      xsem_wait(&sem_contatore,__LINE__,__FILE__);
    #endif
    // calcolo a visualizzo il risultato per il file argv[j]  
    tot_primi = tot_somma = 0;  
    for(int i=0;i<p;i++) {
      tot_primi += a[i].quanti;
      tot_somma += a[i].somma;
    }
    printf("File: %s: %d primi, somma %ld\n",argv[j],tot_primi,tot_somma);
  }
  // file terminati: join dei thread e uscita
  for(int i=0;i<p;i++) {
    xpthread_join(t[i],NULL,__LINE__,__FILE__);
  }
  pthread_mutex_destroy(&mu);
  return 0;
}

