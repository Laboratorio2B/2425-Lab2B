/*
 * Esempio paradigma produttore consumatori
 * in cui abbiamo più di un produttore e 
 * più di consumatore
 * 
 * Produttori e consumatori fanno operazioni inutili e veloci 
 * perché lo scopo è misurare l'overhead del paradigma
 * 
 * 
 * */
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <threads.h>
#include <sys/times.h>
#include "semC11.h"

#define Buf_size 10

#ifdef USACV
#warning "Usa Condition Variables"
#define Metodo " ( CVC11) "
#else
#define Metodo " (SemC11) "
#endif




// funzione per trovare il più piccolo divisore>1 
int divisore(int n) {
  assert(n > 1);
  for (int i = 2; i * i <= n; i++) 
    if (n % i == 0) return i;
  return n;
}

// struct contenente i parametri di input 
// per i thread consumatori 
typedef struct {
  int *buffer; 
  int *pcindex;
#ifdef USACV
  int *pdati;
  mtx_t *mutex;
  cnd_t *empty;
  cnd_t *full;
#else  
  mtx_t *pmutex_buf;
  sem11_t *sem_free_slots;
  sem11_t *sem_data_items;
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
  mtx_t *mutex;
  cnd_t *empty;
  cnd_t *full;
#else
  mtx_t *pmutex_buf;
  sem11_t *sem_free_slots;
  sem11_t *sem_data_items;
#endif
  int num_interi;  
} dati_produttori;

// funzione eseguita dai thread consumer
int cbody(void *arg) {  
  dati_consumatori *a = (dati_consumatori *)arg; 
  int n;
  do {
#ifdef USACV
    mtx_lock(a->mutex);
    while (*(a->pdati) == 0) {
      cnd_wait(a->empty, a->mutex);
    }
    *(a->pdati) -= 1;    
#else
    sem11_wait(a->sem_data_items);
    mtx_lock(a->pmutex_buf);
#endif
    // parte comune a CV e sem
    n = a->buffer[*(a->pcindex) % Buf_size];
    *(a->pcindex) +=1;
#ifdef USACV
    cnd_signal(a->full);
    mtx_unlock(a->mutex);
#else
    mtx_unlock(a->pmutex_buf);    
    sem11_post(a->sem_free_slots);
#endif
    if (n == -1) break;
    a->risultato += divisore(n);
  } while (true);
  return 0;
}     


// funzione eseguita dai thread producer
int pbody(void *arg) {  
  dati_produttori *a = (dati_produttori *)arg; 
  for (int i = 2; i <= a->num_interi; i++) {
#ifdef USACV
    mtx_lock(a->mutex);
    while (*(a->pdati) == Buf_size) {
      cnd_wait(a->full, a->mutex);
    }
    *(a->pdati) += 1;
#else
    sem11_wait(a->sem_free_slots);
    mtx_lock(a->pmutex_buf);
#endif
    a->buffer[*(a->ppindex) % Buf_size] = i;
    *(a->ppindex) += 1;
#ifdef USACV
    cnd_signal(a->empty);
    mtx_unlock(a->mutex);
#else
    mtx_unlock(a->pmutex_buf);
    sem11_post(a->sem_data_items);
#endif
  }
  return 0;
}     

int main(int argc, char *argv[]) {
  if (argc != 4) {
    printf("Uso\n\t%s interi #prod #cons\n", argv[0]);
    exit(1);
  }
  int num = atoi(argv[1]);
  int tp = atoi(argv[argc - 2]);
  int tc = atoi(argv[argc - 1]);
  assert(tp > 0);
  assert(tc > 0);
  clock_t start = times(NULL);

  int buffer[Buf_size];
  int pindex = 0, cindex = 0;
#ifdef USACV
  int dati = 0;
  mtx_t mu;
  cnd_t empty, full;
  mtx_init(&mu, mtx_plain);
  cnd_init(&empty);
  cnd_init(&full);
#else
  sem11_t sem_free_slots, sem_data_items;
  sem11_init(&sem_free_slots, Buf_size);
  sem11_init(&sem_data_items, 0);
  mtx_t mup, muc;
  mtx_init(&mup, mtx_plain);
  mtx_init(&muc, mtx_plain);
#endif

  dati_produttori ap[tp];
  dati_consumatori ac[tc];
  thrd_t prod[tp];
  thrd_t cons[tc];

  for (int i = 0; i < tp; i++) {
    ap[i].ppindex = &pindex;
    ap[i].buffer = buffer;
#ifdef USACV
    ap[i].pdati = &dati;
    ap[i].mutex = &mu; 
    ap[i].empty = &empty;
    ap[i].full = &full;
#else
    ap[i].pmutex_buf = &mup;
    ap[i].sem_data_items = &sem_data_items;
    ap[i].sem_free_slots = &sem_free_slots;
#endif
    ap[i].num_interi = num;
    thrd_create(&prod[i], pbody, &ap[i]);     
  }

  for (int i = 0; i < tc; i++) {
    ac[i].pcindex = &cindex;
    ac[i].buffer = buffer;
#ifdef USACV
    ac[i].pdati = &dati;
    ac[i].mutex = &mu;
    ac[i].empty = &empty;
    ac[i].full = &full;
#else
    ac[i].pmutex_buf = &muc;
    ac[i].sem_data_items = &sem_data_items;
    ac[i].sem_free_slots = &sem_free_slots;        
#endif
    ac[i].risultato = 0;
    thrd_create(&cons[i], cbody, &ac[i]);     
  }

  for (int i = 0; i < tp; i++) 
    thrd_join(prod[i], NULL);

  for (int i = 0; i < tc; i++) { 
#ifdef USACV
    mtx_lock(&mu);
    while (dati == Buf_size) {
      cnd_wait(&full, &mu);
    }
    buffer[pindex % Buf_size] = -1;
    pindex += 1;
    dati += 1;
    cnd_signal(&empty);
    mtx_unlock(&mu);
#else
    sem11_wait(&sem_free_slots);
    buffer[pindex % Buf_size] = -1;
    pindex += 1;
    sem11_post(&sem_data_items);
#endif
  }
  // calcola risultato sommando contributo dei thread
  long tot = 0;
  for (int i = 0; i < tc; i++) { 
    thrd_join(cons[i], NULL);
    tot += ac[i].risultato;
  }
  printf(Metodo "Totale: %ld (%ld per produttore)\n", tot, tot / tp);

// distruzione strutture usate per la sincronizzazione  
#ifdef USACV
  mtx_destroy(&mu);
  cnd_destroy(&empty);
  cnd_destroy(&full);
#else
  mtx_destroy(&mup);
  mtx_destroy(&muc);
  sem11_destroy(&sem_free_slots);
  sem11_destroy(&sem_data_items);
#endif
  double tot_time = (double)(times(NULL) - start) / sysconf(_SC_CLK_TCK);
  printf(Metodo "Elapsed time: %.3lf secs (%.3lf musec x intero)\n", tot_time, 1000000 * tot_time / (num * tp));
  return 0;
}