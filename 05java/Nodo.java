

/**
 * Interfaccia per rappresentare un nodo di un grafo
 * l'unica richiesta per il tipo nodo (attualmente)
 * è che ogni nodo abbia un'etichetta che viene
 * usata per identificarlo in modo univoco
 * 
 * Qualsiasi classe che ha il metodo etichetta() può essere un nodo
 * di un grafo, semplicemente implementando questa interfaccia
 */

public interface Nodo {

	/** restituisce l'etichetta del nodo
	    nodi diversi devono avere etichette diverse */
  public String etichetta();

}