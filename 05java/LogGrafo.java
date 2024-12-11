import java.util.Map;
import java.util.HashMap;
import java.util.TreeSet;
import java.util.SortedSet;


// implementazione della classe Grafo
// basata su TreeMap e TreeSet
// per memorizzare nodi e archi

/**
 * Classe per rappresentare un grafo pesato non orientato
 * basata su HashMap per i nodi e TreeSet per gli archi pesati
 */

public class LogGrafo extends Grafo {
	// insieme di tutti gli archi del grafo
	private SortedSet<Arco> archi;
  // nodi rappresentati come insiemi di archi uscenti
	// ordinati per peso crescente
	private Map<Nodo,SortedSet<Arco>> nodi;

	/** costruttore: inizializza insiemi di archi e nodi 
	 *  inizialmente vuoti
	 * */ 
	public LogGrafo() {
		nodi = new HashMap<Nodo,SortedSet<Arco>>();
		archi = new TreeSet<Arco>();
	}

	public int num_nodi() {
		return nodi.size();
	}

	public int num_archi() {
		return archi.size();
	}

	// verifica se il grafo contiene un nodo
	public boolean contiene_nodo(Nodo n) {
		return nodi.containsKey(n);
	}

	// aggiunge nodo al grafo
	public void aggiungi_nodo(Nodo n) {
		if(nodi.containsKey(n))
		  throw new IllegalArgumentException("Nodo duplicato");
		nodi.put(n,new TreeSet<Arco>());
	} 

	public void aggiungi_arco(Arco a) {
		if(!nodi.containsKey(a.n1))
		  throw new IllegalArgumentException("Primo estremo mancante");
		if(!nodi.containsKey(a.n2))
		  throw new IllegalArgumentException("Secondo estremo mancante");
		for(Arco x : archi) {
			if(a.stessi_estremi(x))
			  throw new IllegalArgumentException("Arco duplicato");
		}
		// aggiunge arco a uscente da n1	e n2
		nodi.get(a.n1).add(a);
		nodi.get(a.n2).add(a);
		archi.add(a);
	}

	// restituisce insieme di archi uscenti da un nodo
	// ordinati per peso crescente
  public SortedSet<Arco> uscenti(Nodo n) {
		if(!nodi.containsKey(n))
		  throw new IllegalArgumentException("Nodo sconosciuto");
		return nodi.get(n);
	}

	// restituisce insieme di tutti gli archi del grafo
	// ordinati per peso crescente
	public SortedSet<Arco> archi() {
		return this.archi;
	}


	/**
	 * metodo main per testare la classe
	 * utilizzando nodi di tipo stringa
	 * 
	 * @param args non usato
	 */
	public static void main(String[] args) {
		// classe di prova per testare il grafo
		class Stringa implements Nodo {
			String nome;
			Stringa(String nome) {this.nome = nome;}
			public String etichetta() {return nome;}
		}
		
    // costruisco un grafo con 4 nodi e 4 archi
		// i cui nodi sono oggetti di tipo stringa
		LogGrafo g = new LogGrafo();
		Stringa a = new Stringa("a");
		Stringa b = new Stringa("b");	
		Stringa c = new Stringa("c");
		Stringa d = new Stringa("d");
		g.aggiungi_nodo(a);
		g.aggiungi_nodo(b);
		g.aggiungi_nodo(c);
		g.aggiungi_nodo(d);
		g.aggiungi_arco(new Arco(a,b,1.0));
		g.aggiungi_arco(new Arco(a,c,3.0));
		g.aggiungi_arco(new Arco(b,c,1.0));
		g.aggiungi_arco(new Arco(c,d,1.0));
		// calcolo i cammini minimi da a
		Map<Nodo,Cammino> m = g.dijkstra(a);
		for(Nodo n : m.keySet()) {
			System.out.println(n.etichetta() + " " + m.get(n));
		}
	}

}

