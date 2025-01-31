# Introduzione a Python


## Python vs C


La sintassi di Python è simile a quella del C tranne che per le sequenti semplificazioni:

 * Non è richiesto il `;` alla fine di ogni linea
 * invece delle `{ }` i blocchi sono evidenziati mediante indentazione (attenti all'editor) e con il carattere `:`
 * Le parentesi `( )` nei test degli `if`/`while`   non sono necessarie
 * `/` vs `//` per distinguere tra divisione e divisione intera. Per i commenti si usa il carattere `#` 
 * non ci sono  `++` e `--`
 * operatori logici si chiamano con il loro nome: `and`, `or`, `not` etc
 * Invece di `true`/`false`/`NULL` ci sono `True`/`False`/`None` 


Python è un linguaggio interpretato non compilato: molto più lento del C (fino a 100+ volte); gli errori vengono segnalati a runtime con messaggi solitamente comprensibili.

Altra differenza sostanziale è che le variabili non hanno tipo e non vanno dichiarate.

Le stringhe sono immutabili e facili da usare. Non c'è il tipo carattere: un carattere è una stringa di lunghezza uno.

Gli interi hanno precisione illimitata: ci pensa l'interprete ad usare un numero adeguato di byte per rappresentare tutte le cifre (fate la prova con il fattoriale)

## Strutture di controllo e iterazione

 * if/else/elif
 * while/continue/break, esiste un comando else per i cicli, non esiste do
 * for è basato sul concetto di range, simile al for-each di Java

## Definizione e invocazione di funzioni

Esempio di funzione che dati due interi restituisce la somma e la stringa contenente la loro concatenazione. Non è una funzione molto sensata: serve per far vedere che è possibile restituire più di un oggetto anche di tipo diverso.

```python
# definizione 
def fun(a,b):
    return a+b,str(a) +str(b)

# invocazione funzione e stampa risultato
s,p = fun(3,5)
print(s,p)
```

## Docstring

Una stringa delimitata da tre virgolette `"""` nella riga immediatamente successiva alla definizione di una funzione di chiama *docstring* e forma un commento che l'interprete associa alla funzione. Viene mostrata con il comando *help* e da alcuni editor specifici per python. Ad esempio

```python
def media(a,b):
  """Calcola la media artimetica"""
  return (a+b)/2

help(media)
Help on function media in module __main__:

media(a, b)
    Calcola la media aritmetica
```



## Costruzione stampa e modifica di una lista
```python
lista = [1,2,3,"ciao"]
print(lista[2:]) # estrae gli elementi da lista[2] alla fine 
lista[2] = 5
lista = lista + [34,40]
lista.append(35)
print(lista)
```

Si noti che le liste in python sono analoghe agli array del C in quanto possiamo accedere all'iesimo elemento in tempo costante scrivendo `a[i]` (ma non dobbiamo preoccuparci di allocazioni e deallocazioni). Malgrado il nome non sono analoghe alle *linked list* del C dove l'accesso agli elementi avviene seguendo i puntatori del campo *next*.




### Esempi di iterazione su una lista usando il ciclo for
```python
parole = ["casa","mare","sole"]
for x in parole:
    print(x.upper())
    x = "monti"
print(parole)
for i in range(len(parole)):
    print(parole[i].upper())
    i = 7
print(parole)

```
Nota: la parola chiave `in` si usa nei cicli for e per testare l'appartenenza. Verificatelo eseguendo lea seguenti istruzioni:
```python
print(4 in [1,2,3])
print(4 in [2,4,8])
```

## Tuple 

Le tuple sono sequenze di oggetti molto simili alle liste; vengono definite e stampate utilizzando le parentesi tonde invece che quadre. La differenza sostanziale è che le tuple sonn immutabili (come le stringhe): una volta creata non possiamo modificarle, in particolare non possiamo modificare i singoli elementi:
```python
a = [2, 3, 5, 7, 11]  # a è una lista
a[1] = 13
print(a)  # -->  [2, 13, 5, 7, 11]
b = (2, 3, 5, 7, 11)  # b è una tupla
b[1] = 13
TypeError
----> 1 b[1] = 13
TypeError: 'tuple' object does not support item assignment
```
Vedremo nel seguito il motivo per cui è utile avere una versione immutabile delle liste.


## Esecuzione di programmi

Inizialmente eseguiremo i programmi chiamando le funzioni all'interno dell'interprete python (prompt `>>>` oppure `[1]`). L'interprete ha il vantaggio di permettere di testare le singole istruzioni/funzioni e di vedere il contenuto delle variabili.

Successivamente eseguiremo i programmi python dalla linea di comando del terminale (in questo caso vengono chiamati `script`). Per poterli eseguire dalla linea di comando è necessario che il file contenente il sorgente inizi con la linea
```python
#!/usr/bin/env python3
```
e che il file venga reso eseguibile con il comando `chmod(1)`


## Lettura dei parametri sulla linea di comando

Per poter accedere ai parametri passati sulla linea di comando come nel C, è necessario importare il modulo `sys` con l'istruzione (solitamente messa all'inzio del file)
```python
import sys
```
A questo punto i parametri passati sulla linea di comando sono disponibili nella lista di stringhe `sys.argv`; quindi `sys.argv[0]` sarà il nome del programma python, `sys.argv[1]` il nome del primo parametro, e così via. Il numero totale di parametri si ottiene con `len(sys.argv)`.


## Dizionari

Sono equivalenti alle `HashMap` di Java ma più semplici da usare. Consistono in una collezione chiavi, dove ad ogni chiave è associato un valore.  

```python
d = {"casa":3, "sole":4}
d["mare"] = 5
d["casa"] = 10
print(d)
for c in d:
    print("Alla chiave", c, "è associato il valore", d[c])
```

I valori possono esere qualsiasi, mentre le chiavi devono essere essere oggetti "immutabili", quindi che non possono (o che non devono) essere modificati. Per questo motivo gli array non possono essere usate come chiavi in un dizionario, per associare ad esempio un valore ad una coppia di interi è necessario usare una tupla:
```python
d[ [1,2] ] = "coppia"
TypeError: unhashable type: 'list'
d[ (1,2) ] = "coppia"
print(d)
{'casa': 10, 'sole': 4, "mare": 5, (1, 2): 'coppia'}
```
Per cancellare un elemento da un dizionario si utilizza il comando `del`:
```python
del d['casa']
```


## Insiemi 

Sono equivalenti agli `HashSet` di Java, ma più semplici da usare. Consistono in una semplice collezione di chiavi senza nessun valore associato. Le operazioni che possiamo fare sono quindi di aggiunta di un elemento (`add`), cancellazione (`remove`), e verifica se un elemento è contenuto nell'insieme (`in`):
```python
s = set()   # crea un insieme vuoto
s.add("rosso")
s.add("verde")
s.add("bianco")
print(s)
{'rosso', 'bianco', 'verde'}
"rosso" in s
True
s.remove("rosso")
"rosso" in s
False
``` 
Mediante un ciclo `for` possiamo poi effettuare la scansione degli elementi di un insieme:
```python
for c in s:  print("  s contiene:", c)
  s contiene: bianco
  s contiene: verde
```
Insiemi e dizionari in python sono entrambi realizzati internamente con tabelle hash, quindi gli oggetti che possono essere memorizzati in un `set` sono gli stessi che si possono usare come chiavi nei dizionari (cioè quelli immutabili).



## Esercizi python 

### Ribalta file (30/1/25)

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
Nota: dopo aver letto le linee in una array, ordinarlo mendiante il metodo `sort` utilizando come chiave la lunghezza della stringa.



