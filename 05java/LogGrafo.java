import java.util.Map;
import java.util.Set;
import java.util.Collections;
import java.util.HashMap;
import java.util.TreeSet;
import java.util.SortedSet;
import java.util.Iterator;
import java.util.LinkedHashMap;


// implementazione della classe Grafo
// basata su TreeMap e TreeSet
// per memorizzare nodi e archi

public class LogGrafo implements Grafo {
	// insieme di tutti gli archi del grafo
	private SortedSet<Arco> archi;
  // nodi rappresentati come insiemi di archi uscenti
	// ordinati per peso crescente
	private Map<Nodo,SortedSet<Arco>> nodi;

	// costruttore: inizializza insiemi di archi e nodi 
	// inizialmente vuoti 
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
	  Calcolo le distanze minime dalla sorgente
		ad ogni altro nodo del grafo 
		
		@param s sorgente da cui calcolare i cammini minimi
		@return mappa che associa ad ogni nodo un Segmento contenente il costo del cammino minimo fino a s e il primo nodo nel cammino
		*/
	  public Map<Nodo,Cammino> dijkstra(Nodo s) 
		{
		// verifica che la sorgente sia nel grafo!
		assert this.contiene_nodo(s): 
		       "La sorgente deve appartenere al grafo";

    // nodi per i quali ho determinato la distanza minima da s
		Map<Nodo,Cammino> risolti = new LinkedHashMap<Nodo,Cammino>();

		// nodi per quali ho trovato almeno un cammino da s 
		// per ogni nodo mantengo il segmento migliore trovato finora
		Map<Nodo,Cammino> raggiunti = new HashMap<Nodo,Cammino>();

		// inizializzo risultato con il nodo sorgente che ha
		// distanza zero da se stesso
		raggiunti.put(s,new Cammino(s,0.0));


    // continuo fino a quando ci sono nodi raggiunti ma non risolti
		while(raggiunti.size()>0) {
			// cerca tra i nodi raggiunti quello con valore associato minimo (attualmente inefficiente)
			Set<Nodo> attivi = raggiunti.keySet();
			Nodo n = Collections.min(attivi, (n1,n2) -> Double.compare(
			           raggiunti.get(n1).costo,raggiunti.get(n2).costo));
			double n_minimo = raggiunti.get(n).costo;
			// sposta n da raggiunti a risolti
			risolti.put(n,raggiunti.remove(n)); 
			// considera archi uscenti da n
			for(Arco a : nodi.get(n)) {
				Nodo m = a.altro_estremo(n);
				double m_dist = n_minimo + a.weight;
				if(risolti.containsKey(m))
				  continue;
				if(!raggiunti.containsKey(m))
				  raggiunti.put(m,new Cammino(n,m_dist));

				else if(raggiunti.get(m).costo>m_dist) {
					raggiunti.get(m).costo = m_dist;
					raggiunti.get(m).precedente = n;
				}
			} // end for Arco
		} // end while 
		return risolti;
	}


  // lancia iteratore partendo dal primo nodo
	public Iterator<Nodo> dijk_iterator() {
		return new DIJKIterator();
	}

  // scheletro di iteratore sui nodi 
	private class DIJKIterator implements Iterator<Nodo> {
		public DIJKIterator() {}
		public boolean hasNext() {return false;}
		public Nodo next() {return null;}

	}


	public static void main(String[] args) {
		// classe di prova per testare il grafo
		class Stringa implements Nodo {
			String nome;
			public Stringa(String nome) {this.nome = nome;}
			public String etichetta() {return nome;}
		}
		
    // costriusco un grafo con 4 nodi e 4 archi
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

