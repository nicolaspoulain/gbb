function dominosPipernoOlivier
clear
clc

%Ci-dessous figurent le déroulement d'une partie de dominos à l'aide de
%fonctions de bases%

D=creedominos;    %Création du jeu de dominos%

[O,C]=distribue(10,D)    %Distribue leurs dominos aux deux joueurs%

[J,O,C,tour]=quicommence(O,C);    %Lancement de la partie%

passe=0;     %Lancement de la formation de la chaîne de dominos%
while passe~=2 & length(O)~=0 & length(C)~=0
if tour==1
    [J,O,tour,passe]=joue(J,O,tour,passe);
    if passe==0
    O
    end
else [J,C,tour,passe]=joue(J,C,tour,passe);
    if passe==0
    C
    end
end
end

disp('La partie est finie :');    %Annonce la fin de la partie et déclare qui a gagné%
a=point(O);
b=point(C);
if length(O)==0 | a<b
    disp('Le joueur O a gagné!')
elseif length(C)==0 |a>b
    disp('Le joueur C a gagné!')
else disp('C''est une égalité!')
end
        

%Ci-dessous figurent toutes les fonctions de base du programme%

function[D] = creedominos %Génère tous les dominos sous forme de matrice%
D=[];
V=[];
for k=6:-1:0
    for i=k:-1:0
        V=[k;i];
        D=[D,V];
    end
end

function[O,D] = tirehasard(n,D) %Tire un nombre n de dominos au hasard dans la série D%
O=zeros(2,n);
for k=1:n
    a=floor(rand(1)*(size(D,2)))+1;
    O(:,k)=D(:,a);
    D(:,a)=[];
end

function[x] = estavant(a,b) %Compare deux dominos en fonction de l'ordre lexicographique%
x=2;
if a(1,1)>b(1,1) | ((a(1,1)==b(1,1) & a(2,1)>b(2,1)))
    x=1;
end

function[x] = estplusfort(a,b) %Compare deux dominos en fonction de la somme de leurs 2 chiffres%
x=1;
if a(1,1)+a(2,1)<b(1,1)+b(2,1)
    x=2;
end

function[T] = tridecdominos(O) %Tri les dominos dans l'ordre décroissant%
T=zeros(2,size(O,2));
for k=1:size(O,2)
    y=size(O,2);
    for i=1:size(O,2)
        x=estavant(O(:,k),O(:,i));
        if x==1
            y=y-1;
        end
    end
    T(:,y)=O(:,k);
end
    
function[L] = doublemoi(O) %Donne la liste des double de O%
L=[];
for k=1:size(O,2)
    if O(1,k)==O(2,k)
        L=[L,[O(1,k);O(2,k);k]];
    end
end

function[H] = histogramme(O) %Calcule le nombre de fois où apparaït chacune des valeurs dans un jeu%
H=zeros(1,7);
for k=0:6
    y=0;
    for i=1:size(O,2)
        if O(1,i)==k
            y=y+1;
        end
        if O(2,i)==k
            y=y+1;
        end
    end
    H(k+1)=y;
end

function[M] = maxpresence(O) %Renvoi le (ou les) nombre qui apparaît le plus souvent dans un jeu à partir de son histogramme%
M=[];
H=histogramme(O);
X=H;
for k=0:5
    if X(7-k)>X(7-k-1)
        X(7-k-1)=[];
    else X(7-k)=[];
    end
end
for k=1:7
    if H(k)==X(1)& X(1)~=0
       M=[M,k-1];
    end
end

function[P] = possibilites(O,p,q) %Donne, à partir de O, les choix possibles du joueur pour les "bouts" p et q%
P=[];
for k=1:size(O,2)
    if O(1,k)==p|O(1,k)==q|O(2,k)==p|O(2,k)==q
        P=[P,[O(1,k);O(2,k);k]];
    end
end

function[O,C] = distribue(n,D) %Créé deux jeux de n dominos au hasard%
[O,D]=tirehasard(n,D);
[C,D]=tirehasard(n,D);
O=tridecdominos(O);
C=tridecdominos(C);

function[J,O,C,tour] = quicommence(O,C) %Permet de savoir qui pose le premier domino : si O commence alors x=1, si c'est C alors x=2%
L=doublemoi(O);
K=doublemoi(C);
if size(L,2)~=0 & size(K,2)==0
    tour=2;
    J=[O(1,L(3,1)),O(2,L(3,1))]
    O(:,1)=[];
    O
elseif size(L,2)==0 & size(K,2)~=0
    tour=1;
    J=[C(1,K(3,1)),C(2,K(3,1))]
    C(:,1)=[];
    C
elseif size(L,2)~=0 & size(K,2)~=0
    tour=3-estavant(L(1:2,1),K(1:2,1));
    if tour==2;
        J=[O(1,L(3,1)),O(2,L(3,1))]
        O(:,L(3,1))=[];
        O
    else J=[C(1,K(3,1)),C(2,K(3,1))]
        C(:,K(3,1))=[];
        C
    end
else tour=3-estavant(O(:,1),C(:,1));
    if tour==2;
        J=[O(:,1)]
        O(:,1)=[];
        O
    else J=[C(:,1)]
        C(:,1)=[];
        C
    end
end

function[J,O,tour,passe] = joue(J,O,tour,passe) %Effectue le déroulement du tour de jeu d'un des 2 joueurs, en sortant comme paramètre les dominos sur la table et dans le jeu du joueur après avoir posé le domino, le numéro du joueur qui joue enseuite et le nombre de "passe" consécutifs%
P=possibilites(O,J(1),J(length(J)));
if length(P)==0
    passe=passe+1;
elseif tour==1
    [J,O]=strategieeffectif(J,O,P); %Stratégie de O%
    passe=0;
elseif tour==2
    [J,O]=strategieplusFort(J,O,P); %Stratégie de C%
    passe=0;
end
if tour==1
    tour=2;
else tour=1;
end

function[x] = point(O) %Calcule les points de pénalités du joueur O%
x=0;
for k=1:size(O,2)
        x=x+O(1,k)+O(2,k);
end

%Ci-dessous figurent les différentes stratégies que peuvent adopter les
%joueurs durant leurs tours de jeu%

function[J,O] = strategiepremier(J,O,P) %Choisi comme stratégie de prendre le premier domino dans l'ordre lexicographique%
if P(1,1)==J(1)
    J=[P(2,1),P(1,1),J]
elseif P(2,1)==J(1)
    J=[P(1,1),P(2,1),J]
elseif P(1,1)==J(length(J))
    J=[J,P(1,1),P(2,1)]
else J=[J,P(2,1),P(1,1)]
end
O(:,P(3,x))=[];

function[J,O] = strategiehasard(J,O,P) %Choisi comme stratégie de prendre le domino au hasard%
x=floor(size(P,2)*rand(1))+1;
if P(1,x)==J(1)
    J=[P(2,x),P(1,x),J]
elseif P(2,x)==J(1)
    J=[P(1,x),P(2,x),J]
elseif P(1,x)==J(length(J))
    J=[J,P(1,x),P(2,x)]
else J=[J,P(2,x),P(1,x)]
end
O(:,P(3,x))=[];

function[J,O] = strategieplusFort(J,O,P) %Choisi comme stratégie de prendre le domino valant le plus de point%
X=[P(:,1)];
for k=2:size(P,2)
    a=estplusfort([X(1:2,1)],[P(1:2,k)]);
    if a==2
        X=[P(:,k)];
    end
end
if X(1,1)==J(1)
    J=[X(2,1),X(1,1),J]
elseif X(2,1)==J(1)
    J=[X(1,1),P(2,1),J]
elseif X(1,1)==J(length(J))
    J=[J,X(1,1),X(2,1)]
else J=[J,X(2,1),X(1,1)]
end
O(:,X(3,1))=[];

function[J,O] = strategieeffectif(J,O,P) %Choisi comme stratégie de prendre le domino dont les valeurs sont les plus présentes dans son jeu%
H=histogramme(O);
X=[P(:,1)];
for k=2:size(P,2)
    if H(X(1,1)+1)+H(X(2,1)+1)<H(P(1,k)+1)+H(P(2,k)+1)
        X=[P(:,k)];
    end
end
if X(1,1)==J(1)
    J=[X(2,1),X(1,1),J]
elseif X(2,1)==J(1)
    J=[X(1,1),P(2,1),J]
elseif X(1,1)==J(length(J))
    J=[J,X(1,1),X(2,1)]
else J=[J,X(2,1),X(1,1)]
end
O(:,X(3,1))=[];

%FIN DU PROGRAMME%
