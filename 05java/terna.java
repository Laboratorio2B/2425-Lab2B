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
  public String toString() {
    return String.format("%d %d %d",x,y,z);
  }

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


  public static void main0(String[] args) {
    // controlla che ci siano almeno tre argomenti e che siano multipli di tre
    if(args.length < 3 || args.length% 3!=0) {
      System.out.println("Uso: java Terna x1 y1 z1 x2 y2 z2 ...");
      System.exit(1);
    }

    // crea una lista di terne di interi dalla linea di comando
    Set<Terna> insieme = new TreeSet<Terna>();
    for(int i=0; i<args.length; i+=3) {
      int x = Integer.parseInt(args[i]);
      int y = Integer.parseInt(args[i+1]);
      int z = Integer.parseInt(args[i+2]);
      insieme.add(new Terna(x,y,z));
    }

    // stampa la lista
    for(Terna t: insieme)
       System.out.println(t);
    System.out.println("---- end ----");
  }


  static Terna leggiTerna(String s) {
      // se ci sono caratteri estranei passa alla linea successiva
      boolean ok = true;
      for(int i=0; i<s.length(); i++) {
        char c = s.charAt(i);
        if(c != '-' && c != ':' && c != ';') {ok = false; break;}
      }
      if(!ok) return null;
      s += "\n";  // mettiamo lo \n iun fondo come in C
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
        // fine del parsing
        Terna t = leggiTerna(line);
        if(t != null)
          insieme.add(t);
      }
      in.close();
    } catch(Exception e) {
      System.err.println("Errore: " + e);
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
