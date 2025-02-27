/*
 * Esempio semplice paradigma produttore consumatori
 * Il produttore legge interi dai file sulla linea di
 * comando e li nette sul buffer. i consumatori calcolano 
 * il numero e la somma dei primi che hanno visto
 * 
 * E' necessario usare 2 semafori per sincronizzare
 * produttori e consumatori all'inizio e alla fine
 * di ogni file
 * 
 * 
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
  sem_t *sem_contatore;  
  sem_t *sem_contatore2;  
} dati;

// funzione eseguita dai thread consumer
void *tbody(void *arg)
{  
  dati *a = (dati *)arg; 
  a->quanti = 0;
  a->somma = 0;
  int n;
  puts("consumatore partito");
  for(int j=0;j<a->numfiles;j++) {
    // attende che il produttore segnali che si può
    // iniziare con questo file con una post su sem_contatore2 
    xsem_wait(a->sem_contatore2,__LINE__,__FILE__);
    a->quanti = 0;
    a->somma = 0;  
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
    // segnala al produttore con una post 
    // che questo thread ha finito con questo file
    xsem_post(a->sem_contatore,__LINE__,__FILE__);
  } 
  puts("Consumatore sta per finire");
  pthread_exit(NULL); 
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
  sem_t sem_free_slots, sem_data_items, 
        sem_contatore,sem_contatore2;
  xsem_init(&sem_free_slots,0,Buf_size,__LINE__,__FILE__);
  xsem_init(&sem_data_items,0,0,__LINE__,__FILE__);
  xsem_init(&sem_contatore,0,0,__LINE__,__FILE__);
  xsem_init(&sem_contatore2,0,0,__LINE__,__FILE__);
  for(int i=0;i<p;i++) {
    // faccio partire il thread i
    a[i].numfiles = argc-1;
    a[i].buffer = buffer;
    a[i].pcindex = &cindex;
    a[i].pmutex = &mu;
    a[i].sem_data_items = &sem_data_items;
    a[i].sem_free_slots = &sem_free_slots;
    a[i].sem_contatore = &sem_contatore;
    a[i].sem_contatore2 = &sem_contatore2;
    xpthread_create(&t[i],NULL,tbody,a+i,__LINE__,__FILE__);
  }
  puts("Thread ausiliari creati");
  // considero tutti i file sulla linea di comando
  for(int j=1;j<argc;j++) {
    fprintf(stderr,"Lavoro su %s\n", argv[j]);    
    FILE *f = fopen(argv[j],"r");
    if(f==NULL) {perror("Errore apertura file"); return 1;}
    // uso sem_contatore2 per segnalare che i produttori
    // possono inziare su questo file 
    for(int i=0;i<p;i++)
      xsem_post(&sem_contatore2,__LINE__,__FILE__);
    // leggo i dati del file  
    while(true) {
      e = fscanf(f,"%d", &n);
      if(e!=1) break; // se il valore e' letto correttamente e==1
      assert(n>0);    // i valori del file devono essere positivi
      xsem_wait(&sem_free_slots,__LINE__,__FILE__);
      buffer[pindex++ % Buf_size]= n;
      xsem_post(&sem_data_items,__LINE__,__FILE__);
    }
    fprintf(stderr,"Dati del file %s scritti\n",argv[j]);
    // scrivo p copie del valore di terminazione file
    for(int i=0;i<p;i++) {
      xsem_wait(&sem_free_slots,__LINE__,__FILE__);
      buffer[pindex++ % Buf_size]= -1;
      xsem_post(&sem_data_items,__LINE__,__FILE__);
    }
    puts("Valori di terminazione scritti");
    // attendo che tutti i consumatori abbiano terminato
    // con il file corrente
    for(int i=0;i<p;i++)
      xsem_wait(&sem_contatore,__LINE__,__FILE__);
    // calcolo a visualizzo il risutlato per il file  
    tot_primi = tot_somma = 0;  
    for(int i=0;i<p;i++) {
      tot_primi += a[i].quanti;
      tot_somma += a[i].somma;
    }
    fprintf(stderr,"%s: %d primi, somma %ld\n",argv[j],tot_primi,tot_somma);
  }
  // file terminati: join dei thread e uscita
  for(int i=0;i<p;i++) {
    xpthread_join(t[i],NULL,__LINE__,__FILE__);
  }
  pthread_mutex_destroy(&mu);
  return 0;
}

