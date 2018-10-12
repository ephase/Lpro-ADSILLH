---
date: "2018-09-03"
categories: ["Réseau", "Cours"]
title: "Réseaux - Introduction"
tags: ["OSI", "latence", "débit"]
---

Le principe de ce cours est de permettre de savoir configurer et gérer un réseau
d'entreprise.

# La pile OSI (vue rapide)

elle se découpe en 7 couches a connaitre par cœur.

 1. Physique : les bits *câble, ondes, fibre, ADSL ...*
 2. Liaison : on parle de **trames** *Ethernet, WiFi*
 3. Réseau : acheminement - **paquets** *IP, IPX*
 4. Transport : accusé de réception, port, **datagramme** *TCP, UDP, IPX*
 5. Session : qui? quoi? *CAS (SSO)*
 6. Présentation : encodage, langue *UTF-8, XML*
 7. Application : tout le reste.

Voir schema sur l'encapsulation

# Latence et débit

Voir schema Page 3

**Débit** : Quantité d'information que l'on est capable d'envoyer / recevoir
dans un certain laps de temps.

**Latence** : Temps entre le moment ou j'envoie mon premier bit est envoyé et où
il est reçu. La latence est indépendante du débit. La latence étant difficile à
mesurer, il est commun d'utiliser plutôt le temps d'un aller retour.

Les deux notions ne sont pas liées même s'il est possible que l'une impacte
l'autre. On considère que l'expérience utilisateur n'est plus fluide à partir de
10ms de latence et n'est plus "interactive" à partir de 100ms.

## Exercice

Calculer le débit nécessaire pour envoyer une vidéo de 640x360 à 25hz et 24
bits/pixels et un fichier audio 16 bits à 48 Khz

### fichier vidéo

Une image  de notre fichier vidéo se compose donc de **230400** pixels et donc
pèse **5529**Kb *(5529600 bits)*. Pour 24 images il faut un débit de
**138240**Kb/s

### fichier audio

Une seconde de flux audio représentant 48000 valeurs de 16 bits le débit
nécessaire est donc de **768**Kb/s. Je suis parti du principe que le fichier
audio en question est mono.

## Correction

Prendre des approximations, ne pas s'embêter des détails.

Pour comparaison

 - Fichier vidéo h264 400-100 Kb/s
 - Fichier audio 128 Kb/s
