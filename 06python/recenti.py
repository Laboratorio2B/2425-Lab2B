#!/usr/bin/env python3
"""
Esempio di script per la navigazione del filesystem

Visualizza l'elenco dei file più recenti di un dato numero di 
giorni ordinati per dimensione decrescente
Prende come parametro opzionale una espressione
regolare, e in tal caso considera solo i file
il cui nome la soddisfano 


Comandi per la gestione dei tempi

time.localtime()  # converte secondi da epoch in un oggetto struct_time 
time.asctime()    # converte un oggetto struct_time in ascii
time.mktime()     # inverso di time.localtime() struct_time -> secondi da epoch


Comandi per la gestione di file e directory

os.getcwd()       # restituisce directory corrente
os.chdir(path)    # cambia directory
os.listdir(path)  # elenca file (restituisce lista di stringhe)
os.access(path)   # verifica i permessi 
os.path.getsize(path)  # dimensione file
os.path.exists(path)   # vero se il file/directory esiste  
os.path.isfile(path)   # vero se regular file
os.path.isdir(path)    # vero se directory
os.path.islink(path)   # vero se symbolic link
os.path.join(nome_dir,nome_file) # combina nome dir e file
os.path.abspath(path)  # restituisce path assoluto
os.path.realpath(path) # restituisce nome canonico eliminando link simbolici
os.path.getmtime(path) # istante ultima modifica in secs da epoch

Lista completa dei comandi su:
  https://docs.python.org/3/library/time.html
  https://docs.python.org/3/library/os.html
  https://docs.python.org/3/library/os.path.html
  
Introduzione alle regular expression:
  https://realpython.com/regex-python/  
"""
import os, os.path, sys, time, re

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
    return f"{self.path}\n size:{self.size}  modificato:{t}\n"

  def __repr__(self):
    return f"Miofile({self.path})"


def main(nomedir,g,regex=None):
  """Lancia ricerca ricorsiva su nomedir"""
  if not os.path.exists(nomedir):
    print("Il nome che mi hai passato non esiste")
    sys.exit(1)
  if not os.path.isdir(nomedir):
    print("Il nome che mi hai passato esiste, ma non è una directory")
    sys.exit(1)
  nomeabs = os.path.abspath(nomedir)
  # pone il limite dei file in elenco a g giorni prima di ora
  limite  = time.mktime(time.localtime()) - g*24*3600
  elenco = cerca_recenti(nomeabs,limite,set(),regex)
  print("------------------------") 
  print(f"Ci sono {len(elenco)} file più recenti di {g} giorni");
  elenco.sort(reverse=True) 
  # stampa i file utilizzando il metodo __str__ di Miofile
  for f in elenco:
    print(f)

  

# funzione ricorsiva per cercare i file modificati dopo limite
def cerca_recenti(nome,datalimite,giaesplorati,regex):
  """restituisce la lista dei file modificati dopo datalimite
 tra quelli nella directory nome e sue sottodirectory"""
  
  assert os.path.isdir(nome), "Argomento deve essere una directory"
  print(f"Begin: {nome}",file=sys.stderr)
  # inizializzo la lista di output inizialmente vuota
  recenti = []
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
      miof = Miofile(nomecompleto)
      # aggiungo miof alla lista solo se
      # è stato modificato dopo datalimite
      if not miof.precedente_a(datalimite):
        if regex:
          # se è stata data una espressione regolare
          # aggiunge il file a elenco solo
          # se nomecompleta ha un match con regex
          if re.search(regex,nomecompleto):
            recenti.append(miof)
        else:
          # se non è stata passata una espressione
          # regolare il file viene aggiunto a elenco
          recenti.append(miof)
    else:
      # nomecompleto è una directory: possibile chiamata ricorsiva
      if not os.access(nomecompleto, os.R_OK | os.X_OK):
        print(f"!! Directory {nomecompleto} non accessibile",file=sys.stderr)
        continue
      nomereal = os.path.realpath(nomecompleto)
      if nomereal in giaesplorati:
        print(f"!! Directory {nomereal} già esplorata",file=sys.stderr)
        continue
      giaesplorati.add(nomereal)
      # directory nuova e accessibile: esegui ricorsione
      recenti_subdir = cerca_recenti(nomecompleto,datalimite, giaesplorati,regex)
      # concateno al risultato
      recenti += recenti_subdir
  # ciclo for su i file di questa directory terminato     
  print(f"End: {nome}",file=sys.stderr)
  return recenti

# invoca main passando il nome di una directory e un numero di giorni
# (anche frazionario)
# se viene passata anche un espressione regolare
# considera solamente i file il cui nome 
# soddisfa tale espressione
if len(sys.argv) == 3:
    main(sys.argv[1], float(sys.argv[2]))
    sys.exit(0)
elif len(sys.argv) == 4:
    main(sys.argv[1], float(sys.argv[2]), sys.argv[3])
    sys.exit(0)    
else:
    print("Uso:", sys.argv[0], "nome_directory numero_giorni [regex]")
    sys.exit(2)
