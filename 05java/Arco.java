
/**
 Classe che rappresenta un arco pesato di un grafo
*/ 
public class Arco implements Comparable<Arco> {
	Nodo n1, n2;
	double weight;

	public Arco(Nodo n1,Nodo n2, double w){
		this.n1=n1;
		this.n2=n2;
		this.weight=w;
	}
	
	public boolean stessi_estremi(Arco a) {
		if(n1.equals(a.n1) && n2.equals(a.n2))
		  return true;
		if(n1.equals(a.n2) && n2.equals(a.n1))
		  return true;
		return false;
	}

	public Nodo altro_estremo(Nodo n) {
		if(n.equals(n1)) return n2;
		if(n.equals(n2)) return n1;
		throw new IllegalArgumentException("nodo non estremo dell'arco");
	}

	/** Confronta due archi sulla base del peso. Il confronto è poi esteso ai nodi 
	 * utilizzando l'ordinamento naturale delle etichette dei nodi.
	 * La soluzione non è completamente pulita: sarebbe preferibile 
	 * richiedere che i nodi implementino l'interfaccia Comparable<Nodo>
	 * e usare il metodo compareTo di Nodo, ma questo richiederebbe di
	 * utilizzare i tipi generici in maniera non banale.
	 * */
	public int compareTo(Arco a) {
		if(weight < a.weight) return -1;
		if (weight > a.weight) return 1;
		// i pesi sono uguali confrontiamo gli estremi
		if(n1.etichetta().compareTo(a.n1.etichetta())!=0)
		  return n1.etichetta().compareTo(a.n1.etichetta());
		return n2.etichetta().compareTo(a.n2.etichetta());
	}

	public boolean equals(Object o) {
		if(o instanceof Arco) {
			Arco a = (Arco) o;
			return n1.equals(a.n1) && n2.equals(a.n2) && weight==a.weight;
		}
		return false;
	}

	public String toString() {
		return String.format("%s <= %6.2f => %s",n1.etichetta(),weight, n2.etichetta());
	}

}