#!/usr/bin/env python3
import threading
import queue
import sys
import math

# Funzione per verificare se un numero è primo
def primo(n):
    if n < 2:
        return False
    if n % 2 == 0:
        return n == 2
    for i in range(3, int(math.sqrt(n)) + 1, 2):
        if n % i == 0:
            return False
    return True

# Funzione eseguita dai thread consumatori
def cbody(buffer, buffer_lock, sem_free_slots, sem_data_items, results, thread_id):
    quanti = 0
    somma = 0
    print(f"[C] Consumatore {thread_id} partito")
    while True:
        sem_data_items.acquire()
        with buffer_lock:
            n = buffer.get()
        sem_free_slots.release()
        if n == -1:  # Segnale di terminazione
            break
        if primo(n):
            quanti += 1
            somma += n
    results[thread_id] = (quanti, somma)
    print(f"[C] Consumatore {thread_id} terminato")

# Funzione eseguita dai thread produttori
def pbody(nomefile, buffer, buffer_lock, sem_free_slots, sem_data_items):
    print(f"[P] Produttore con file {nomefile} partito")
    try:
        with open(nomefile, "r") as f:
            for line in f:
                n = int(line.strip())
                assert n > 0  # I valori del file devono essere positivi
                sem_free_slots.acquire()
                with buffer_lock:
                    buffer.put(n)
                sem_data_items.release()
    except FileNotFoundError:
        print(f"Errore: file {nomefile} non trovato")
    print(f"[P] Produttore con file {nomefile} terminato")

def main():
    if len(sys.argv) < 2:
        print(f"Uso: {sys.argv[0]} file1 [file2 file3 ... filek]")
        sys.exit(1)

    cons = 3  # Numero di thread consumatori
    assert cons > 0
    prod = len(sys.argv) - 1  # Numero di thread produttori
    assert prod > 0

    tot_primi = 0
    tot_somma = 0

    # Buffer condiviso
    buffer = queue.Queue(maxsize=10)
    buffer_lock = threading.Lock()

    # Semafori
    sem_free_slots = threading.Semaphore(10)
    sem_data_items = threading.Semaphore(0)

    # Creazione e avvio dei thread consumatori
    results = [None] * cons
    consumer_threads = []
    for i in range(cons):
        t = threading.Thread(target=cbody, args=(buffer, buffer_lock, sem_free_slots, sem_data_items, results, i))
        consumer_threads.append(t)
        t.start()
    print("Thread consumatori creati")

    # Creazione e avvio dei thread produttori
    producer_threads = []
    for i in range(prod):
        t = threading.Thread(target=pbody, args=(sys.argv[i + 1], buffer, buffer_lock, sem_free_slots, sem_data_items))
        producer_threads.append(t)
        t.start()
    print("Thread produttori creati")

    # Attesa della terminazione dei thread produttori
    for t in producer_threads:
        t.join()

    # Scrittura dei segnali di terminazione per i consumatori
    for _ in range(cons):
        sem_free_slots.acquire()
        with buffer_lock:
            buffer.put(-1)
        sem_data_items.release()
    print("Valori di terminazione scritti")

    # Attesa della terminazione dei thread consumatori e calcolo del risultato
    for t in consumer_threads:
        t.join()

    for quanti, somma in results:
        tot_primi += quanti
        tot_somma += somma

    print(f"Trovati {tot_primi} primi con somma {tot_somma}")

if __name__ == "__main__":
    main()