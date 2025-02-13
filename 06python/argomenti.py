#!/usr/bin/env python3
"""
Esempi che mostrano l'uso di argomenti posizionali e keyword

Vedere la documentazione delle principali funzioni
https://docs.python.org/3/library/functions.html
"""


# esempio di funzione che esegue una somma o un prodotto
def somma(a,b):
  return a+b
    
# generalizzazione che accetta un numero
# qualsiasi di argomenti posizionali
def multi_somma(a,*altri):
  print("altri:", altri)
  for b in altri:
    a += b
  return a
  
# questa è problematica perchè può essere chiamata
# senza argomenti  
def multi_sommaNO(*tutti):
  a = tutti[0]
  for b in tutti[1:]:
    a += b
  return a

# funzione con anche due keyword argument
# per ogni keyword devo specificare il valore di default 
def xsomma(a,b,segno = 1, potenza = 0):
  return segno*(a+b)*10**potenza
  
# metodi validi per chiamare la funzione
# xsomma(2,3) -> 5      (usa i valori di default)
# xsomma(2,3,-1) -> -5  (usa il valore di default per potenza)
# xsomma(2,3,-1,2) -> -500 (usa i valori passati)
# xsomma(2,3,segno=-1, potenza=2) -> -500  (come sopra)
# xsomma(2,3,potenza=2) -> 500   (usa il valore di default per segno)

# funzione che accetta un numero arbitrario di argomenti
# posizionali, ora quelli keyword devono essere
# forniti con la keyword  
def multi_xsomma(a,*altri,segno = 1, potenza = 0):
  for b in altri:
    a += b
  return segno*a*10**potenza

# accetta un numero qualsiasi di argomenti keywords
def multi_xsomma2(a,*altri,segno = 1, **kwargs):
  print("diz:", kwargs)
  for b in altri:
    a += b
  a = segno*a
  if "potenza" in kwargs:
    a*= 10**kwargs["potenza"]
  if "soglia" in kwargs:
    if a> kwargs["soglia"]:
      a = kwargs["soglia"]
  return a
  
# per finire: uso di * e ** per scompattare liste e dizionari
lista = [2,3,5,7]
diz = {"segno":-1, "potenza":3}
# esempio: 
#   multi_xsomma(*lista,**diz) 


