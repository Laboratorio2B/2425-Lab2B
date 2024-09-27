##  Introduzione ad array, puntatori e file (introC)


**Lezione 1** (17/9/24)

Introduzione al linguaggio C. Esempio di lettura e scrittura di interi dal terminale. Compilazione: errori e warning. Permessi di lettura scrittura esecuzione `rwx`. Esecuzione di programmi. Array "statici" (tipo `int a[n]` in `somma.c`) vs array "dinamici" gestiti con `malloc/free` utilizzati in `sommad.c`

-----------------------

**Lezione 2** (20/9/24)

* Allocazione dinamica di array in C. Funzioni: malloc/free/realloc. Programma `primi.c`.
* Utilizzo di *valgrind* per verificare il corretto utilizzo e deallocazione degli array

-------

**Lezione 3** (24/9/24)

* Introduzione ai puntatori; uso di puntatori negli array.
* Significato e uso di `*` e `&`
* Esempio passaggio parametri per riferimento attraverso un puntatore: `parametri.c` e `sommadprod.c`
* Significato istruzioni `#include`

-------

**Lezione 4** (27/9/24)

* Warning di valore ignorato. Comando `man`, sezioni del manuale, comando `whatis`. Esempi `printf`/`scanf`
* Rappresentazione di stringhe in C: i tipi `char` e `char *`. Stampa di stringhe con `printf` e `puts`. 
* Uso dei parametri argc/argv di main()
* Introduzione al *makefile*; compilazione con il comando *make*

-------

**Lezione 5** (1/10/24)

* Copia di un puntatore vs copia di un array: la funzione strdup()
* Scrittura in un file di testo: fopen(), fprintf(), fclose(). Modalità append.
* Terminazione in caso di errori: funzione perror() e variabile globale `errno`
