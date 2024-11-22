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

* Warning di valore ignorato. Comando `man`, sezioni del manuale, 
* Rappresentazione di stringhe in C: i tipi `char` e `char *`. Stampa di stringhe con `printf`, calcolo lunghezza con `strlen`. 
* Uso dei parametri argc/argv di main()
* Copia di un puntatore vs copia degli elementi di un array: la funzione strdup()


-------

**Lezione 5** (1/10/24)

* Introduzione al *makefile*; compilazione con il comando *make*
* Scrittura in un file di testo: fopen(), fprintf(), fclose(). Modalità append.
* Comandi `less` e `od` per visualizzare il contenuto di un file
* Lettura da un file di testo: comando fscanf().  
* Terminazione in caso di errori: funzione perror(). Cenni sulla ridirezione di `stdout`

-------

**Lezione 6** (4/10/24)

* Passaggio di array dalle/alle funzioni
* Uso di `assert`
* stdout/stderr e loro ridirezione; esempi
* Esercitazione sulle stringhe e sul passaggio di array alle funzioni. Significato del tipo `const char *`


-------


**Lezione 7** (8/10/24)


* Equivalenza fra `*a` e `a[0]`
* Equivalenza fra `a` e `&a[0]`, significato di `&a[i]`
* Aritmetica dei puntatori: definizione di `a+i` come `&a[i]`
* Esempio di uso della notazione `&a[i]` nel *mergesort*
* Creazione e deallocazione di un array di stringhe, programma `astringhe`
* Lettura di stringhe con `scanf`:  problematica del [buffer overflow](https://en.wikipedia.org/wiki/Buffer_overflow)

----

**Lezione 8** (15/10/24)


* Lettura di stringhe con `scanf`: evitare il buffer overflow: modificatori `%Ns` (con N massimo numero di caratteri) e `%ms`
* Costruzione/concatenazione di stringhe mediante `asprintf` 
* Definizione di tipi composti con `struct`; definizione di nuovi tipi con `typedef` e uso combinato con `struct`
* Operazioni sui tipi composti, notazioni `.` e  `->`; passaggio per valore e per riferimento di struct. 
* Ordinamento di un array di struct mediante mergesort

----

**Lezione 9** (18/10/24)

* Array di puntatori a struct, esempio: array di capitali. Confronto lessicografico di stringhe, funzione `strcmp`.
* Ordinamento array di puntatori a struct mediante mergesort
* Terminazione in caso di errori: funzione perror() e variabile globale `errno`


----

**Lezione 10** (22/10/24)

* Introduzione all'uso delle funzioni come parametri di altre funzioni
* Mergesort con funzione di confronto passata come argomento
* Esercizi: ordinamento di file per dimensione e nome. Funzioni `fseek` e `ftell`. Uso del tipo `long`


----


**Lezione 11** (25/10/24)


* Introduzione alla funzione di libreria  `qsort`
* Prova in itinere sugli argomenti delle lezioni 1-9. 


----

**Lezione 12** (29/10/24)

* Puntatori generici (`void *`). Tipo della funzione di confronto in `qsort`, casting.
* Introduzione alle *linked list*: creazione, stampa, distruzione di liste. Inserimento in testa e in coda. 


----

**Lezione 13** (4/11/24)

* Linked list: procedure ricorsive per la cancellazione e l'inserimento ordinato. 
* Alberi binari di ricerca: procedure ricorsive per costruzione, visita e deallocazione.  

--- 

**Lezione 14** (5/11/24)

* Alberi binari di ricerca: visita in preorder, ricerca di un elemento, calcolo altezza (file `abr_capitali.c`)
* Lettura da file con getline(). Parsing di stringhe mediante strtok() (file `leggicitta.c`)


--- 

**Lezione 15** (8/11/24)

* Variabili statiche. Funzionamento interno di strtok(). Esempio di lettura di stringhe mediante strtok() (`legginomi.c`)
* Test di primalità in assembler (`primo.s`). Makefile e compilazione di programmi formati da più file. Concetto di target, prerequisiti e comandi. 
* Uso di un debugger: esecuzione passo/passo, breakpoint, visualizzazione variabili locali. 
* Accesso a sistemi remoti via ssh con X11 forwarding. Uso di gdbgui per debugging su sistemi remoti attraverso un browser.


--- 

**Lezione 16** (12/11/24)

* File oggetto (`.o`) e compilazione separata; makefile con target multipli. 
* Implementazione `strcmp` in assembler ARM.
* Breakpoint condizionali in gdb/gdbgui. Visualizzazione registri ARM e loro uso nei breakpoint condizionali.

--- 

**Lezione 17** (13/11/24)

* Ancora sul makefile: lazy evaluation, variabili, variabili automatiche, e regole implicite.
* Operatori bitwise `&`, `|`, `^`, `~`, `<<`, `>>`. Esempio: `bitops.c`
* Utilizzo di puntatori e costruzione di un array di numeri primi in assemler ARM (`array_primi.s`)

-----

**Lezione 18** (19/11/24)

* Esercizi su ABR.
* Definizione e uso di matrici bidimensionali definite in modo statico (ad esempio `int a[3][5]`).


---

**Lezione 19** (22/11/24)

* Definizione e uso di matrici bidimensionali definite in modo dinamico come array di righe.
* Seconda prova in itinere.


----

**Lezione 20** (26/11/24)

* Lettura e scrittura di file binari in C
