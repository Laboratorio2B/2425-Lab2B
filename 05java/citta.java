import java.io.BufferedReader;
import java.io.FileReader;
import java.util.HashSet;
import java.util.Set;

class Citta {
  String nome;
  double lat, lon; // latitudine e longitudine
  int popolazione; // popolazione in migliaia
  
  Citta(String nome, double lat, double lon, int popolazione) {
    this.nome = nome;
    this.lon = lon;
    this.lat = lat;
    this.popolazione = popolazione;
  }

  public String toString() {
    return String.format("%s (%.2f,%.2f) %d ",nome,lat,lon,popolazione);
  }

  @Override
  public boolean equals(Object obj) {
    if(obj == null || getClass() != obj.getClass())
      return false;
    Citta c = (Citta)obj;
    return nome.equals(c.nome) && lat == c.lat && lon == c.lon && popolazione == c.popolazione;
  }

  // @Override
  public int hashCode() {
    int hash = 7;
    hash = 31 * hash + nome.hashCode();
    hash = 31 * hash + Double.hashCode(lat);
    hash = 31 * hash + Double.hashCode(lon);
    hash = 31 * hash + Integer.hashCode(popolazione);
    return hash;
  }

  // converte una stringa in una città se possibile
  // altrimenti restituisce null
  // se il primo carattere è # la stringa è un commento e ritorna null
  static Citta leggiCitta(String s) {
    if(s.charAt(0)=='#') 
      return null;
    String[] campi = s.split(",");
    //if(campi.length != 4)
    //  throw new IncompleteRow("Riga incompleta: " + s);
    return new Citta(campi[0].trim(),Double.parseDouble(campi[1].trim()),
                     Double.parseDouble(campi[2].trim()),
                     Integer.parseInt(campi[3].trim()));
  }

 

  public static void main(String[] args) {
    if(args.length !=1) {
      System.out.println("Uso: java Citta nomefile");
      System.exit(1);
    }
    // creazione insieme di città 
    Set<Citta> insieme = new HashSet<Citta>();
    try {
      BufferedReader br = new BufferedReader(new FileReader(args[0]));
      String linea;
      while((linea = br.readLine()) != null) {
        Citta c = leggiCitta(linea);
        if(c != null)
          if(insieme.add(c)!=true)
            System.out.println("Città duplicata: " + c);
      }
      br.close();
    } catch(Exception e) {
      System.err.println("Errore: " + e);
      e.printStackTrace();
      System.exit(2);
    }
    // stampa elenco città
    for(Citta c: insieme)
      System.out.println(c);
    System.out.println("---- end ----");
  }
}


// creo una eccezione per segnalare righe incomplete
// eredita da RuntimeException affinché sia un unchecked exception
class IncompleteRow extends RuntimeException {
  IncompleteRow(String e) {super(e);}
}
