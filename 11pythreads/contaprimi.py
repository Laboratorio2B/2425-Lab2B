#! /usr/bin/env python3
import sys, threading, time, os, argparse, logging
import concurrent.futures

# descrizione usata dal modulo argparse
Description = """Esempi elementari di uso di thread in python:
main() calcola i primi in un intervallo usando due thread 
       "semplici",
main_pool() usa un pool di thread e il metodo executor.map, 
main_submit() usa un pool di 2 thread e il metodo executor.submit 
"""


# configurazione del logging
# il logger scrive su un file con nome uguale al nome del file eseguibile
# ma estensione .log
logging.basicConfig(filename=os.path.basename(sys.argv[0])[:-3] + '.log',
                    level=logging.INFO, datefmt='%d/%m/%y %H:%M:%S',
                    format='%(asctime)s - %(levelname)s - %(message)s')


# classe usata per passare i dati ai thread e ricevere il risultato
class Dati:
  def __init__(self,a,b):
    self.a = a           # input del htread 
    self.b = b           # input del thread 
    self.risultato = -1  # output del thread


# corpo del thread prende un oggetto Dati come argomento
def tbody(dati):
  logging.debug(f"Inizia esecuzione del thread che parte da {dati.a} e arriva a {dati.b}")
  dati.risultato = conta_primi(dati.a, dati.b)
  logging.debug(f"Termina esecuzione del thread che parte da {dati.a} e arriva a {dati.b}")
  return dati.risultato


# crea due thread in maniera C-like
def main(a,b):
  logging.info("Inizia esecuzione del main")
  # crea 2 thread passando ad ognuno i suoi dati
  c = (a+b)//2
  d1 = Dati(a,c)
  d2 = Dati(c,b)
  t1 = threading.Thread(target=tbody, args=(d1,))
  t2 = threading.Thread(target=tbody, args=(d2,))
  # avvia i thread misurando il tempo di esecuzione
  start = time.time()
  t1.start()
  t2.start()
  t1.join()
  t2.join()
  end = time.time()
  print(f"Tra {a} e {b} ci sono {d1.risultato+d2.risultato} primi e ci ho messo {end-start:.2f} secondi")
  logging.info(f"Termina esecuzione del main, secondi={end-start}")


# usa un pool di thread ed executor.map
def main_pool(a,b,p):
  logging.info("Inizia esecuzione di main_pool")
  assert p>0, "Il numero di thread deve essere maggiore di 0"
  # crea l'intervallo per ognuno dei p thread
  dati = []
  for i in range(p):
    dati.append(Dati(a+(b-a)*i//p, a+(b-a)*(i+1)//p-1))
  # avvia i thread misurando il tempo di esecuzione 
  start = time.time() 
  # se nella riga seguente uso ProcessPoolExecutor invece di ThreadPoolExecutor
  # vengono lanciati processi invece che thread
  with concurrent.futures.ThreadPoolExecutor(max_workers=p) as executor:
    # il return value di ogni singola chiamata a tbody viene messo in risultati
    risultati = executor.map(tbody, dati)
    print("executor è terminato")
  # il calcolo del tempo di esecuzione e' da fare fuori dal contesto del with
  # perché executor.map() termina prima che abbiano terminato tutti i thread
  # al termine della with viene fatto il join di tutti i thread 
  end = time.time()
  tot = sum(risultati)
  print(f"Tra {a} e {b} ci sono {tot} primi e ci ho messo {end-start:.2f} secondi")
  logging.info("Termina esecuzione di main_pool")
  return


# usa un pool di thread ed executor.submit
def main_submit(a,b):
  logging.info("Inizia esecuzione del main_submit")
  # crea 2 thread passando ad ognuno i suoi dati
  c = (a+b)//2
  d1 = Dati(a,c)
  d2 = Dati(c,b)
  start = time.time()
  with concurrent.futures.ThreadPoolExecutor() as executor:
    print(f"Posso usare {executor._max_workers} workers");
    r1 = executor.submit(tbody, d1)
    r2 = executor.submit(tbody, d2)
    print("r1 running?", r1.running())
    print("r2 done?", r2.done())
  # anche qui all'uscita del with viene fatta la join
  end = time.time()
  print(f"Tra {a} e {b} ci sono {r1.result()+r2.result()} primi e ci ho messo {end-start:.2f} secondi");
  print("r1 running?", r1.running())
  print("r2 done?", r2.done())
  logging.info("Termina esecuzione del main_submit")
     

# ----------- metodi di servizio -------------    

# conta i primi in [a,b]
def conta_primi(a,b):
  tot = 0
  for i in range(a,b+1):
    if primo(i):
      tot += 1
  return tot

# dato un intero n>0 restituisce True se n e' primo
# False altrimenti
def primo(n):
    assert n>0, "L'input deve essere positivo"
    if n==1:
        return False
    if n==2:
        return True
    if n%2 == 0:
        return False
    assert n>=3 and n%2==1, "C'e' qualcosa che non funziona"
    for i in range(3,n//2,2):
        if n%i==0:
            return False
        if i*i > n:
            break    
    return True



# questo codice viene eseguito solo se il file è eseguito direttamente
# e non importato come modulo con import da un altro file
if __name__ == '__main__':
  # parsing della linea di comando vedere la guida
  #    https://docs.python.org/3/howto/argparse.html
  parser = argparse.ArgumentParser(description=Description, formatter_class=argparse.RawTextHelpFormatter)
  parser.add_argument('min', help='minimo', type = int)  
  parser.add_argument('max', help='massimo', type = int)   
  parser.add_argument('-p', help='Usa un pool di P thread', type = int, default=-1) 
  parser.add_argument('-s', help='Usa un pool di due thread con submit', action="store_true") 
  args = parser.parse_args()
  if args.s:
    main_submit(args.min,args.max)
  elif args.p <0:
    main(args.min,args.max)
  else:
    main_pool(args.min,args.max,args.p)



