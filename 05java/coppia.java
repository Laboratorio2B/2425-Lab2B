import java.util.ArrayList;

class Coppia {
  int x,y; // variabili d'istanza

  // costruttore
  Coppia(int x, int y) {
    this.x = x;  // usiamo this per distinguere tra variabili d'istanza e parametri
    this.y = y;
  }
  // esempio di secondo costruttore che crea la coppia 0,0
  // è un esempio di overloading (stesso nome, segnatura diversa)
  Coppia() {
    x=0; y=0;  // l'uso di this qui non è necessario
  }

  // crea stringa che rappresenta la coppia
  // viene invocato ad esempio da println
  public String toString() {
    return String.format("(%d,%d)",x,y);
  }

  // overriding del metodo equals di Object per confrontare coppie
  // sulla base dei valori delle componenti
  public boolean equals(Object o) {
    if(o == null || !(o instanceof Coppia))
      return false;
    Coppia c = (Coppia)o;      // casting simile a quello del C
    return x==c.x && y==c.y;
  }

  // metodo main: crea coppie con i dati dalla linea di comando e le ordina
  public static void main(String[] args) {
    // controlla che ci siano almeno due argomenti e che siano pari
    if(args.length < 2 || args.length% 2!=0) {
      System.out.println("Uso: java Coppia x1 y1 x2 y2 ...");
      System.exit(1);
    }

    // crea una lista di coppie di interi passati sulla linea di comando
    ArrayList<Coppia> lista = new ArrayList<Coppia>();
    for(int i=0; i<args.length; i+=2) {
      int x = Integer.parseInt(args[i]);
      int y = Integer.parseInt(args[i+1]);
      lista.add(new Coppia(x,y));
    }
    lista.add(new Coppia()); // aggiunge una coppia 0,0

    // ordina la lista per differenza di elementi crescente
    lista.sort((c1,c2) -> Math.abs(c1.x-c1.y) - Math.abs(c2.x-c2.y));

    // esempio più complesso che ordina per differenza crescente
    // e a parità di differenza, ordina per x decrescente
    // lista.sort((c1,c2) -> {
    //   int diff1 = Math.abs(c1.x-c1.y);
    //   int diff2 = Math.abs(c2.x-c2.y);
    //   if(diff1 != diff2)
    //     return diff1 - diff2;
    //   return c2.x - c1.x;
    // });

    // stampa la lista
    for(Coppia c: lista)
       System.out.println(c);
    System.out.println("---- end ----");
    // test del metodo contains: invoca equals di Coppia
    Coppia c = new Coppia(2, 3);
    System.out.println("c=" + c + " è in lista? " + lista.contains(c));
  }
}