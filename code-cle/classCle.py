#!/usr/bin/python
import math   # This will import math module

class cle():
  """La classe cle : nombres positifs non nuls en code CLE"""
  pass

  def __init__(self, lst):
    """Initialisation de la classe cle (constructeur)"""
    try:
      lst = list(lst)
    except TypeError:
        print("l'argument doit etre de type list.")
    self.lst = lst

  def affiche(self):
    """Methode d'affichage d'un code CLE
    - Exemples :
    >>> cle([7, 3, 0]).affiche()
    [7, 3, 0]
    >>> p = cle(7)
    l'argument doit etre de type list.
    """
    return self.lst

  ## Trois methodes redefinies... pour gagner du temps
  def __str__(self):
    """Permet d'afficher les messages d'erreur correctement"""
    return self.lst
  def __len__(self):
    """Permet de demander len(cle) plutot que len(cle.lst)"""
    return len(self.lst)
  def __getitem__(self,key):
    """Permet de demander cle[i] plutot que cle.lst[i]"""
    return self.lst[key]

  def cle_to_deci(self):
    """Methode de calcul de l'ecriture decimale d'un nombre en code CLE.
    - Exemples :
    >>> p = cle([6, 1])
    >>> p.cle_to_deci()
    66.0
    >>> cle([7, 3, 0]).cle_to_deci()
    137.0
    """
    n = 0
    for i in range(len(self)):
      n = n + math.pow(2, self[i])
    return n

  def est_plus_grand_que(self,other):
    """Methode de test du classement de deux nombres en code CLE.
    - Algorithme : Comparer les premiers elements de chaque 
    liste jusqu'a obtenir une comparaison. 
    En cas d'egalite de tous les elements communs aux deux listes, 
    on departage les deux listes par leur longueur.
    - Exemples :
    >>> p = cle([7, 3, 0])
    >>> q = cle([7, 2])
    >>> p.est_plus_grand_que(q)
    True
    >>> q.est_plus_grand_que(cle([7, 2, 1, 0]))
    False
    """
    i = 0
    while self[i] == other[i] and i < min(len(self),len(other))-1:
      i = i+1
    if self[i] > other[i] or ( self[i] == other[i] and len(self) > len(other) ):
      return True
    return False

  def __add__(self,other):
    """Redefinition pour la classe cle de la methode __add__
    qui implemente l'operateur d'addition +.
    L'operation c + d sera resolue pour les codes cle.
    - Exemples :
    >>> ( cle([7, 3, 0]) + cle([7, 1]) ).affiche()
    [8, 3, 1, 0]
    >>> ( cle([11, 1, 0]) + cle([11, 7, 3, 0]) ).affiche()
    [12, 7, 3, 2]
    """
    return cle(somme_bulle(self.lst,other.lst))
    #return cle(somme_tri(self.lst,other.lst))

  def __sub__(self,other):
    """Redefinition pour la classe cle de la methode __sub__
    qui implemente l'operateur de soustraction -.
    L'operation c - d sera resolue pour les codes cle.
    - Exemples :
    >>> ( cle([7, 3, 0]) - cle([7, 1]) ).affiche()
    [2, 1, 0]
    >>> cle([7, 3, 0]) - cle([8])
    'Erreur : la difference doit etre positive'
    >>> ( cle([12]) - cle([11, 7, 3, 0]) ).affiche()
    [10, 9, 8, 6, 5, 4, 2, 1, 0]
    """
    c = diff_cle(self.lst,other.lst)
    if not isinstance(c, list):
      return("Erreur : la difference doit etre positive")
    return cle(c)

  def __mul__(self,other): # implemete l'operateur *
    """Redefinition pour la classe cle de la methode __mul__
    qui implemente l'operateur de multiplication *.
    L'operation c * d sera resolue pour les codes cle.
    - Exemples :
    >>> ( cle([5, 2, 0]) * cle([4]) ).affiche()
    [9, 6, 4]
    >>> ( cle([12, 4]) * cle([12, 4]) ).affiche()
    [24, 17, 8]
    >>> ( cle([5, 3]) * cle([7, 2, 0]) ).affiche()
    [12, 10, 7, 6, 3]
    """
    c = []
    for i in range(len(self)):
      for j in range(len(other)):
        c = c + [ self[i] + other[j] ]
    return cle(c) + cle([])

  #def __div__(self,other): # implemente l'operateur /

def entier_to_cle(n):
  """Fonction de calcul de la liste des nombres apparaissant
  dans le code CLE d'un nombre entier en base 10 donne.
  - Algorithme : Etude du reste dans les divisions 
  successives par deux
  - Exemples : 
  >>> entier_to_cle(137).affiche()
  [7, 3, 0]
  >>> entier_to_cle(66).affiche()
  [6, 1]
  """
  x = float(n) # pour premier le test de parite dans la boucle while
  c = cle([])
  i = 0
  while x>0:
    if math.floor(x/2) != x/2 :
      c.lst.insert(0,i)
    x = math.floor(x/2)
    i = i+1
  return c

def somme_tri(c,d):
  """Fonction de calcul de la liste des nombres apparaissant
  dans le code cle de la somme de deux nombres en 
  code CLE (deux listes non vides)
  - Algorithme : Tri prealable d'une liste formee des
  deux listes fusionnees puis sommation des jumeaux
  """
  s = c+d
  s.sort(reverse=True)
  for i in range(len(s)-1,0,-1):
    if s[i-1]==s[i]:   # addition
      s[i-1] = s[i]+1
      s.pop(i)
  return s  

def somme_bulle(c,d):
  """Fonction de calcul de la liste des nombres apparaissant
  dans le code cle de la somme de deux nombres en 
  code CLE (deux listes non vides)
  - Algorithme : Tri a bulle et puis sommation 
  des jumeaux
  """
  s = c+d
  flag = 0
  while flag==0 and len(s)>1:
    flag = 1
    for i in range(len(s)-1,0,-1):
      if s[i-1]==s[i]:   # addition
        s[i-1] = s[i]+1
        s.pop(i)
        flag = 0
      elif s[i-1]<s[i]:  # permutation
        tmp = s[i]
        s[i] = s[i-1]
        s[i-1] = tmp
        flag = 0
  return s

def diff_cle(c,d):
  """Fonction de calcul de la liste des nombres apparaissant
  dans le code cle de la difference de deux nombres positifs 
  c et d (c>d) en code CLE (deux listes non vides)
  - Algorithme : voir la documentation
  """
  if cle(d).est_plus_grand_que(cle(c)):
    return "Erreur : la difference doit etre positive"
  u = c
  for i in range(len(d)):
    n = d[i]
    j = len(u)-1
    while ( u[j]<n and j>0 ):
      j = j-1
    u = u[:j] + [k for k in range(u[j]-1,n-1,-1)] + u[j+1:]
  return u

if __name__ == "__main__":
    import doctest
    doctest.testmod()

