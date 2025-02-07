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
import os, os.path, sys


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
  elenco = elenco_file(nomedir,set())
  # ordino l'elenco dei file per dimensione decrescente
  elenco.sort(key=lambda f: (f[1], f[0]),reverse=True )
  # stampo intestazione elenco
  print(f"{'-- dim --':>10}  {'-- nome '+30*'-'}")
  # stampo elenco
  for f in elenco:
    print(f"{f[1]:>10}  {f[0]}")
  return
    

# funzione ricorsiva per cercare il file più grande
# nella directory corrente e in tutte le sue sottodirectory
def elenco_file(nome,dir_esplorate):
  """restituisce una coppia (nome,file) per ogni 
     file dentro la directory nome e le sue
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
      # il file è rappresentato dalla tupla
      #  (nome,dimensione)
      lista.append((nuovonome,nuovadim))
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

