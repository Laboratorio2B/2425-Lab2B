## Compilazione ed esecuzione da riga di comando

Nel caso sia necessario compilare ed eseguire i programmi java dalla linea di comando utilizzare il comando `javac`. Ad esempio si possono compilare tutti i file con estensione `java` nella directory corrente scrivendo
```bash
javac *.java
```  
Questo comando crea un file `.class` per ognuna delle classi contenute nei file `.java` (i nomi dei file `.class` sono quindi quelli delle classi, non dei sorgenti). Se una classe contiene al suo interno il metodo 
`public static void main(String[] args)`  allora è possibile eseguire il codice di tale metodo scrivendo ad esempio
```bash
java GrafoCitta italiane.txt 90
```
in questo caso i parametri "italiane.txt" e "90" sono memorizzati nell'array di stringhe `args` argomento di `main`. Si noti che in questo esempio `args` avrebbe due elementi, non 3 come in C.


## Creazione documentazione in html

Per creare la documentatione di tutte le classi e metodi `public`  dei file java della directory corrente dare il comando
```bash
javadoc -d doc *.java
```
La documentazione è consultabile partendo ad esempio da `doc/overview-tree.html`. Per creare una buona documentazione è necessario utilizzare dei commenti con gli opportuni tags, si veda ad esempio la pagina [wiki/Javadoc](https://en.wikipedia.org/wiki/Javadoc)


## Esercizio


### Calcolo del minimum spanning tree (11/12/24)


Aggiungere alla classe `Grafo` il metodo
```
List<Arco> mst()
```
che calcola il minimum spanning tree di un grafo mediante l'[algoritmo di Prim](https://en.wikipedia.org/wiki/Prim's_algorithm). L'algoritmo deve restituire la lista degli archi che fanno parte di un minimum spanning tree, o di una minimum spanning forest nel caso il grafo non sia connesso. Testare l'algoritmo sul grafo fornito dalle città italiane. Per gli esperimenti si tenga conto che la distanza (il secondo parametro sulla linea di comando) deve essere almeno 320 Km affinché il grafo risulti connesso.  

Per l'implementazione dell'algoritmo è necessario mantenere l'insieme dei nodi appartenenti all'albero (consiglio usare un `HashSet`) e l'insieme degli archi che collegano un nodo interno all'albero con un nodo all'esterno (consiglio di usare un `TreeSet`, ricordo che la classe `Arco` è  `Comparable<Arco>` con un ordinamento basato sulla lunghezza dell'arco).

