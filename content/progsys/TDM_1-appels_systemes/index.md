---
title: "TDM : Les appels système"
date: 2018-09-05
categories: ['Programmation système', 'TD machine']
tags : ['appels', 'systèmes', 'Python', 'C']
---

#Différence entre les strace C et Python

Le strace python est plus volumineux, ce aui semble normal puisque le code est
interprété, et charge par conséquent l'ensemble de l'environnement d'execution
Python. *(36 lignes pour le C contre 613 pour le Python)*.

Cependant il y a aussi beaucoup des similitudes, comme l'appel à la libc et les
commamdes envoyées au kernel (write).

On peut donc en déduire que le temps d'exécution pour la version Python est plus
grande avec plus d'appels système.

## temps d'execution comparé avec strace -tt

 - C : début 17:52:29.647826 fin 17:52:29.668778 
 - Python : début 17:50:49.455004 fin 17:50:49.769447

La différence de temps d'execution apparaît clairement ici

# Tracer un programme en cours d'exécution

J'ai essayé de tracer htop lancé dans un autre terminal mais j'ai une erreur
(Opération non permise). En passant par root pour lancer mon strace, il
fonctionne. L'affichage du strace est continue logique puisque le programme
tournant en arrière plan, il continue de faire des appels systèmes (surveillance
des processus du sytèmes)

Il est dont logique que je ne puisse pas stracer le processus 0, même apres
avoir modifier la variable sysctl yama.

# Avec le sleep

On constate un appel système de plus (nanosleep) avant le write.
