#Python#

Python est un langage de programmation multi-paradigme[^1]. Il favorise la
programmation impérative structurée, et orientée objet. Il est doté d'un typage
dynamique fort, d'une gestion automatique de la mémoire par ramasse-miettes et
d'un système de gestion d'exceptions ; il est ainsi similaire à 

* Perl
* Ruby
* Scheme
* Smalltalk
* Tcl

Implémentation de la fonction factorielle :
$$ x! = \prod_{n=1}^x n $$

```Python
 # Fonction factorielle en Python
 def factorielle(x):
     if x < 2:
         return 1
     else:
         return x * factorielle(x-1)
```

[^1]:Un paradigme de programmation est un style fondamental de programmation
informatique qui traite de la manière dont les solutions aux problèmes doivent
être formulées dans un langage de programmation


