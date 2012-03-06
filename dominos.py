#!/usr/bin/python
import random


def creation_jeu(max=6):
  """Cree la boite de jeu avec l'ensemble des dominos
  qui seront distribues
  """
  jeu=[]
  for i in range(max,-1,-1):
    for j in range(i,-1,-1):
      jeu = jeu + [[i,j]]
  return jeu

def distribue(jeu,n=10):
  """Tire dans jeu (sans remise) une main de n dominos"""
  main = []
  for i in range(n):
    r = random.randint(0,len(jeu)-1)
    main = main + [ jeu[r] ]
    jeu.pop(r)
  return main

def est_avant(d,e):
  """Verifie si deux dominos sont dans l'ordre lexicographique
  - Exemples :
  >>> est_avant( [2,3], [1,2] ); est_avant( [2,3], [2,4] )
  True
  False
  """
  if d[1]>e[1] or ( d[1]==e[1] and d[2]>e[2] ):
    return True
  return False

if __name__ == "__main__":
  import doctest
  doctest.testmod()
  
  jeu = creation_jeu()
  joueur_A = distribue(jeu)
  joueur_B = distribue(jeu)
  print "Joueur A : ",joueur_A
  print "Joueur B : ",joueur_B
  print "Talon : ",jeu
  print est_avant([2,3],[1,2])
