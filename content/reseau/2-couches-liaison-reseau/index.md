---
title: "Le modèle OSI - la couche physique, la couche liaison et la couche réseau"
date: 2018-09-05
categories: ["Réseau", "Cours"]
tags: ["physique", "cable", "fibre", "wifi"]
---

Les normes 801.## concerne en général la couche physique voire la couche liaison
voir la page [IEEE 802](https://fr.wikipedia.org/wiki/IEEE_802) sur Wikipedia.

# Les câbles

On parles souvent de câble RJ45 ou câble Ethernet, il est plus juste de parler
de câble à paires torsadées et fiches RJ45. Pourquoi les fils sont torsadés?
tout simplement pour minimiser les perturbations
[diaphoniques](https://fr.wikipedia.org/wiki/Diaphonie) induites par les fils
entre eux. Le tout est souvent enveloppé dans un blindage soit par
paire et/ou pour l'ensemble.

Voir la page des [paires
torsadées](https://fr.wikipedia.org/wiki/Paire_torsad%C3%A9e) sur Wikipedia

# La fibre optique

Le principe est de faire passer de la lumière dans un fil en utilisant les
propriété réfractrice de la lumière. La fibre est en fait composée, en son cœur,
de deux couches avec des indice de réfraction différent afin de contenir la
lumière en la faisant rebondir.

De par leurs longueur d'onde différentes, il est tout à fait possible d'utiliser
plusieurs couleurs de lumières afin d'envoyer plusieurs signaux en simultané.

On parle de *fibre noire* lorsque la fibre est livrée telle quelle, c'est au
client de "l'allumer" et fournissant les boitiers d'alimentation (émetteur et
récepteurs).

A contrario, la fibre allumée est fournie avec sa "lumière" c'est par contre le
prestataire (FAI par exemple) qui attribuera la couleur de la lumière.

Voir [la fibre optique](https://fr.wikipedia.org/wiki/Fibre_optique) sur
Wikipedia

# Le WiFi

Le WiFi, utilisé pour l'ensemble des normes 802.11, couvre un ensemble de
protocoles de communication sans fils utilisant les ondes radioélectriques en
utilisant des bandes de fréquences autour de 2,4 Ghz (normes a,b,g,n) et
5 Ghz(n,ac)

La norme découpe des canaux dans les bandes définie (14 utilisables dont 11 en
France pour celle 2,4 Ghz et 25 utilisable pour 5 Ghz)

On trouve deux type d'antennes pour le WiFi :

 - **directionnelle** : le signal est envoyé dans une direction en particulier,
     qui permet de diminuer l'atténuation et d'augmenter la portée.
 - **omnidirectionnelle** : le signal est envoyé dans toute les directions, ce
     qui diminue la portée.

Voir la page du [WiFi](https://fr.wikipedia.org/wiki/Wi-Fi) sur Wikipédia.

## Le théorème de Shannon

Voir [la
page](https://fr.wikipedia.org/wiki/Th%C3%A9or%C3%A8me_d%27%C3%A9chantillonnage)
Wikipedia.

# Les adresses

Quel que soit le type de réseau, il y a forcément une notion d'adresse
sous-jacente. Pou Internet, réseau de réseaux, on dénombre trois type d'adresses

## Adresse MAC

Inscrite directement dans le matériel (carte réseau ethernet, wifi ...). Cette
adresse est utilisée localement (réseau interne) est codée sur 48bits  en
hexadécimal : 24 étant spécifique au constructeur et 24 au matériel. Deux
périphériques peuvent avoir la même adresse MAC, mais le fabriquant se
débrouillera pour les envoyer à des coins opposés du monde pour ne pas qu'ils se
retrouve sur le même réseau local.

## Adresse IP

C'est une adresse virtuelle, dans le sens ou elle n'est pas inscrite en dur dans
le matériel comme l'adresse MAC. Elle est globale, routable et doit être unique.
Il en existe deux type IPv4 et IPv6

### IPv4

C'est le type d'adresse le plus ancien, une adresse IPv4 se compose de 4 valeurs
décimales séparées par des points (32 bits). Il est donc possible d'en attribuer
environ 4 milliards. C'est aujourd'hui une ressource rare épuisée dans certaines
partie du globe (États-Unis par exemple).

### IPv6

C'est le type d'adresse le plus récent, il a été créé pour pallier à la pénurie
d'IPv4. Une adresse IPv6 est composée de 128 bits en hexadécimal regroupés en
paquet de 16 bits séparés par des deux-points (:).

Elles sont plus facilement structurées, prenons l'exemple de la machine 4 de la
salle 101 du CREMI, son adresse est

```
2001:660:6101:800:252:4
```

 - `2001:660` correspond au réseau
     [RENATER](https://fr.wikipedia.org/wiki/R%C3%A9seau_national_de_t%C3%A9l%C3%A9communications_pour_la_technologie,_l%27enseignement_et_la_recherche0
 - `:6101` c'est la région (Aquitaine - Bordeaux)
 - `:800` c'est le bâtiment du CREMI
 - `:252` c'est le numéro de salle
 - `:4` c'est le poste.

On voit ici que le découpage de l'adresse IPv6 la rend lisible.

## Le masque de sous réseau

Il permet de distinguer la partie utilisée pour le routage de celle utilisée
pour numéroter les interface d'une adresse IP.

En IPv4, le masque de sous réseau distingue les bits d'une adresse IP
identifiant le sous-réseau et ceux identifiant l'hôte.

En IPv6, le masque est appelé préfixe et est un multiple de 4 afin de découper
des morceaux entiers d'adresse pour les sous réseau, facilitant ainsi la vie de
tout le monde.
