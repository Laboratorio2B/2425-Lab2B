/*
 * Esempio di implementazione di uno stack lock-free utilizzando
 * una variabile atomica (il puntatore all'inzio dello stack)
 * e l'operazione atomic_compare_exchange_weak()
 * 
 * Se viene definita la costante USE_MUTEX il sorgente 
 * gestisce il puntatore all'inizio dello stack utilizzando 
 * un mutex tradizionali.
 * 
 * il comando make crea gli eseguibili per entrambe le versioni
 * 
 * Sfortunatamente la versione lock free non è corretta in quanto
 * occasionalmente termina per un double-free, oppure entra in lock
 * nei prossimi giorni cercherò di capire quale sia il problema
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdatomic.h>
#include <pthread.h>
#include <sys/times.h>

// struct che rappresenta un singolo nodo dello stack
typedef struct Node {
    int value;
    struct Node* next;
} Node;

// struct che rappresenta la testa dello stack.
// Utilizziamo un puntatore atomico perchè le operazioni di pop e push
// devono agire su esso in modo atomico.
#ifndef USE_MUTEX
typedef struct {
    _Atomic(Node*) head;
} LockFreeStack;
#else
typedef struct {
  Node* head;
  pthread_mutex_t *mu; 
} LockFreeStack;
#endif


// struttura di input per i singoli thread
typedef struct {
    LockFreeStack stack;     // stack
    atomic_int pop_count;    // numero di pop eseguiti
    atomic_long pop_sum;     // somma dei valori estratti
    int num_push_threads;
    int num_pop_threads;
    int num_values_per_push;
    atomic_int push_id;      // usato per fornire id ai thread-push
} SharedData;


#ifndef USE_MUTEX
// Push operation: atomically inserts a new node at the head.
void push(LockFreeStack* stack, int value) {
    Node* new_node = malloc(sizeof(Node));
    if (!new_node) {
        perror("malloc");
        exit(EXIT_FAILURE);
    }
    new_node->value = value;
    
    Node* old_head = atomic_load(&stack->head);    // Read the current head
    do {
        // re-read the current head, not necessary since old_head is updated by atomic_compare_exchange  
        // old_head = atomic_load(&stack->head); 
        new_node->next = old_head;               // Set next pointer for the new node
    } while (!atomic_compare_exchange_weak(&stack->head, 
                             &old_head, new_node));
}

// Pop operation: atomically removes the head node from the stack.
// Returns 1 if successful (and stores the popped value), or 0 if the stack was empty.
int pop(LockFreeStack* stack, int *value) {
    Node* old_head;
    do {
        old_head = atomic_load(&stack->head);
        if (old_head == NULL) {
            return 0;  // Stack is empty.
        }
    } while (!atomic_compare_exchange_weak(&stack->head, &old_head, old_head->next));
    
    *value = old_head->value;
    free(old_head);
    return 1;
}
#else
// pop() e push() basta sui mutex
void push(LockFreeStack* stack, int value) {
  Node* new_node = malloc(sizeof(Node));
  if (!new_node) {
      perror("malloc");
      exit(EXIT_FAILURE);
  }
  new_node->value = value;
  pthread_mutex_lock(stack->mu);
  new_node->next = stack->head;
  stack->head = new_node;
  pthread_mutex_unlock(stack->mu);
}
// Returns 1 if successful (and stores the popped value), or 0 if the stack was empty.
int pop(LockFreeStack* stack, int *value) {
  pthread_mutex_lock(stack->mu);
  Node *old_head = stack->head;
  if(old_head == NULL) {
    pthread_mutex_unlock(stack->mu);
    return 0;
  }
  stack->head = old_head->next;
  pthread_mutex_unlock(stack->mu);
  *value = old_head->value;
  free(old_head);
  return 1;
}
#endif


// parte comune a atomic e mutex

// Producer thread function: pushes a series of unique values onto the stack.
void *push_thread(void* arg) {
    SharedData* data = (SharedData*)arg;
    int thread_id = atomic_fetch_add(&data->push_id, 1);
    // Each thread pushes a series of values based on its ID.
    for (int i = 0; i < data->num_values_per_push; i++) {
        int value = thread_id * data->num_values_per_push + i;
        push(&data->stack, value);
    }
    return NULL;
}

// Consumer thread function: pops values from the stack until all expected items are popped.
void *pop_thread(void* arg) {
    SharedData* data = (SharedData*)arg;
    while (1) {
        int value;
        if (pop(&data->stack, &value)) {
            atomic_fetch_add(&data->pop_count, 1);
            atomic_fetch_add(&data->pop_sum, value);
        } else {
            int total_pushed = data->num_push_threads * data->num_values_per_push;
            int total_popped = atomic_load(&data->pop_count);
            if (total_popped >= total_pushed) {
                break;
            }
            sched_yield();  // passa la CPU ad altri thread se lo stack è vuoto
        }
    }
    return NULL;
}



int main(int argc, char* argv[]) {

    // legge il numero di thread da riga di comando
    if(argc!=4) {
        fprintf(stderr, "Usage: %s <num_push_threads> <num_pop_threads> <num_values_per_push>\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    int num_push_threads = atoi(argv[1]);
    int num_pop_threads = atoi(argv[2]);
    int num_values_per_push = atoi(argv[3]);

    // inizia misurazione runtime
    clock_t start = times(NULL);

    // Inizializza la struttura dati condivisa
    SharedData data = {
        .stack = { .head = NULL },
        .pop_count = ATOMIC_VAR_INIT(0),
        .pop_sum = ATOMIC_VAR_INIT(0),
        .num_push_threads = num_push_threads,
        .num_pop_threads = num_pop_threads,
        .num_values_per_push = num_values_per_push,
        .push_id = ATOMIC_VAR_INIT(0)
    };
    #ifdef USE_MUTEX
    pthread_mutex_t mu= PTHREAD_MUTEX_INITIALIZER;
    data.stack.mu = &mu;
    #endif

    pthread_t push_threads[data.num_push_threads];
    pthread_t pop_threads[data.num_pop_threads];


    // Create producer (push) threads.
    for (int i = 0; i < data.num_push_threads; i++) {
        if (pthread_create(&push_threads[i], NULL, push_thread, &data) != 0) {
            perror("pthread_create");
            exit(EXIT_FAILURE);
        }
    }

    // Wait for push threads to complete.
    for (int i = 0; i < data.num_push_threads; i++) {
        pthread_join(push_threads[i], NULL);
    }
    
    // Create consumer (pop) threads.
    for (int i = 0; i < data.num_pop_threads; i++) {
        if (pthread_create(&pop_threads[i], NULL, pop_thread, &data) != 0) {
            perror("pthread_create");
            exit(EXIT_FAILURE);
        }
    }
    
    // Wait for pop threads to complete.
    for (int i = 0; i < data.num_pop_threads; i++) {
        pthread_join(pop_threads[i], NULL);
    }

    // termina misurazione runtime
    clock_t end = times(NULL);

    // verifica il numero di push e pop eseguiti
    int total = atomic_load(&data.pop_count);
    printf("Total values pushed: %d\n", data.num_push_threads * data.num_values_per_push);
    printf("Total values popped: %d\n", total);

    // verifica che lo stack sia vuoto
    if (atomic_load(&data.stack.head) == NULL)
         printf("Stack is empty\n");
    else
         printf("Stack is not empty\n");

    // verifica che sono stati letti i valori corretti
    long sum = atomic_load(&data.pop_sum);
    printf("Sum of values popped: %ld\n", sum);
    long expected_sum = data.num_push_threads * data.num_values_per_push;
    printf("Expected sum: %ld\n", (expected_sum * (expected_sum - 1)) / 2);

    // termina mostrando il tempo di esecuzione
    double elapsed_time = (double)(end - start) / sysconf(_SC_CLK_TCK);
    printf("Elapsed time: %.3f milliseconds\n", 1000*elapsed_time);

    return 0;
}
