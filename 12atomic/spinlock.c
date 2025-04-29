#define _GNU_SOURCE   // permette di usare estensioni GNU
#include <stdio.h>
#include <stdlib.h>
#include <stdatomic.h>
#include <pthread.h>
#include <unistd.h>
#include <assert.h>

// confronto di diversi metodi per evitare race conditions 
// nell'aggioramento di una variabile intera condivisa 

// Se viene definito USE_MUTEX  la race condition
// viene evitata con un mutex

// Se viene definito USE_ATOMIC_SUM la race condition
// viene evitata meorizzando la somma in una variabile atomic_long
// che quindi può essere aggiornata in maniera atomica 
// con la funzione atomic_fetch_add

// Altrimenti la race condition viene evitata usando un 
// spinlock, che è una sorta di semplice mutex con attesa attiva

// il make crea gli eseguibili per tutte e tre le versioni:
//    muspinlock.out  atspinlock.out e spinlock.out

// gli esperimenti mostrano che l'uso della variabile atomic è 
// piu veloce del mutex che e piu veloce dello spinlock 
// viene utilizzata una pthread barrier per far partire tutti 
// i thread allo stesso momento allo scopo di massimizzare la
// probabilità di accessi simultanei alla variabile condivisa


typedef struct {
    #if defined(USE_MUTEX)
      long *somma;           // somma non atomica  + mutex
      pthread_mutex_t *lock;
    #elif defined(USE_ATOMIC_SUM)
      atomic_long *somma;    // somma atomica
    #else
      long *somma;           // somma non atomica
      atomic_flag *lock;    // spinlock 
    #endif
    pthread_barrier_t *barrier; // barriera per far partire i thread contemporaneamente
    int num_somme;              // numero di somme da eseguire
} dati;

void *tbody(void* arg) {
    dati *d = (dati *)arg;
    pthread_barrier_wait(d->barrier);
    for(int i=0;i<d->num_somme;i++) {
      #if defined(USE_MUTEX)
        pthread_mutex_lock(d->lock);
        *(d->somma) += i;
        pthread_mutex_unlock(d->lock);
      #elif defined(USE_ATOMIC_SUM)
        atomic_fetch_add(d->somma, i);
        // equivalente a *(d->somma) += i;
      #else // spinlock
        while(atomic_flag_test_and_set(d->lock))
          ;
        *(d->somma) += i;// *(d->somma) non è atomico
        // equivalente a   False -> d->lock
        atomic_flag_clear(d->lock);
      #endif
    }
    return NULL;
}
 


int main(int argc, char* argv[]) {
    if(argc!=3) {
        fprintf(stderr, "Usage: %s <num_thread> <num_somme>\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    int num_somme = atoi(argv[2]);
    int num_thread = atoi(argv[1]);
    pthread_t t[num_thread];

    // Inizializza la struttura dati condivisa
    dati d;
    #if defined(USE_MUTEX)
      long somma = 0;
      d.somma = &somma;
      pthread_mutex_t mu= PTHREAD_MUTEX_INITIALIZER;
      d.lock = &mu;
    #elif defined(USE_ATOMIC_SUM)
      atomic_long somma = ATOMIC_VAR_INIT(0);
      d.somma = &somma;
    #else
      atomic_flag lock = ATOMIC_FLAG_INIT;
      d.lock = &lock;
      long somma=0;
      d.somma = &somma;
    #endif
    pthread_barrier_t b;
    pthread_barrier_init(&b,NULL,num_thread); 
    d.barrier = &b; 
    d.num_somme = num_somme;


    // Create threads
    for (int i = 0; i < num_thread; i++) {
        if (pthread_create(&t[i], NULL, tbody, &d) != 0) {
            perror("pthread_create");
            exit(EXIT_FAILURE);
        }
    }
    // Wait for all threads to complete
    for (int i = 0; i < num_thread; i++) {
        pthread_join(t[i], NULL);
    }
    // Destroy the barrier
    pthread_barrier_destroy(&b);
    // Print the result
    printf("somma = %ld\n", somma);
    printf("Valore atteso: %ld\n", ((long)num_somme*(num_somme-1)/2)*num_thread);
    return 0;
}

