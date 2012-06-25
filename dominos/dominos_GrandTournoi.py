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

def strategie_pire(table,player,possbl):
  """Place sur la table le dernier domino dans l'ordre lexicographique"""
  table = positionne(player[ possbl[len(possbl)-1] ], table)
  player.pop(possbl[len(possbl)-1])
  return table,player

def strategie_hasard(table,player,possbl):
  """Place sur la table un domino au hasard"""
  ind = random.randint(0,len(possbl)-1)  
  table = positionne(player[ possbl[ind] ], table)
  player.pop(possbl[ind])
  return table,player

def strategie_plus_de_points(table,player,possbl):
  """Place sur la table le domino qui vaut le plus de points"""
  # Repere l'indice du meillleur domino
  max = -1
  for i in range(len(possbl)):
    val = player[possbl[i]][0]+player[possbl[i]][1]
    if val > max:
      ind = i
      max = val
  # place le domino sur la table
  table = positionne(player[ possbl[ind] ], table)
  player.pop(possbl[ind])
  return table,player

def strategie_plus_present(table,player,possbl):
  """Place sur la table le domino dont la moyenne de presence des deux valeurs 
  est la plus elevee"""
  # Construit le tableau des effectifs des valeurs presentes dans la main
  effectifs = [0, 0, 0, 0, 0, 0, 0]
  for i in range(len(player)):
    effectifs[ player[i][0] ] = effectifs[ player[i][0] ] + 1
    effectifs[ player[i][1] ] = effectifs[ player[i][1] ] + 1
  # Repere l'indice du meilleur domino
  max_moyenne = -1
  for i in range(len(possbl)):
    moyenne = effectifs [ player[possbl[i]][0] ] + effectifs [ player[possbl[i]][1] ]
    if moyenne > max_moyenne:
      max_moyenne = moyenne
      ind = i
  # place le domino sur la table
  table = positionne(player[ possbl[ind] ], table)
  player.pop(possbl[ind])
  return table,player

def who_wins(player1,player2):
  somme_points_p1 = 0
  for i in range(len(player1)):
    somme_points_p1 = somme_points_p1 + player1[i][0] + player1[i][0]
  somme_points_p2 = 0
  for i in range(len(player2)):
    somme_points_p2 = somme_points_p2 + player2[i][0] + player2[i][0]
  if somme_points_p1 < somme_points_p2:
    return 1
  elif somme_points_p1 > somme_points_p2:
    return 2
  else:
    return 0

if __name__ == "__main__":
  nbparties=100000

  #############################################    
  resultats_tournoi_pire_has = [0, 0, 0]
  for i in range(nbparties*1):
    jeu = creation_jeu()
    player1 = tri_decr(distribue(jeu))
    player2 = tri_decr(distribue(jeu))
    # Initialisation de la partie
    if is_player1_first(player1,player2):
      table = [ player1[0] ]
      player1.pop(0)
      a_qui_le_tour = 2
    else:
      table = [ player2[0] ]
      player2.pop(0)
      a_qui_le_tour = 1
    # Les tours de jeu :
    cpt_passe = 0
    while cpt_passe<2 and len(player1)>0 and len(player2)>0:
      if a_qui_le_tour==1:
        possbl = possibilites(table,player1)
        if len(possbl)==0:
          cpt_passe = cpt_passe + 1
        else:
          table,player1 = strategie_pire(table,player1,possbl)
          cpt_passe = 0
        a_qui_le_tour = 2
      else:
        possbl = possibilites(table,player2)
        if len(possbl)==0:
          cpt_passe = cpt_passe + 1
        else:
          table,player2 = strategie_hasard(table,player2,possbl)
          cpt_passe = 0
        a_qui_le_tour = 1
  
    # Fin de partie : on affiche ce qu'il reste a chacun.
    winner = who_wins(player1,player2)
    resultats_tournoi_pire_has[winner] = resultats_tournoi_pire_has[winner] + 1

  #############################################    
  resultats_tournoi_pire_pres = [0, 0, 0]
  for i in range(nbparties*1):
    jeu = creation_jeu()
    player1 = tri_decr(distribue(jeu))
    player2 = tri_decr(distribue(jeu))
    # Initialisation de la partie
    if is_player1_first(player1,player2):
      table = [ player1[0] ]
      player1.pop(0)
      a_qui_le_tour = 2
    else:
      table = [ player2[0] ]
      player2.pop(0)
      a_qui_le_tour = 1
    # Les tours de jeu :
    cpt_passe = 0
    while cpt_passe<2 and len(player1)>0 and len(player2)>0:
      if a_qui_le_tour==1:
        possbl = possibilites(table,player1)
        if len(possbl)==0:
          cpt_passe = cpt_passe + 1
        else:
          table,player1 = strategie_pire(table,player1,possbl)
          cpt_passe = 0
        a_qui_le_tour = 2
      else:
        possbl = possibilites(table,player2)
        if len(possbl)==0:
          cpt_passe = cpt_passe + 1
        else:
          table,player2 = strategie_plus_present(table,player2,possbl)
          cpt_passe = 0
        a_qui_le_tour = 1
  
    # Fin de partie : on affiche ce qu'il reste a chacun.
    winner = who_wins(player1,player2)
    resultats_tournoi_pire_pres[winner] = resultats_tournoi_pire_pres[winner] + 1
 
  #############################################    
  resultats_tournoi_pire_point = [0, 0, 0]
  for i in range(nbparties*1):
    jeu = creation_jeu()
    player1 = tri_decr(distribue(jeu))
    player2 = tri_decr(distribue(jeu))
    # Initialisation de la partie
    if is_player1_first(player1,player2):
      table = [ player1[0] ]
      player1.pop(0)
      a_qui_le_tour = 2
    else:
      table = [ player2[0] ]
      player2.pop(0)
      a_qui_le_tour = 1
    # Les tours de jeu :
    cpt_passe = 0
    while cpt_passe<2 and len(player1)>0 and len(player2)>0:
      if a_qui_le_tour==1:
        possbl = possibilites(table,player1)
        if len(possbl)==0:
          cpt_passe = cpt_passe + 1
        else:
          table,player1 = strategie_pire(table,player1,possbl)
          cpt_passe = 0
        a_qui_le_tour = 2
      else:
        possbl = possibilites(table,player2)
        if len(possbl)==0:
          cpt_passe = cpt_passe + 1
        else:
          table,player2 = strategie_plus_de_points(table,player2,possbl)
          cpt_passe = 0
        a_qui_le_tour = 1
  
    # Fin de partie : on affiche ce qu'il reste a chacun.
    winner = who_wins(player1,player2)
    resultats_tournoi_pire_point[winner] = resultats_tournoi_pire_point[winner] + 1
      

  #############################################    
  resultats_tournoi_pres_point = [0, 0, 0]
  for i in range(nbparties*1):
    jeu = creation_jeu()
    player1 = tri_decr(distribue(jeu))
    player2 = tri_decr(distribue(jeu))
    # Initialisation de la partie
    if is_player1_first(player1,player2):
      table = [ player1[0] ]
      player1.pop(0)
      a_qui_le_tour = 2
    else:
      table = [ player2[0] ]
      player2.pop(0)
      a_qui_le_tour = 1
    # Les tours de jeu :
    cpt_passe = 0
    while cpt_passe<2 and len(player1)>0 and len(player2)>0:
      if a_qui_le_tour==1:
        possbl = possibilites(table,player1)
        if len(possbl)==0:
          cpt_passe = cpt_passe + 1
        else:
          table,player1 = strategie_plus_present(table,player1,possbl)
          cpt_passe = 0
        a_qui_le_tour = 2
      else:
        possbl = possibilites(table,player2)
        if len(possbl)==0:
          cpt_passe = cpt_passe + 1
        else:
          table,player2 = strategie_plus_de_points(table,player2,possbl)
          cpt_passe = 0
        a_qui_le_tour = 1
  
    # Fin de partie : on affiche ce qu'il reste a chacun.
    winner = who_wins(player1,player2)
    resultats_tournoi_pres_point[winner] = resultats_tournoi_pres_point[winner] + 1
      
  #############################################    
  resultats_tournoi_point_has = [0, 0, 0]
  for i in range(nbparties*1):
    jeu = creation_jeu()
    player1 = tri_decr(distribue(jeu))
    player2 = tri_decr(distribue(jeu))
    # Initialisation de la partie
    if is_player1_first(player1,player2):
      table = [ player1[0] ]
      player1.pop(0)
      a_qui_le_tour = 2
    else:
      table = [ player2[0] ]
      player2.pop(0)
      a_qui_le_tour = 1
    # Les tours de jeu :
    cpt_passe = 0
    while cpt_passe<2 and len(player1)>0 and len(player2)>0:
      if a_qui_le_tour==1:
        possbl = possibilites(table,player1)
        if len(possbl)==0:
          cpt_passe = cpt_passe + 1
        else:
          table,player1 = strategie_plus_de_points(table,player1,possbl)
          cpt_passe = 0
        a_qui_le_tour = 2
      else:
        possbl = possibilites(table,player2)
        if len(possbl)==0:
          cpt_passe = cpt_passe + 1
        else:
          table,player2 = strategie_hasard(table,player2,possbl)
          cpt_passe = 0
        a_qui_le_tour = 1
  
    # Fin de partie : on affiche ce qu'il reste a chacun.
    winner = who_wins(player1,player2)
    resultats_tournoi_point_has[winner] = resultats_tournoi_point_has[winner] + 1
       
  #############################################    
  resultats_tournoi_pres_has = [0, 0, 0]
  for i in range(nbparties*1):
    jeu = creation_jeu()
    player1 = tri_decr(distribue(jeu))
    player2 = tri_decr(distribue(jeu))
    # Initialisation de la partie
    if is_player1_first(player1,player2):
      table = [ player1[0] ]
      player1.pop(0)
      a_qui_le_tour = 2
    else:
      table = [ player2[0] ]
      player2.pop(0)
      a_qui_le_tour = 1
    # Les tours de jeu :
    cpt_passe = 0
    while cpt_passe<2 and len(player1)>0 and len(player2)>0:
      if a_qui_le_tour==1:
        possbl = possibilites(table,player1)
        if len(possbl)==0:
          cpt_passe = cpt_passe + 1
        else:
          table,player1 = strategie_plus_present(table,player1,possbl)
          cpt_passe = 0
        a_qui_le_tour = 2
      else:
        possbl = possibilites(table,player2)
        if len(possbl)==0:
          cpt_passe = cpt_passe + 1
        else:
          table,player2 = strategie_hasard(table,player2,possbl)
          cpt_passe = 0
        a_qui_le_tour = 1
  
    # Fin de partie : on affiche ce qu'il reste a chacun.
    winner = who_wins(player1,player2)
    resultats_tournoi_pres_has[winner] = resultats_tournoi_pres_has[winner] + 1
      
  print "pire_has",resultats_tournoi_pire_has
  print "pire_pres",resultats_tournoi_pire_pres
  print "pire_point",resultats_tournoi_pire_point
  print "pres_point",resultats_tournoi_pres_point
  print "point_has",resultats_tournoi_point_has
  print "pres_has",resultats_tournoi_pres_has

