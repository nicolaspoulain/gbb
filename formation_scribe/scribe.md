%Formation scribe
%Nicolas Poulain - Dafor 2012


# Les modules EOLE

Extrait de
<http://eole.orion.education.fr/index.php/home>

![Logo EOLE](scribe_html_2ce1ff75.png)

Eole est un projet collaboratif basée sur la philosophie du logiciel libre.
La mutualisation des compétences et des moyens permet de
réaliser des solutions économiques, fiables et performantes.

Chaque module constitue une distribution GNU/LINUX spécifique qui
permet d'installer facilement un serveur dédié. Les services offerts
sont pré-configurés, l'ensemble est cohérent. Vous devez télécharger sur
ce site l'image ISO qui vous permettra de graver un DVD ou un CD
d'installation. Ce DVD/CD est multi module, le choix du module à
installer est proposé au démarage (boot).

Les modules sont disponibles en deux versions 

EoleNg 2.2                                      EoleNg 2.3 
------------------------------------------      ---------------------------------------
-   Disponible depuis le 16 Janvier 2009.       -   Disponible depuis le 8 Juin 2011.
-   basée sur la version 8.04 Ubuntu            -   basée sur la version 10.04 Ubuntu
-   Arrêt du support mises à jour : Juin 2013   -   Arrêt du support mises à jour : Juin 2015


## Scribe : Un serveur pédagogique complet.

Scribe est un contrôleur de domaine dotée de fonctions évoluées. Il
optimise la gestion de votre parc de stations clientes. Il dispose d'un
annuaire qui référence, élèves, parents, personnels enseignant et
administratifs, il propose un service de messagerie et héberge vos
applications web au sein d'un portail Web 2.0.

Scribe est un contrôleur de domaine.
-   gestion des connexions réseau des utilisateurs ;
-   partage de fichiers et de répertoires ;
-   support des ACL ;
-   partage d'imprimantes ;
-   gestion des comptes utilisateurs et des accès ;
-   exécution d'applications utilisateurs ;


Scribe est un système de messagerie articulé autour d'un annuaire
performant.
-   l'annuaire est initialisé à partir d'importation de comptes (SCONET,
    BE1D, AAF, CSV,...) ;
-   l'annuaire peut servir de base d'authentification pour d'autres
    services réseaux ;
-   la messagerie gère deux domaines distincts (l'Internet et l'intranet
    académique) ;
-   utilisation au choix d'une interface web multilingue ou d'un client
    de messagerie standards.
-   un service de listes de diffusion ;
-   une sécurité anti­spam, un anti­virus, une gestion de quotas (taille
    des boites aux lettres)


Scribe offre des services web
-   un serveur web;
-   un portail web;
-   des applications pré-installés


Scribe est un serveur d'authenfication unqiue (SSO)
-   Eole SSO Utilise l'annuaire LDAP
-   Les standards C.A.S 2 et OpenID sont supportés
-   La féération d'identité est possible via le protocole SAML.


Scribe dispose d'une gestion avancée des utilisateurs et des
postes clients
-   Distribution de devoir
-   Controle d'acces à Internet et au services réseaux
-   appliquer des restrictions ou pré­configurer des applications, en
    fonction du login de l'utilisateur ou de ses groupes et du nom de la
    machine sur laquelle il se connecte ;
-   effectuer des actions distantes sur les stations (fermer la session,
    éteindre ou redémarrer un ou plusieurs postes) ;
-   surveiller la détection de virus par le serveur ;


## Schéma d'un réseau d'établissement

Sur l'académie de Paris, les réseaux d'établissement sont organisés par
la DSI, service informatique du Rectorat qui assure en plus
l'administration et la maintenance des serveurs Amon, Scribe et Horus. Voir figure \ref{2149feac}.

![\label{2149feac}](scribe_html_2149feac.png)

Les établissements assurent par eux-même les tâches de gestion courante
du réseau et des machines avec les droits qui leur sont laissé en
utilisant au travers des interfcaes de gestion de ces serveurs.


## Déroulement de la formation

Durant cette formation, nous allons simuler un réseau d'établissement
grâce à des machines virtuelles. Cela nous permettra de contrôle
complètement tous les éléments, d'effectuer tous les tests sans risque
pour les machines réelles et le réseau qui nous entourent.

Sur la figure suivante, on simule plusieurs machines sur une seule
appelée machine hôte. Un poste windows virtuel est un ordinateur
virtuel sur leque a été installé un système d'exploitation comme on le
fait sur une machine réelle.

![\label{413f4ff1}](scribe_html_413f4ff1.png)

La situation décrite sur la figure \ref{413f4ff1} est équivalente à celle de la figure \ref{78733841}.

![\label{78733841}](scribe_html_78733841.png)

Notre travail maintenant est de créer la machine virtuelle qui va
accueillir le sytème d'exploitation Linux sur lequel fonctionne.

Après avoir demandé la création d'une nouvelle machine dans VirtualBox,
puis donné un nom (disons scribe) et choisi le type de système
d'exploitaion (ici Linux-Ubuntu), passez toutes les étapes en acceptant
les choix par défaut (sauf pour la mémoire vive que nous passerons de
512Mo à 256Mo).

Une fois ces opérations terminées,
-   cocher la case «Activer PAE/NX» dans l'onglet «Processeur» de la
    section «Système» de votre machine virtuelle.
-   choisir le mode d'accès par pont pour la carte réseau 1. Elle
    prendra ainsi une adresse sur le réseau sur lequel se trouve la
    machine hôte.
-   L'image iso du dvd eole, téléchargée sur le site Eole
    <http://eoleng.ac-dijon.fr/pub/iso/> doit être monté comme un cd(ou dvd)
    virtuel, pour cela cliquez sur l'icône cd-rom dans l'onglet
    «Stockage», comme sur l'image ci-dessous puis naviguez dans
    l'arborescence pour indiquer le fichier .iso.

![Virtualbox](scribe_html_m69a9bc5b.png)\

On peut alors mettre en marche la machine virtuelle.

# Mise en place du serveur

## Installation

![image](scribe_html_m1451a7db.png)

Après avoir vérifié au niveau du BIOS
que la machine démarre prioritairement
sur le CD, le menu ci-contre apparaît.

Une bonne idée consiste à vérifier si le disque (le CD) a des défauts
afin d'éviter de perdre du temps avec une installation qui échouerait
sans doute.

Une fois ceci fait, lançons l'installation de Scribe.
La procédure est automatique et vous n'avez qu'à observer les étapes

![image](scribe_html_4b5e166b.png)

Après redémarrage, le gestionnaire de démarrage GRUB vous propose
plusiers entrées. Nous choisissons celle par défaut

![image](scribe_html_m6522c87a.png)

Vous pouvez maintenant vous identifier avec le login «root».

-   Pour scribe 2.2, le mot de passe est ``eole``,
-   Pour scribe 2.3, le mot de passe est ``$eole&123456$``.


Avant de nous lancer dans la configuration de notre contôleur de domaine
scribe, notons ici quelques informations réseau dont nous aurons besoin
pour configurer notre serveur :


- L'adresse IP fixe que nous allons lui attribuer : _ _ _ . _ _ _ .
_ _ _ . _ _ _
- Le masque de sous-réseau : _ _ _ . _ _ _ . _ _ _ . _ _ _
- L'adresse de broadcast : _ _ _ . _ _ _ . _ _ _ . _ _ _
- L'adresse IP du ou des serveur(s) DNS : 
    - _ _ _ . _ _ _ . _ _ _ .
_ _ _
    - _ _ _ . _ _ _ . _ _ _ . _ _ _
- Le nom que nous donnerons à la machine :
- Le nom du domaine auquel les clients vont s'authentifier :
- Le mot de passe root[^1] : ``dog!cat$mouse``
- Le mot de passe eole ou scribe : ``fox!bird$fly``
- Le mot de passe admin : ``plop``

[^1]: root : utiisateur linux ; a tous les droits sur le serveur
    
    scribe : utilisateur linux ; peut faire certains tests via 
    une interface graphique

    admin : utilisateur samba ; gestionnaire des machines une fois 
    celles-ci connectées au domaine , c'est l'administrateur réseau).


Mise à jour :

``root@scribeng:~# Maj-Auto -i``

Saisissez la commande ``gen_config`` qui va lancer le générateur de
configuration

``root@scribeng:~# gen_config``

La série d'écrans est à renseigner en étant très attentif.

### Configuration scribe 2.2

Voir fugures \ref{m4d52829f} à \ref{m44b07c21}.

![Scribe 2.2 \label{m4d52829f}](scribe_html_m4d52829f.jpg)

![Scribe 2.2 ](scribe_html_1f004f88.jpg)

![Scribe 2.2 ](scribe_html_m5ada8c94.jpg)

![Scribe 2.2 \label{m44b07c21}](scribe_html_m44b07c21.jpg)

### Configuration scribe 2.3

Voir fugures \ref{5dc039bc} à \ref{m6ec096b1}.

![Scribe 2.3 \label{5dc039bc}](scribe_html_5dc039bc.png)

![Scribe 2.3 ](scribe_html_m291b378c.png)

![Scribe 2.3 ](scribe_html_m408bb4bb.png)

![Scribe 2.3 ](scribe_html_43381116.jpg)

![Scribe 2.3 ](scribe_html_m672d0ca.png)

![Scribe 2.3 ](scribe_html_71d87af7.png)

![Scribe 2.3 ](scribe_html_m51dc1543.png)

![Scribe 2.3 ](scribe_html_28489be3.png)

![Scribe 2.3 \label{m6ec096b1}](scribe_html_m6ec096b1.png)

### Instanciation

Après avoir rempli les champs, on peut enregistrer ces paramètres qui
vont constituer un fichier nommé «zephir.eol» dont le rôle est de donner
les instructions au programme d'instanciation instance.

``root@scribeng:~# instance zephir.eol``

Au cours de l'exécution du programme, il vous sera demandé de changer
les mots de passe

-   du super administrateur Linux nommé «root»;
-   de l'administrateur scribe restreint nommé «eole» ou «scribe»
-   de l'admistrateur du domaine nommé «admin».

Lors de l'installation, le programme vous proposera sans doute des
télécharger des mises à jour il est essentiel d'accepter car elles
corrigent des problèmes de sécurité comme elles apportent des
améliorations des programmes de la distribution Ubuntu mais aussi de la
suite Scribe développée par l'académie de Dijon.

À la fin du processus, après construction des bases de données et
exécution de tous les scripts de pré/postinstance, la machine doit
redémarrer.

### Pourquoi n'utiliser instance qu'une seule fois ?

Un fois les modifications apportées au fichier config.eol, on se gardera
bien de relancer le programme d'instanciation instance. En effet il
poserait des questions inutiles, pourrait effacer des comptes, perturber
gravement les connexions au domaine des machines déjà intégrées.

En cas de changement sur le réseau ou d'erreur lors de la configuration,
il est possible de relancer la commande gen_config afin de modifier
le fichier /etc/eole/config.eol. Pour ensuite lancer une reconfiguration
avec le programme reconfigure.
dont la fonction est de lire le fichier /etc/eole/config.eol pour en
appliquer les paramètres aux différents services en fonctionnement.

~~~~
root@scribeng:~# gen_config /etc/eole/config.eole
root@scribeng:~# reconfigure
~~~~


## Intégration des clients au domaine

-   Première étape : faire entrer les machines windows dans le domaine.
    La copie d'écran suivante montre l'utilisateur admin faisant entrer
    dans le domaine une machine sous WindowsXP

    ![intégration au domaine](scribe_html_78aacfe6.png)

-   Deuxième étape : installer le client scribe.

    Après redémarrage et connexion en tant qu'admin sur le domaine,
    rendons-nous dans le poste de travail puis dans le répertoire personnel
    nommé U: Nous allons exécuter le programme ``Install_Client_Scribe``.

    ![Installation du client scribe](scribe_html_m4358be12.png)

# L'E.A.D. (Eole ADmininistration)

## Présentation

L'EAD est une interface WEB qui permet de faire l'administration de
premier niveau de toutes les composantes du serveur Scribe : système,
messagerie, utilisateurs, groupes,… Il offre également aux professeurs
la possibilité de modifier leurs préférences, gérer les élèves, les
groupes dont ils sont responsables…

L'accès à l'EAD se fait depuis le navigateur web FIREFOX ou CHROME
(mais PAS Internet Explorer) avec l'URL suivante :
<https://srv-scribe:4200/> (ou
<https://xxx.xxx.xxx.xxx:4200> avec l'adresse IP du serveur Scribe).

![L'EAD](scribe_html_661f3efb.jpg)

Cliquez sur scribe puis à la connexion entrez le nom d'utilisateur et le
mot de passe associé.

En tant qu'administrateur, vous allons créer la base des élèves de
l'établissement à partir des fichiers zip ou xml récupérés depuis sconet
(fichiers exemple sur <http://cjoint.com/?0AiqKCRRtW3>):

-   ``ExportXML_ElevesSansAdresses.zip`` ou ``ElevesSansAdresses.xml``
-   ``ExportXML_Nomenclature.zip`` ou ``Nomenclature.xml``
-   ``ExportXML_Structures.zip`` ou ``Structures.xml``


Dans l'EAD, cliquez Outils puis successivement sur Importation ->
Importation annuelle des bases. Choisissez ensuite «Sconet» puis «Élèves
seulement». Les trois fichiers seront traités pour intégrer les élèves
au système en créant leurs identifiants et mots de passe selon vos
préférences.

Les profils utilisateurs représentent l'environnement par défaut des
utilisateurs. Il existe trois types de profils :

-   le profil local : il est stocké sur la station Windows,
    l'environnement est donc modifié lorsque l'utilisateur change de
    poste
-   le profil itinérant : il est stocké dans le répertoire personnel de
    l'utilisateur, l'environnement suit l'utilisateur
-   les profils obligatoires : ils sont stockés dans un répertoire
    commun, l'environnement est le même pour tous mais il faut
    générer les profils avant de pouvoir les utiliser

EOLE gère ces trois types. Il n'y a rien de particulier à faire pour les
profils locaux ou itinérants. Par contre, il faut créer les profils
obligatoires.

![Import des utilisateurs](scribe_html_10e3ad18.jpg)

il est aussi possible de passer par l'import d'un fichier plus
rudimentaire au format texte .csv dont voici une strucure type :

~~~~
"numero";"nom";"prenom";"sexe";"date";"classe";"niveau"
"0224ISWV7201";"DUPOND";"Norbert";"M";"07/12/1997";"3e1";"3"
"0544ETME3593";"MARTIN";"Maxence";"F";"04/09/1999";"4e1";"4"
"3887ELDEX3983";"DURAND";"Raoul";"M";"03/02/1999";"4e1";"4"
~~~~

## Exercices sur la gestion des utilisateurs

(@) Exercice : Créer les comptes à partir des fichiers sconet.

    -   Tester un ou deux des comptes créés.
    -   Dans l'EAD, accéder au compte d'un élève, lire les informations
        disponibles (en particulier le quota disque) et réinitialiser son
        mot de passe (en forçant sa modification à la 1ère connexion)
    -   Tester l'édition groupée pour réinitialisation tous les mots de
        passe des élèves.
     
(@) Exercice : Créez un nouvel élève, ayant les propriétés suivantes
:
    -   Login : votreprenom.eleve (N ⇒ numéro donné par le formateur)
    -   prénom : le votre , nom = eleve , mot de passe = 1234
    -   Numéro interne de l'élève : 400N
    -   Quota disque = 50 Mo
    -   Date de naissance = date du jour
    -   Profil utilisateur = Obligatoire
    -   Classe : stage\_formation3
    -   Vérifier sa connexion et l'accès à ses partages dans le poste de
    -   Listez tous les élèves dont le login commence par un T
    -   Editez les propriétés d'un élève
    -   Editez les propriétés d'un professeur

(@) Exercice : Créez un nouveau professeur, ayant les propriétés
suivantes :
(Attention les caractères spécifiques tels que les accents sont interdits
dans le nom de login)
    -   Login : votreprenom.prof
    -   prénom : le votre , nom = prof, mot de passe = 1234
    -   Quota disque = 100 Mo
    -   Profil utilisateur = Obligatoire
    -   Il n'est pas professeur principal, ni membre du groupe DomainAdmins
    -   Pas d'activation du Shell

(@) Exercice : L'utilisateur crée dans l'exercice précédent est
professeur de mathématiques dans les classes 4e1 et 4e2
    -   Créez la matière Mathématiques
    -   Inscrivez-le dans sa matière, ses équipes pédagogiques et
        permettez-lui d'administrer la classe de 4e1
    -   Vérifier sa connexion et l'accès à ses partages dans le poste de
        travail.
    -   Se connecter à l'EAD en tant que professeur et vérifier son statut
        de professeur administrateur de la classe 4e1.

## Exercices sur la gestion des groupes

(@) Exercice : Vérifier si le groupe «Documentation» existe. Dans la
négative :
    -   créer un groupe de type Matières dont l'intitulé est
        «Documentation».
    -   Avec Partage et Modèle Données/Travail
    -   Pas de liste de diffusion
    -   Inclure les documentalistes dans le groupe “Documentation” (les
        créer si nécessaire et les associer à toutes les classes)

(@) Exercice : Créer un groupe «stage» ayant les propriétés
suivantes :
    -   Type «groupe»
    -   Avec partage : Modèle Données/Travail
    -   Pas de liste de diffusion
    -   Membres : 3 élèves (penser à leur appartenance à une classe pour les
        affecter tous en une seule manipulation) et le professeur
        «votreprenom.prof».
    -   Lister les membres du groupe stage via l'EAD
    -   Vérifier la création du partage et les droits effectifs des
        utilisateurs

# 4. ESU (Environnements Sécurisés des Utilisateurs)

## Présentation

ESU permet de gérer de façon la configuration de l'environnement de
l'utilisateur sur les stations Windows.Cette configuration est établie à
l'ouverture de la session, en fonction du nom de la machine et du groupe
d'utilisateurs auquel appartient l'utilisateur.

-   ESU configure les environnements à partir de règles (des clés de la
    base de registre Windows) qui sont stockées dans le fichier XML
    <\\adresse_serveur\\esu\\Console\\ListeRegles.xml>.
-   ESU permet de gérer le cragement des icônes du bureau et du menu
    démarrer. Ces icônes sont stockées dans le lecteur R: (icones$).
    On trouve ici les dossiers correspondant aux groupes de machines
    définis dans ESU.
    Par exemple pour le groupe <grp_eole> créé par défaut à
    l'installation de scribe,

![L'Arborescence du dossier icones$](scribe_html_6d99f570.png)

-   les icônes placées dans <R:\grp_eole\_Machine\Bureau> seront
    visibles par tous les utilisateurs du groupe <grp_eole>;
-   les icônes placées dans <R:\grp_eole\professeurs\Bureau> ne
    seront visibles que par les professeurs du groupe <grp_eole>;
-   les éléments placés dans <R:\grp_eole\professeurs\Menu>
    Démarrer s'afficheront dans le menu Démarrer des utilisateurs du
    groupe <grp_eole>;

Profils utilisateurs et ESU

Il est important de distinguer les profils utilisateurs (notion interne
à Windows) et ESU. Les profils utilisateurs sont appliqués en premier et
définissent un environnement de départ. La configuration ESU est
appliquée ensuite et modifie, ajoute ou supprime des paramètres de cet
environnement.

Par exemple, le menu démarrer est contenu dans le profil de
l'utilisateur mais si un chemin alternatif est défini dans ESU (Console
ESU : Windows -> Dossiers) alors, le menu démarrer utilisé sera
celui défini dans ESU, et non celui du profil.

Pour lancer la console, cliquez sur le raccourci présent dans le
répertoire personnel de l'utilisateur admin. 
Par défaut, seul l'utilisateur admin a accès à la console.

![Raccourci ESU](scribe_html_5a76214.jpg) 

La console ESU sert à paramétrer les règles qui seront appliquées sur
les machines clientes lors de l'ouverture de session. Elles sont
réparties en deux groupes :

-   les règles "machines" définissant le comportement global des
    machines, elles sont appliquées quelque soit l'utilisateur qui se
    connecte ;
-   les règles "utilisateurs" définissant l'environnement de
    l'utilisateur comme les restrictions, le paramétrage de
    l'explorateur et du fond d'écran, etc.
-   Chaque coche peut prendre 3 états :

![Les états dans ESU](scribe_html_4d04a591.png)

1. cochée : Règle activée
1. décochée : Règle désactivée
1. grisée : Règle non prise en compte

![image](scribe_html_3a7fb0ad.jpg)

## Règles de priorité dans l'application des règles ESU




