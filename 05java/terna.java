import java.io.BufferedReader;
import java.io.FileReader;
import java.util.Set;
import java.util.TreeSet;

class Terna implements Comparable<Terna> {
  int x,y,z; // variabili d'istanza

  // costruttore
  Terna(int x, int y, int z) {
    this.x = x;  // usiamo this per distinguere tra variabili d'istanza e parametri
    this.y = y;
    this.z = z;
  }

  // crea stringa che rappresenta la terna
  @Override
  public String toString() {
    return String.format("%d %d %d",x,y,z);
  }

  @Override
  public boolean equals(Object o) {
    if(o == null || !(o instanceof Terna))
      return false;
    Terna t = (Terna)o;      // casting simile a quello del C
    return x==t.x && y==t.y && z==t.z;
  }

  // metodo compareTo richiesto dall'interfaccia Comparable
  // a sua volta richiesta da TreeSet
  // confronta due terne considerando le componenti z,y,x in questo ordine
  public int compareTo(Terna t) {
    if(z != t.z)
      return z - t.z;
    if(y != t.y)
      return y - t.y;
    return x - t.x;
  }

  // somma delle componenti
  public int somma() {
    return x+y+z;
  }
  // terna pitagorica?
  public boolean pitagorica() {
    return x*x + y*y == z*z;
  }


  // legge una terna da una stringa di :;- come specificato
  // nel testo del compito. Sarebbe più semplice usare il metodo split
  // della classe String, ma ho voluto mostrare una soluzione
  // basata su semplici cicli for/while
  static Terna leggiTerna(String s) {
      // se ci sono caratteri estranei passa alla linea successiva
      boolean ok = true;
      for(int i=0; i<s.length(); i++) {
        char c = s.charAt(i);
        if(c != '-' && c != ':' && c != ';') {ok = false; break;}
      }
      if(!ok) return null;
      s += "\n";  // mettiamo lo \n in fondo come in C
      // spezza la linea in sequenze di --- secondo i caratteri : e ;
      int i=0,j,x,y,z;
      // salta separatori iniziali
      while(s.charAt(i) == ';' || s.charAt(i) == ':') i++;
      if(s.charAt(i) == '\n') return null; // linea senza ----
      j = i;
      while(s.charAt(j) == '-') j++;
      if(s.charAt(j) == '\n') return null; // linea con solo 1 sequenza di -
      x = j-i; i=j+1;
      while(s.charAt(i) == ';' || s.charAt(i) == ':') i++;
      if(s.charAt(i) == '\n') return null; // linea senza con solo 1 sequenza di -
      j = i;
      while(s.charAt(j) == '-') j++;
      if(s.charAt(j) == '\n') return null; // linea con solo 2 sequenze di -
      y = j-i; i=j+1;
      while(s.charAt(i) == ';' || s.charAt(i) == ':') i++;
      if(s.charAt(i) == '\n') return null; // linea senza con solo 2 sequenze di -
      j = i;
      while(s.charAt(j) == '-') j++;
      z = j-i; i=j;
      // ora devo verificare che non ci siano altri - prima dello \n
      ok = true;
      while(s.charAt(i) != '\n') {
        if(s.charAt(i) == '-') {ok = false; break;}
        i++;
      }
      if(!ok) return null;
      return new Terna(x,y,z);
  }

  public static void main(String[] args) {
    // controlla che ci siano un argomento (il nome del file)
    if(args.length <1) {
      System.out.println("Uso: java Terna nomefile i1 i2 ...");
      System.exit(1);
    }

    // crea una lista di terne di interi leggendo da argv[0]
    Set<Terna> insieme = new TreeSet<Terna>();

    // apre il file e legge una linea alla volta
    try {
      BufferedReader in = new BufferedReader(new FileReader(args[0]));
      while(in.ready()) {
        String line = in.readLine();
        Terna t = leggiTerna(line);
        if(t != null)
          insieme.add(t);
      }
      in.close();
    } catch(Exception e) {
      System.err.println("Errore: " + e);
      // stampo la traccia dello stack per debug
      e.printStackTrace();
      System.exit(2);
    }
    // stampa numero totale terne
    System.out.println(insieme.size());
    // conta le terne con somma assegnata
    for(int i=1; i<args.length; i++) {
      int s = Integer.parseInt(args[i]);
      int count = 0;
      for(Terna t: insieme)
        if(t.somma() == s)
          count++;
      System.out.println(count);
    }

    // stampa la lista
    for(Terna t: insieme)
      if (t.pitagorica())
        System.out.println(t);
    System.out.println("---- end ----");
  }  
}
