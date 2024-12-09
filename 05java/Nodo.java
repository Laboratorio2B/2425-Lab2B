
// l'unica richiesta per il tipo nodo (al momento) 
// è che ogni nodo abbia una etichetta che viene
// usata per identificarlo in modo univoco 
// l'etichetta è usata anche per stampare il nodo
public interface Nodo {

	// restituisce l'etichetta del nodo
	// nodi diversi devono avere etichette diverse
  public String etichetta();

}