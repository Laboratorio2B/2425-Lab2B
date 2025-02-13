#!/usr/bin/env python3
"""
Esempio di script per la navigazione del filesystem
Legge sulla riga di comando il nome di una directory
e restituisce il file più grande in essa contenuto
incluse tutte le sottodirectory

Funzioni di libreria per la gestione di file e directory

os.getcwd()       # restituisce directory corrente
os.chdir(path)    # cambia directory
os.listdir(path)  # elenca file (restituisce lista di stringhe)
os.access(path)   # verifica i permessi 
 
os.path.getsize(path) # dimensione file
os.path.exists(path)  # vero se il file/directory esiste  
os.path.isfile(path)  # vero se regular file
os.path.isdir(path)   # vero se directory
os.path.islink(path)  # vero se symbolic link
os.path.join(nome_dir,nome_file) # combina nome dir e file
os.path.abspath(path)  # restituisce path assoluto
os.path.realpath(path) # restituisce nome canonico eliminando link simbolici

Lista completa dei comandi su:
  https://docs.python.org/3/library/os.html
  https://docs.python.org/3/library/os.path.html

Per informazioni sui permessi dei file o come cambiarli
  man chmod

Per informazioni sui link simbolici e il loro uso:
   https://linuxize.com/post/how-to-create-symbolic-links-in-linux-using-the-ln-command/
"""
import os, os.path, sys, subprocess, time

# classe per memorizzare le informazioni di un file
class Miofile:
  def __init__(self,path):
    self.path = path
    self.mtime = os.path.getmtime(path)
    self.size = os.path.getsize(path)
  
  def precedente_a(self,limite):
    """Restituisce true se il tempo di modifica
    è precedente a limite espresso in secondi da Epoch"""
    return self.mtime < limite
    
  def __lt__(self,other):
    "confronta dimensioni e a parità di dimensione il nome"
    if self.size < other.size:
      return True
    if self.size > other.size:
      return False
    return self.path < other.path
    
  def __eq__(self,other):
    return self.path==other.path and self.size==other.size and self.mtime==other.mtime 

  def __str__(self):
    t = time.asctime(time.localtime(self.mtime))
    return f"{self.path}\n size:{self.size}  modificato:{t}"

  def __repr__(self):
    return f"Miofile({self.path})"


def main(nomedir):
  """Lancia ricerca ricorsiva su nomedir"""
  if not os.path.exists(nomedir):
    print("Il nome che mi hai passato non esiste")
    sys.exit(1)
  if not os.path.isdir(nomedir):
    print("Il nome che mi hai passato esiste, ma non è una directory")
    sys.exit(1)
  # invocazione funzione ricorsiva per creare l'elenco  
  nomeabs = os.path.abspath(nomedir)
  elenco = elenco_file(nomeabs,set())
  for f in elenco:
    print(f)
  # trasformo l'elenco dei file in un dizionario
  #   lunghezza -> [lista file con quella lunghezza]
  lunghezze = {}
  for f in elenco:
    # controllo se esiste già la lunghezza f.size nel dizionario
    if f.size in lunghezze:
      # aggiungo f alla lista dei file di lunghezza f.size
      lunghezze[f.size].append(f)
    else:
      lunghezze[f.size] = [ f ]
  print("------ contenuto del dizionario")
  for lun in lunghezze:
    print("--- lunghezza:", lun, " ---") 
    for f in lunghezze[lun]:
      print(f)
  ### DA COMPLETARE
  # togliere return e modificare visualizza_duplicati
  # in maniera che il risultato sia corretto    
  return
  # considero tutte le liste che contegono +
  # di un file e cerco i duplicati
  for lun in lunghezze:
    if len(lunghezze[lun])>1:
      print("=== file di lunghezza", lun)
      visualizza_duplicati(lunghezze[lun])
  return
    
# da modificare perché ora riceve una lista
# di oggetti Miofile
def visualizza_duplicati(lista_file):
  """Data una lista di oggetti Miofile, restituisce 
  quelli che hanno lo stesso contenuto"""
  for i in range(len(lista_file)):
    for j in range(i+1,len(lista_file)):
      # confronta i due file
      file1 = lista_file[i]
      file2 = lista_file[j]
      if cmpshell(file1,file2)==0:
        print(file1)
        print("  identico a")
        print(file2)
        print(30*"-")
        break;
  return
        
def cmpshell(a,b):
  """Confronta i due file usando il comando cmp 
  della shell restituisce l'exit value di cmp"""
  ris = subprocess.run(["cmp",a,b],capture_output=True,
                                   encoding='utf-8')
  return ris.returncode


# funzione ricorsiva per cercare il file più grande
# nella directory corrente e in tutte le sue sottodirectory
def elenco_file(nome,dir_esplorate):
  """restituisce una lista con un oggetto Miofile per 
     ogni file dentro la directory nome e le sue
     sottodirectory"""

  assert os.path.isdir(nome), "Argomento deve essere una directory"
  print(f"Begin: {nome}",file=sys.stderr)
  # inizializzo la lsita di file trovati
  lista = []
  # ottiene il contenuto della directory 
  listafile = os.listdir(nome)
  for f in listafile:
    nomecompleto = os.path.join(nome,f)
    # verifica se il file è accessibile
    if not os.access(nomecompleto,os.F_OK):
      print("!! Broken link:", nomecompleto, file=sys.stderr)
      continue
    # distinguo tra file normali e directory
    if not os.path.isdir(nomecompleto):
      nuovadim = os.path.getsize(nomecompleto)
      nuovonome = nomecompleto
      # aggiungo alla lista risultato
      # il file appena incontrato
      # il file è rappresentato dalla t
      # classe Miofile
      lista.append(Miofile(nuovonome))
    else:
      # nomecompleto è una directory: possibile chiamata ricorsiva
      # verifico che la directory sia esplorabile 
      if not os.access(nomecompleto, os.R_OK | os.X_OK):
        print(f"!! Directory {nomecompleto} non accessibile",file=sys.stderr)
        continue
      # verifica che la directory non sia già stata esplorata
      # va fatta con il realpath perchè la stessa dir può avere più nomi  
      nomereal = os.path.realpath(nomecompleto)
      if nomereal in dir_esplorate:
        print(f"!! Directory {nomecompleto} già esplorata",file=sys.stderr)
        print(f"!!  con nome {nomereal}",file=sys.stderr)
        continue
      dir_esplorate.add(nomereal)
      # directory nuova e accessibile: esegui ricorsione
      lista_dir = elenco_file(nomecompleto,dir_esplorate)
      lista += lista_dir
  # fine ciclo for su i file di questa directory     
  print(f"End: {nome}",file=sys.stderr)
  return lista
  
  
# verifico argomenti sulla linea di comando
# e faccio partire la ricerca
if len(sys.argv) == 2:
    main(sys.argv[1])
else:
    print("Uso:", sys.argv[0], "nome_directory")

