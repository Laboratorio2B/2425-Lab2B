

## Esercizi



### Processi consumatori multipli (25/3/25)

Modificare l'esempio `shm_prod.c/shm_cons.c` in modo che 1) vengano utilizzati 3 consumatori e 2) i consumatori (eseguibile `shm_cons.out`) vengono lanciati dal produttore `shm_prod.out` utilizzado `fork` e `exec`.

Si osservi che i consumatori necessitano di una variabile `cindex` condivisa e di un relativo mutex per regolarne l'accesso; a questo scopo conviene che il produttore crei un nuovo blocco di memoria condivisa con un unico intero e un ulteriore semaforo con nome da utilizzare come mutex (in alternativa a creare un nuovo blocco il blocco usato per il buffer può essere creato di dimensione `Buf_size+1`).


