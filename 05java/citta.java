import java.io.BufferedReader;
import java.io.FileReader;
import java.util.HashSet;
import java.util.Set;

class Citta implements Nodo {
  String nome;
  double lat, lon; // latitudine e longitudine
  int popolazione; // popolazione in migliaia
  
  Citta(String nome, double lat, double lon, int popolazione) {
    this.nome = nome;
    this.lon = lon;
    this.lat = lat;
    this.popolazione = popolazione;
  }

  // converte una stringa in una città se possibile
  // altrimenti lancia un'eccezione
  Citta(String s) {
    String[] campi = s.split(",");
    if(campi.length != 4)
      throw new IncompleteRow("Riga con !=4 campi: " + s);
    nome = campi[0].trim();
    lat = Double.parseDouble(campi[1].trim());
    lon = Double.parseDouble(campi[2].trim());
    popolazione = Integer.parseInt(campi[3].trim());
  }

  @Override
  public String toString() {
    return String.format("%s (%.2f,%.2f) %d ",nome,lat,lon,popolazione);
  }

  @Override
  public boolean equals(Object obj) {
    // verifica se obj==null oppure se obj non è una città
    if(obj == null || getClass() != obj.getClass())
      return false;
    Citta c = (Citta)obj;
    return nome.equals(c.nome) && lat == c.lat && lon == c.lon && popolazione == c.popolazione;
  }

  // override del metodo hashCode per garantire coerenza con equals
  // se due oggetti sono uguali, devono avere lo stesso hashcode
  // necessario altrimenti HashSet non funziona correttamente
  // notare che la qualità del codice hash non è importante
  // per la correttezza del programma, ma influisce sulle prestazioni
  // ce ne accorgiamo quando abbiamo molti oggetti perché
  // un buon hashcode garantisce operazioni in tempo costante
  // uno cattivo può portare a operazioni in tempo lineare
  @Override
  public int hashCode() {
    int hash = 7;
    hash = 31 * hash + nome.hashCode();
    hash = 31 * hash + Double.hashCode(lat);
    hash = 31 * hash + Double.hashCode(lon);
    hash = 31 * hash + Integer.hashCode(popolazione);
    return hash;
  }

  // funzioni che servono per usare le città come nodo di un grafo
  // implementando l'interfaccia Nodo
  public int compareTo(Citta c) {
    return nome.compareTo(c.nome);}

  public String etichetta() {return  nome;}


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
        // salta linee vuote o commenti
        if(linea.length() == 0 || linea.charAt(0) == '#')
          continue;
        Citta c = new Citta(linea);
        if(insieme.add(c)!=true)
          System.out.println("Città duplicata: " + c);
      }
      br.close(); // chiudo il file
    }
    // esempio di gestione di una eccezione separatamente
    // dal caso generale: in questo caso invece di terminare
    // il programma, stampo un messaggio e elaboro le città lette
    catch (IncompleteRow e) {
      System.err.println(e + "\nInterrompo la lettura ");
    }
    catch(Exception e) {
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
