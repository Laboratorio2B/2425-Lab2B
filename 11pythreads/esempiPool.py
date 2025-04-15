#! /usr/bin/env python3

# esempi sui pool di thread

import sys, threading, logging, time, os
import concurrent.futures



# classe usata per rappresentare la somma e il suo mutex
class Somma:
  def __init__(self):
    self.somma = 0
    self.lock = threading.Lock() # questo è l'analogo di un mutex del C



# sleep per un numero assegnato di secondi
def tipo1(s):
  print(f"Tipo 1: dormo per {s} secondi");
  time.sleep(s)
  print(f"Fine tipo 1: {s} secondi")
  return s

def tipo2(s):
  print(f"Tipo 2: dormo per {s} secondi");
  time.sleep(s)
  print(f"Fine tipo 2: {s} secondi")
  return s




def main(p):
  with concurrent.futures.ThreadPoolExecutor(max_workers=p) as executor:
    executor.map(tipo1,range(1,7))
    executor.submit(tipo1,3.1415)
    print("Esecuzioni tipo 1 inviate")
    # with concurrent.futures.ThreadPoolExecutor(max_workers=p) as executor2:
    executor.map(tipo2,range(2,8))
    print("Esecuzioni tipo 2 inviate")
  


# invoca il main con il parametro passato sulla linea di comando 
if len(sys.argv)==2:
  main(int(sys.argv[1]))
else:
  print("Uso:\n\t %s numthread" % sys.argv[0])



