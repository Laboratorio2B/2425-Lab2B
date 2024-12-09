import java.util.Map;
import java.util.SortedSet;


/**
Interfaccia per rappresentare un grafo pesato non orientato
*/
public interface Grafo {
	
	/**
	aggiunge un nodo al grafo

	@param n nodo da aggiungere
	@throws IllegalArgumentException se n è già nel grafo
	*/
	public void aggiungi_nodo(Nodo n);

	/**
	aggiunge un arco al grafo

	@param a arco da aggiungere
	@throws IllegalArgumentException se uno degli estremi non è nel grafo
	@throws IllegalArgumentException se un arco con gli stessi estremi è nel grafo
	*/
	public void aggiungi_arco(Arco a);

  /**
	Restituisce il numero totale di nodi nel grafo
	@return numero dei nodi
	*/
	public int num_nodi();

	/**
	Restituisce il numero totale di archi nel grafo
	@return numero degli archi
	*/
	public int num_archi();

	/**
	 * Verifica se il grafo contiene un nodo
	 
	 * @param n nodo da cercare
	 * @return true se il nodo è presente, false altrimenti
	 */
	public boolean contiene_nodo(Nodo n);

	/**
	Restituisce l'insieme degli archi uscenti da un nodo ordinati per peso crescente
	@param n nodo del grafo
	@return insieme degli archi uscenti da n
	@throws IllegalArgumentException se n non è un nodo del grafo
	*/
  public SortedSet<Arco> uscenti(Nodo n);

	/**
	Restituisce l'insieme di tutti gli archi del grafo ordinati per peso crescente

	@return insieme di tutti gli archi del grafo
	*/
	public SortedSet<Arco> archi();

  /**
	Restituisce i cammini minimi da un nodo sorgente 

	@parameter s sorgente per il calcolo dei cammini minimi
	@return mappa che associa ad ogni nodo raggiungibile da s
	        il costo del cammino minimo
	*/
  public Map<Nodo,Cammino> dijkstra(Nodo s);

}