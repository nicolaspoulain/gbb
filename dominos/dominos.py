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
  """Le joueur A a-t-il une meilleure main que le joueur B ?"""
  if est_avant(pl1[0],pl2[0]):
   return True
  return False

def possibilites(table,pl):
  """Donne, la liste des dominos de pl qui peuvent
  etre places sur la table
  >>> possibilites([3,4], [[2,3],[1,5],[4,6]])
  [0, 2]
  """
  possbl = []
  x=table[0]
  y=table[-1]
  for i in range(len(pl)):
    if pl[i][0]==x or pl[i][1]==x or pl[i][0]==y or pl[i][1]==y:
      possbl = possbl + [ i ] 
  return possbl

def un_tour_de_jeu(table,player,passe):
  """Tente de placer sur un des deux bouts de la table un
  domino de la maon du player. Si ce n'est pas spossible,
  on incremente passed_tours.
  """
  possbl = possibilites(table,player)
  if len(possbl)==0:
    return table,player,passe + 1
  table = table + player[ possbl[0] ]
  player.pop(possbl[0])
  return table,player,0

if __name__ == "__main__":
  import doctest
  doctest.testmod()
  
  jeu = creation_jeu()
  player1 = tri_decr(distribue(jeu))
  player2 = tri_decr(distribue(jeu))
  print "Joueur 1 : ",player1
  print "Joueur 2 : ",player2
  print ""
  # la partie commence
  if is_player1_first(player1,player2):
    print "Joueur1 commence"
    table = player1[0]
    player1.pop(0)
    a_qui_le_tour = 2
  else:
    print "Joueur2 commence"
    table = player2[0] 
    player2.pop(0)
    a_qui_le_tour = 1
  print "Table : ", table
  passed_tours = 0
  while passed_tours<2 and len(player1)>0 and len(player2)>0:
    if a_qui_le_tour==1:
      table,player1,passed_tours = un_tour_de_jeu(table,player1,passed_tours)
      a_qui_le_tour = 2
      print "j1->Table : ", table
    else:

      table,player2,passed_tours = un_tour_de_jeu(table,player2,passed_tours)
      a_qui_le_tour = 1
      print "j2->Table : ", table
  print "Joueur 1 : ",player1
  print "Joueur 2 : ",player2
  

