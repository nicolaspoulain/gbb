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

def tri_decr(player):
  """Trie les dominos du joueur dans l'odre
  decroissant lexocographique
  - Exemple :
  >>> tri_decr([ [1,2], [3,4], [1,3], [2,0] ])
  [[3, 4], [2, 0], [1, 3], [1, 2]]
  """
  player = sorted(player)
  player.reverse()
  return player

def is_player1_first(pl1,pl2):
  """Le joueur 1 a-t-il une meilleure main que le joueur 2 ?"""
  if est_avant(pl1[0],pl2[0]):
   return True
  return False

def possibilites(table,pl):
  """Donne, la liste des dominos de pl qui peuvent
  etre places sur la table
  >>> possibilites([[3,4],[4,4]], [[2,3],[1,5],[4,6]])
  [0, 2]
  """
  possbl = []
  x=table[0][0]
  y=table[-1][1]
  for i in range(len(pl)):
    if pl[i][0]==x or pl[i][1]==x or pl[i][0]==y or pl[i][1]==y:
      possbl = possbl + [ i ] 
  return possbl

def positionne(domino,table):
  """Positionne le domino correctement sur la table"""
  if domino[0]==table[0][0]:
    table = [ [ domino[1],domino[0] ] ] + table
  elif domino[1]==table[0][0]:
    table = [ [ domino[0],domino[1] ] ] + table
  elif domino[0]==table[-1][1]:
    table = table + [ [ domino[0],domino[1] ] ]
  else: 
    table = table + [ [ domino[1],domino[0] ] ]
  return table 

if __name__ == "__main__":
  import doctest
  doctest.testmod()
  
  jeu = creation_jeu()
  player1 = tri_decr(distribue(jeu))
  player2 = tri_decr(distribue(jeu))
  print "La main du Joueur 1 : ",player1
  print "La main du Joueur 2 : ",player2
  print ""
  # initialisation de la partie
  if is_player1_first(player1,player2):
    table = [ player1[0] ]
    print "Joueur 1 joue : ", table
    player1.pop(0)
    a_qui_le_tour = 2
  else:
    table = [ player2[0] ]
    print "Joueur 2 joue : ", table
    player2.pop(0)
    a_qui_le_tour = 1
  
  # c'est parti
  cpt_passe = 0
  while cpt_passe<2 and len(player1)>0 and len(player2)>0:
    if a_qui_le_tour==1:
      possbl = possibilites(table,player1)
      if len(possbl)==0:
        print "Joueur 1 passe."
        cpt_passe = cpt_passe + 1
      else:
        table = positionne(player1[ possbl[0] ], table)
        player1.pop(possbl[0])
        cpt_passe = 0
        print "Joueur 1 joue : ", table
      a_qui_le_tour = 2
    else:
      possbl = possibilites(table,player2)
      if len(possbl)==0:
        print "Joueur 2 passe."
        cpt_passe = cpt_passe + 1
      else:
        table = positionne(player2[ possbl[0] ], table)
        player2.pop(possbl[0])
        cpt_passe = 0
        print "Joueur 2 joue : ", table
      a_qui_le_tour = 1
  
  # Fin de partie
  print "La main du Joueur 1 : ",player1
  print "La main du Joueur 2 : ",player2
  

