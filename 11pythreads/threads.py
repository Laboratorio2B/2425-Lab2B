#! /usr/bin/env python3

import sys, threading, time


# funzione eseguita dai singoli thread 
def countdown(a,pausa):
  for i in range (a,-1,-1):
    print(f"{threading.current_thread().name}" ,"-->", i)  
    time.sleep(pausa)        # attende pausa secondi


def main(lista):
  # thread creati
  th = []
  # crea e avvia un thread per ogni elemento in lista
  for s in lista:
    x = threading.Thread(target=countdown, args=(int(s), 0.5))
    x.start()
    th.append(x) # salva il thread nella lista
  # attende terminazione dei thread 
  for y in th:
    y.join()
    print(f"Thread  {y.name} terminato")
  return


# invoca il main con i parametri passati sulla linea di comando 
if len(sys.argv)>1:
  main(sys.argv[1:])
else:
  print("Uso:\n\t %s i1 [i2 i3 ...]" % sys.argv[0])



