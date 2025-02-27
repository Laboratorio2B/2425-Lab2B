

## Esercizi



### Sort parallelo (30/1/25)

Il programma `threadSort.c` utilizza `qsort` per ordinare un array di interi. Modificarlo in maniera da velocizzare l'ordinamento utilizzando *un* thread ausiliario. L'ordinamento deve essere fatto secondo il seguente schema:

1. l'array da ordinare viene logicamente diviso in due: la prima metà viene ordinata dal thread ausiliario, la seconda metà dal theread principale. Gli ordinamenti delle due metàpossono essere fatte con `qsort`

2. Le due metà ordinate devono essere combinate nel risultato finale utilizzando la funzione `merge()` che trovate nel sorgente. 


Completata questa prima parte, scrivere una nuova versione che utilizza *tre* thread ausiliari (quindi quattro in totale). Una coppia di thread deve effettare il sorting della prima metà utilizzando la strategia vista sopra, una seconda coppia ordina la seconda metà e alla fine il thread principale effettua il merge finale.

E' possibile risolvere l'esercizio utilizzando solo le funzione `pthread_create` e `pthread_join`, ma è comunque ammesso l'uso di mutex e semafori. 









Scrivere un programma python `reverse.py` che invocato dalla linea di comando scrivendo 
```bash
reverse.py nomefile
```
legge le linee del file di testo `nomefile` e le scrive in ordine inverso su un nuovo file chiamato `nomefile.rev`. Ad esempio se il file `test.txt` contiene le linee
```
prima riga

questa è la terza riga
la seconda era vuota
```
deve essere creato il file `test.txt.rev` contenente
```
la seconda era vuota
questa è la terza riga

prima riga
```
Nota: aprire il file e leggere le linee come nell'esempio `serieA.py`. Per aprire il file `nome` in scrittura usare
```python
with open(nome,"w") as g:
```
e per scrivere una stringa nel file `g` usare la funzione `print` con la keyword `file` nel seguente modo 
```python
print("ciao", file=g)
```

### Ordina file (30/1/25)

Scrivere un programma python `ordina.py` che invocato dalla linea di comando scrivendo 
```bash
reverse.py nomefile
```
legge le linee del file di testo `nomefile` e le scrive in ordine di lunghezza decrescente 
su un nuovo file chiamato `nomefile.dec`. Ad esempio per il file `test.txt` dell'esercizio precedente dovrebbe essere creato un file `test.txt.dec` contente le linee:
```
questa è la terza riga
la seconda era vuota
prima riga


```
Nota: dopo aver letto le linee in un array, ordinarlo mediante il metodo `sort` utilizzando come chiave la lunghezza della stringa.



### File profondi (11/2/25)

Scrivere un programma `profondi.py` che invocato dalla linea di comando scrivendo 
```bash
profondi.py nomedir
```
esplora la directory `nomedir` e le sue sottodirectory  e visualizza l'elenco dei file che si trovano a maggiore profondità nell'albero con radice `nomedir` ordinando l'elenco per dimensione di file crescente.
Ad esempio, se la directory `nomedir` ha la seguente struttura:
```
nomedir
├── dir0_d1
│   ├── dir0_d0
│   │   ├── filea0.txt
│   │   └── filea1.txt
│   ├── file0.txt
│   └── file1.txt
├── dir1_d1
│   ├── dir1_d0
│   │   ├── fileb0.txt
│   │   └── fileb1.txt
│   ├── file0.txt
│   └── file1.txt
├── file0.txt
└── file1.txt

```
l'output deve consistere dei 4 file `filea0.txt`, `filea1.txt`, `fileb0.txt`, `fileb1.txt` ordinati per dimensione di file crescente. 

*Suggerimento:* partire dall'esempio `duplicati.py` e modificare la classe `Miofile` in modo che memorizzi anche la profondità a cui il file e stato trovato, e modificare il metodo `__lt__` in modo da poter ordinare una lista di file per profondità e dimensione. 







