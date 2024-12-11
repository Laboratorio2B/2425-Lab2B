import java.util.Collections;
import java.util.HashMap;
import java.util.LinkedHashMap;
import java.util.Map;
import java.util.Set;
import java.util.SortedSet;

// Nota: per costruire la documentazione delle classi
// Grafo, Arco, Città etc. utilizzare il comando
//    javadoc -d doc  *.java
// Successivamente aprire il file doc/overview-tree.html


/**
Classe astratta per rappresentare un grafo pesato non orientato
*/
public abstract class Grafo {
	
	/**
	aggiunge un nodo al grafo

	@param n nodo da aggiungere
	@throws IllegalArgumentException se n è già nel grafo
	*/
	abstract public void aggiungi_nodo(Nodo n);

	/**
	aggiunge un arco al grafo

	@param a arco da aggiungere
	@throws IllegalArgumentException se uno degli estremi non è nel grafo
	@throws IllegalArgumentException se un arco con gli stessi estremi è nel grafo
	*/
	abstract public void aggiungi_arco(Arco a);

  /**
	Restituisce il numero totale di nodi nel grafo
	@return numero dei nodi
	*/
	abstract public int num_nodi();

	/**
	Restituisce il numero totale di archi nel grafo
	@return numero degli archi
	*/
	abstract public int num_archi();

	/**
	 * Verifica se il grafo contiene un nodo
	 
	 * @param n nodo da cercare
	 * @return true se il nodo è presente, false altrimenti
	 */
	abstract public boolean contiene_nodo(Nodo n);

	/**
	Restituisce l'insieme degli archi uscenti da un nodo ordinati per peso crescente
	@pa	ram n nodo del grafo
	@return insieme degli archi uscenti da n
	@throws IllegalArgumentException se n non è un nodo del grafo
	*/
  abstract public SortedSet<Arco> uscenti(Nodo n);

	/**
	Restituisce l'insieme di tutti gli archi del grafo ordinati per peso crescente

	@return insieme di tutti gli archi del grafo
	*/
	abstract public SortedSet<Arco> archi();

  /**
	Restituisce i cammini minimi da un nodo sorgente 

	@parameter s sorgente per il calcolo dei cammini minimi
	@return mappa che associa ad ogni nodo raggiungibile da s
	        il costo del cammino minimo e il nodo precedente
	*/
  public Map<Nodo,Cammino> dijkstra(Nodo s) {
				// verifica che la sorgente sia nel grafo!
		assert this.contiene_nodo(s): 
		       "La sorgente deve appartenere al grafo";

    // nodi per i quali ho determinato la distanza minima da s
		Map<Nodo,Cammino> risolti = new LinkedHashMap<Nodo,Cammino>();

		// nodi per quali ho trovato almeno un cammino da s 
		// per ogni nodo mantengo il segmento migliore trovato finora
		Map<Nodo,Cammino> raggiunti = new HashMap<Nodo,Cammino>();

		// inizializzo raggiunti con il nodo sorgente che ha
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
			for(Arco a : uscenti(n)) {
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

}