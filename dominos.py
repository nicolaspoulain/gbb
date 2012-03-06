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
  if d[0]>e[0] or ( d[0]==e[0] and d[1]>e[1] ):
    return True
  return False

def tri_decr(m):
  """Trie les dominos de la main m dans l'odre
  decroissant lexocographique
  - Exemple :
  >>> tri_decr([ [1,2], [3,4], [1,3], [2,0] ])
  [[3, 4], [2, 0], [1, 3], [1, 2]]
  """
  m = sorted(m)
  m.reverse()
  return m

def is_player1_first(pl1,pl2):
  """Le joueur A a-t-il une meilleure main que le joueur B ?"""
  if est_avant(pl1[0],pl2[0]):
   return True
  return False


if __name__ == "__main__":
  import doctest
  doctest.testmod()
  
  jeu = creation_jeu()
  player1 = tri_decr(distribue(jeu))
  player2 = tri_decr(distribue(jeu))
  print "Joueur 1 : ",player1
  print "Joueur 2 : ",player2
  print "Talon : ",jeu
  # la partie commence
  if is_player1_first(player1,player2):
    print "Joueur1 commence"
    table = [ player1[0] ]
    player1.pop(0)
    a_qui_le_tour = 2
  else:
    print "Joueur2 commence"
    table = [ player2[0] ]
    player2.pop(0)
    a_qui_le_tour = 1
  


