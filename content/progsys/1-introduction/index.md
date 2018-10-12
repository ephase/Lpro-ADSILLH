---
title: "Programmation Système : Appels systèmes"
date: 2018-09-04
tags: ["programmation", "système", "appels système"]
categories: ["Programmation système", "Cours"]
---

# Qu'est ce que c'est?

C'est créer des programmes plutôt bas niveau qui s'interfacent avec le noyau
et/ou les librairies du système. Il faut donc une bonne connaissance du
fonctionnement du système et être en mesure d'identifier d'éventuels problèmes
de performance.

# Le système d'exploitation

C'est un ensemble de logiciels fournissant les services nécessaires à
l'exécution de programmes pour l'utilisateur final :

 - exécution de programmes
 - accès aux matériels
 - accès au(x) système(s) de fichier(s)
 - accès  mémoire
 - accès réseau

## POSIX et IEE

La norme POSIX, standard IEEE, définie les interfaces utilisateurs et
logiciels, la ligne de commande standard et son interface de scripts, les
entrées / sorties de base. les attributs de fichiers (permissions)

La norme SUS *Single Unie Specification* est une norme plus récente et moins
contraignante. elle est basée sur les travaux de l'IEEE et de l'Open Group.

[SUS][https://fr.wikipedia.org/wiki/Single_UNIX_Specification] et
[POSIX][https://fr.wikipedia.org/wiki/POSIX] sur Wikipedia

## LSB

LSB ou *Linux Standard Base* définie l'agencement général du système GNU / Linux
: librairies standard, commandes POSIX étendues, hiérarchie du système de
fichiers, les runlevels etc.

[LSB][https://fr.wikipedia.org/wiki/Linux_Standard_Base] sur Wikipedia.

## Architecture des systèmes "modernes"

Ils sont organisés en couches un peu à la façon du modèle OSI : 

 1. le matériel
 2. l'assemblage (assembleur)
 3. système (applications système)
 4. Application
 N. Les applications / machines virtuelles (capables de traverser les couches
 pour atteindre le matériel)

 Plus la couche est basse, plus la programmation est efficace du point de vue
 des performances mais plus difficile est la portabilité (un code assembleurs et
 moins portable qu'un script Python)

 Les systèmes modernes possèdent deux espaces d'exécutions : 

  - l'espace utilisateur `user-space` ou `user-land`
  - l'espace noyau `kernel-space` ou `kernel-land`

# La libc

La `libc` est la librairie standard des système GNU/ Linux, elle implémente
les fonctionnalités de base pour la plupart des programmes. C'est une API pour
accéder aux services du noyau :

 - entrées /sorties
 - allocation fine de la mémoire
 - gestion du temps
 - encapsulation des appels noyau et en dehors du noyau (user-land)

# Programme et processus

Un programme est un fichier exécutable stocké sur un disque (mémoire morte). Un
processus et un programme mais en mémoire (et donc en cours d'exécution). Un
processus est défini par son identifiant (PID) et accède à un espace d'adressage
en mémoire vive (pour la pile, les données ...)

# Descripteurs de fichiers

Un descripteur de fichier pour *file descriptor* (abrégé *fd*) est un entier
positif affecté par le noyau pour référencer un fichier utilisé par un
processus. Il est bon de préciser que dans la philosophie Unix, tout est
fichier, le *fd* se retrouve donc pour les fichiers, sockets, tubes,
périphériques etc.

# Les appels système

Ils permettent de demander au noyau de réaliser une action précise et se font au
travers de la libc. Voici comment cela se passe : 

 - Préparer les arguments en les positionnant dans des registres spécifiques
   pour le noyau
 - placer le numéro d'appel système dans un registre spécifique invoquer
 - l'instruction `trap` qui bascule le processeur en mode noyau

Le noyau invoque `system_call` après validation des arguments et retourne le
résultat à la libc :

 - en cas d'erreur la libc invoque `errno` avec en résultat le code erreur
 - sinon le résultat
