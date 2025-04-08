## Esercizi





### Alice, Bob e i segnali real-time (3/4/25)


Si consideri il seguente gioco tra Alice e Bob: si parte da un intero *N*, a turno ogni giocatore può: 

* se *N* pari sottrarre 1 da *N* oppure dimezzarlo
* se *N* è dispari può solo sottrarre 1 (la mossa è forzata)

vince che raggiunge il valore zero. Ad esempio, se il valore iniziale è 10 una possibile sequenza di gioco è: 

* Bob: 12 -> 6
* Alice: 6 -> 5
* Bob: 5 -> 4 
* Alice: 4 -> 2 
* Bob: 2 -> 1 
* Alice 1 -> 0 e vince 


Scrivere una coppia di programmi `alice` e `bob` che effettuano un certo numero di partite secondo il seguente schema:

1. `alice` chiede all'utente (con `scanf`) un intero non negativo e lo comunica a `bob` con un segnale; 

2. Se il valore *N* fornito dall'utente è zero entrambi i programmi terminano, altrimenti iniziano a giocare e `bob` esegue la prima mossa

3. I programmi devono fare mosse valide (non importa se giocano male) e comunicare la mossa scelta all'avversario utilizzando un segnale (la mossa va comunicata anche quando è forzata)

4. Chi riceve la mossa deve verificarne la correttezza e inviare la sua mossa di risposta ancora con un segnale

5. Quando la partita finisce ogni programma deve stampare un messaggio `ha vinto Alice` oppure `ha vinto Bob` e poi si torna al punto 1 per una eventuale nuova partita.

Idealmente i due programmi devono fare sempre mosse valide, accorgersi che la partita è finita e stampare lo stesso messaggio alla fine di ogni partita. 


Dovete scrivere due sorgenti diversi `alice.c` e `bob.c`; l'utente deve eseguire dalla di comando `alice` che deve far partire `bob` con una `fork` + `exec`; questo è necessario perchè ogni programma deve poter conoscere il `pid` dell'altro per inviare i segnali. Usate i segnali real-time con la funzione `sigqueue(3)` per inviare un intero, e `sigwaitinfo(2)` per la ricezione dei segnali.


