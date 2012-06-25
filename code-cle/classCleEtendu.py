#!/usr/bin/python
import math   # This will import math module
from classCle import *

class cle_etendu(cle):
  """La classe cle : nombres decimaux relatifs en code CLE"""
  pass

  def cle_to_deci(self):
    """Surcharge de la methode de la classe cle pour la prise
    en compte du bit de signe
    - Exemples :
    >>> p = cle_etendu([-1, 9, 3, 2, 0, -2])
    >>> p.cle_to_deci()
    -525.25
    >>> cle_etendu([0, 0]).cle_to_deci()
    0.0
    >>> cle_etendu([1, 19, -7]).cle_to_deci()
    524288.0078125
    """
    return self[0] * cle(self[1:]).cle_to_deci()

  def est_plus_grand_que(self,other):
    """Surcharge de la methode de la classe cle pour la prise
    en compte du bit de signe
    - Exemples :
    >>> cle_etendu([1, 5]).est_plus_grand_que(cle_etendu([1, 4]))
    True
    >>> cle_etendu([-1, 5]).est_plus_grand_que(cle_etendu([-1, 4]))
    False
    >>> cle_etendu([1, 5]).est_plus_grand_que(cle_etendu([-1, 4]))
    True
    >>> cle_etendu([1, 4]).est_plus_grand_que(cle_etendu([-1, 5]))
    True
    """
    if self[0] > other[0]:
      return True     # cas signe(a) > signe(b)
    elif self[0] < other[0]:
      return False    # cas signe(a) < signe(b)
    elif self[0]==1:
      if cle(self[1:]).est_plus_grand_que(cle(other[1:])):
        return True   # cas a >  b > 0
      return False    # cas b >= a > 0
    elif self[0]==-1:
      if cle(self[1:]).est_plus_grand_que(cle(other[1:])):
        return False  # cas a <  b < 0
      return True     # cas b <= a < 0
    return True       # cas a =  b = 0

  def __add__(self,other):
    """Redefinition pour la classe cle_etendu de la methode __add__
    qui implemente l'operateur d'addition +.
    L'operation c + d sera resolue pour les codes cle_etendu.
    - Exemples :
    >>> p = cle_etendu([-1, 7, 3, 0, -3])
    >>> q = cle_etendu([1, 7, 1])
    >>> print p.cle_to_deci(),"+",q.cle_to_deci(),"=",(p+q).affiche(),"=",(p+q).cle_to_deci()
    -137.125 + 130.0 = [-1, 2, 1, 0, -3] = -7.125
    >>> r = cle_etendu([1, 9, 1])
    >>> print p.cle_to_deci(),"+",r.cle_to_deci(),"=",(p+r).affiche(),"=",(p+r).cle_to_deci()
    -137.125 + 514.0 = [1, 8, 6, 5, 4, 3, -1, -2, -3] = 376.875
    """
    if self[0] * other[0] > 0:
      c = [self[0]] + ( cle(self[1:]) + cle(other[1:]) ).lst
    elif self[0] * other[0] <0:
      if cle(self[1:]).est_plus_grand_que(cle(other[1:])):
        c = [self[0]] + ( cle(self[1:]) - cle(other[1:]) ).lst
      else:
        c = [other[0]] + ( cle(other[1:]) - cle(self[1:]) ).lst
    elif self[0]==0:
        c = other.lst   
    else:
        c = self.lst
    return cle_etendu(c)

  def __sub__(self,other):
    """Redefinition pour la classe cle_etendu de la methode __sub__
    qui implemente l'operateur d'addition -.
    L'operation c - d sera resolue pour les codes cle_etendu.
    Algorithme : implementation du fait que c - d = c + (-d)
    """
    return self + cle_etendu( -1*other[0] + cle(other[1:]).lst )

  def __mul__(self,other):
    """Redefinition pour la classe cle_etendu de la methode __mul__
    qui implemente l'operateur de multiplication *.
    L'operation c * d sera resolue pour les codes cle_etendu.
    - Exemples :
    >>> ( cle_etendu([-1, 5, 2, 0]) * cle_etendu([-1, 4]) ).affiche()
    [1, 9, 6, 4]
    >>> ( cle_etendu([1, 12, 4]) * cle_etendu([-1, 12, 4]) ).affiche()
    [-1, 24, 17, 8]
    >>> ( cle_etendu([-1, 5, 3]) * cle_etendu([1, 7, 2, 0]) ).affiche()
    [-1, 12, 10, 7, 6, 3]
    """
    signe = self[0]*other[0] 
    val_abs = ( cle(self[1:]) * cle(other[1:]) ).lst
    return cle_etendu( [signe] + val_abs )

def deci_to_cle(n,prof_max):
  """Fonction de calcul de la liste des nombres apparaissant
  dans le code CLE d'un nombre decimal relatif en base 10 donne.
  Une profondeur maximale est a prciser puisque l'ecriture CLE 
  d'un nombre decimal n'est pas necessairement finie.
  - Algorithme : on reutilise la fonction programmee pour la partie entiere
  on cree un fonction recursive pour la partie fractionnaire
  - Exemples : 
  >>> deci_to_cle(-525.25, 5).affiche()
  [-1, 9, 3, 2, 0, -2]
  >>> deci_to_cle(524288.0078125, 10).affiche()
  [1, 19, -7]
  >>>  
  """
  if n==0:
    return [0, 0]
  # Partie entiere
  p_ent = math.floor(abs(n))
  p_ent_lst = entier_to_cle(p_ent).lst
  # Partie fractionnaire
  p_frac = float(abs(n)-math.floor(abs(n)))
  p_frac_lst = frac_to_cle(p_frac,prof_max,0,[])
  # Conacatenation du signe et des deux parties
  return cle_etendu([int(abs(n)/n)] + p_ent_lst + p_frac_lst)

def frac_to_cle(p_frac, prof_max, idx, cle_p_frac):
  """Fonction a appel recursif pour le calcul de
  la representation CLE de la partie decimale d'un nombre.
  """
  if p_frac==0 or idx==prof_max:
    return list(cle_p_frac) 
  if 2*p_frac>=1:
    return frac_to_cle(2*p_frac-1, prof_max, idx+1, cle_p_frac+[-idx-1])
  else:
    return frac_to_cle(2*p_frac, prof_max, idx+1, cle_p_frac)
 

if __name__ == "__main__":
    import doctest
    doctest.testmod()

