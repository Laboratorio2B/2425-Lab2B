#define _GNU_SOURCE   // permette di usare estensioni GNU
#include <stdio.h>    // permette di usare scanf printf etc ...
#include <stdlib.h>   // conversioni stringa exit() etc ...
#include <stdbool.h>  // gestisce tipo bool
#include <assert.h>   // permette di usare la funzione ass
#include <string.h>   // funzioni per stringhe
#include <errno.h>    // richiesto per usare errno
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <semaphore.h>
#include <sys/mman.h>
#include <sys/stat.h>        /* For mode constants */
#include <fcntl.h>           /* For O_* constants */
#include <pthread.h>

// termina programma
void termina(const char *s); 
void xtermina(const char *s, int linea, char *file); 

// operazioni su FILE *
FILE *xfopen(const char *path, const char *mode, int linea, char *file);

// operazioni su file descriptors
void xclose(int fd, int linea, char *file);

// thread
void xperror(int en, char *msg);

int xpthread_create(pthread_t *thread, const pthread_attr_t *attr,
                          void *(*start_routine) (void *), void *arg, int linea, char *file);
int xpthread_join(pthread_t thread, void **retval, int linea, char *file);

// mutex 
int xpthread_mutex_init(pthread_mutex_t *restrict mutex, const pthread_mutexattr_t *restrict attr, int linea, char *file);
int xpthread_mutex_destroy(pthread_mutex_t *mutex, int linea, char *file);
int xpthread_mutex_lock(pthread_mutex_t *mutex, int linea, char *file);
int xpthread_mutex_unlock(pthread_mutex_t *mutex, int linea, char *file);

// barrier
int xpthread_barrier_init(pthread_barrier_t *restrict barrier, const pthread_barrierattr_t *restrict attr, 
                          unsigned int count, int linea, char *file);
int xpthread_barrier_destroy(pthread_barrier_t *barrier, int linea, char *file);
int xpthread_barrier_wait(pthread_barrier_t *barrier, int linea, char *file);

// semafori named e unnamed POSIX
sem_t *xsem_open(const char *name, int oflag, mode_t mode, unsigned int value, int linea, char *file);
int xsem_unlink(const char *name, int linea, char *file);
int xsem_close(sem_t *sem, int linea, char *file);
int xsem_init(sem_t *sem, int pshared, unsigned int value, int linea, char *file);
int xsem_destroy(sem_t *sem, int linea, char *file);
int xsem_post(sem_t *sem, int linea, char *file);
int xsem_wait(sem_t *sem, int linea, char *file);

// condition variables
int xpthread_cond_init(pthread_cond_t *restrict cond, const pthread_condattr_t *restrict attr, int linea, char *file);
int xpthread_cond_destroy(pthread_cond_t *cond, int linea, char *file);
int xpthread_cond_wait(pthread_cond_t *restrict cond, pthread_mutex_t *restrict mutex, int linea, char *file);
int xpthread_cond_signal(pthread_cond_t *cond, int linea, char *file);
int xpthread_cond_broadcast(pthread_cond_t *cond, int linea, char *file);

