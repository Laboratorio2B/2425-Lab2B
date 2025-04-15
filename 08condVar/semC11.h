// implementation of posix semaphores using c11 threads library
#include <threads.h>

// Semaphore structure
typedef struct {
  int value;       // Semaphore value
  mtx_t mutex;     // Mutex to protect the semaphore
  cnd_t cond;      // Condition variable for signaling
} sem11_t;

// Initialize the semaphore
int sem11_init(sem11_t *sem, int value) {
  if (value < 0) {
      return -1; // Semaphore value must be non-negative
  }
  sem->value = value;
  if (mtx_init(&sem->mutex, mtx_plain) != thrd_success) {
      return -1;
  }
  if (cnd_init(&sem->cond) != thrd_success) {
      mtx_destroy(&sem->mutex);
      return -1;
  }
  return 0;
}

// Wait (decrement) the semaphore
int sem11_wait(sem11_t *sem) {
  mtx_lock(&sem->mutex);
  while (sem->value <= 0) {
      cnd_wait(&sem->cond, &sem->mutex); // Wait until the semaphore value is positive
  }
  sem->value--; // Decrement the semaphore value
  mtx_unlock(&sem->mutex);
  return 0;
}

// Post (increment) the semaphore
int sem11_post(sem11_t *sem) {
  mtx_lock(&sem->mutex);
  sem->value++; // Increment the semaphore value
  cnd_signal(&sem->cond); // Signal one waiting thread
  mtx_unlock(&sem->mutex);
  return 0;
}

// Destroy the semaphore
int sem11_destroy(sem11_t *sem) {
  mtx_destroy(&sem->mutex);
  cnd_destroy(&sem->cond);
  return 0;
}

