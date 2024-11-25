## Esercizi


### Triangolo di Tartaglia (22/10/24)

Il [triangolo di Tartaglia](https://it.wikipedia.org/wiki/Triangolo_di_Tartaglia) contiene i coefficienti binomali:
```
1
1 1
1 2 1
1 3 3 1
1 4 6 4 1
...
```

Per i=0,1,2,... la riga i-esima contiene i+1 valori
```
t[i][0] ... t[i][i]
```
che soddisfano alla relazione `t[i][0] = t[i][i] = 1` e per *k=1,...,i-1*
```
t[i][k] = t[i-1][k-1] + t[i-1][k]
```

Scrivere una procedura `int **tartaglia(int n)` che dato un intero *n* restituisce le prime *n* righe del triangolo di tartaglia. Si noti che il triangolo può essere visto come una matrice in cui la prima riga ha 1 elemento, la seconda due, etc.
 
Scrivere poi un *main* che legge un intero positivo *n* dalla riga di comando invoca la procedura `tartaglia` passando *n* come argomento, stampa il triangolo risultante e dealloca tutta la memoria utilizzata.

Verificare con valgrind che il programma acceda solo a memoria precedentemente allocata e che al termine del'esecuzione tutta la memoria venga rilasciata.


