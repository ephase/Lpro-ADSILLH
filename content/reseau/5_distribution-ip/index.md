---
title : " Distribution d'adresses IP"
date : 2018-10-08
categories : ["Réseau", "cours"]
tags : ["IP", "IPv4", "IPv6", "DHCP"]
----

## Distribuer des adresses IP dans un réseau

Dans le cadre d'un réseau informmatique, il existe trois manières de distribuer
des adresse IP :

 * une approche statique
 * une approche dynamique
 * une approche automatique

### L'approche statique

On ne s'attardera pas ici sur l'approche statique qui consiste en l'assignation
et la confitugation manuelle de la couche IP sur l'ensemble des postes.

### L'approche dynamique (DHCP)

Un serveur fourni les adresses IP aux différents clients avec une durée de vie
définie (en général de 24h). Le protocole DHCP permet de fournir d'autres
information comme les serveurs DNS, le nom d'hôte, les serveurs d'impression, de
temps etc.

###L'approche automatique

Les machines du réseau se "mettent d'accord". C'est un fonctionnement naturel
en IPv6 "backporté" en IPv4. Les adresse IP attribués sont dans la plage
169.254.0.0/16 et fe80::0/64. En IPv6, les routeurs peuvent faire du *Router
Advertisement* : chaque routeur averti les machines connectées du réseau qu'il
gère, par exemple 2001:911:4::/64.

## L'attribution des adresse IP

C'est l'IANA *Internet Assigned Numbers Authority* aui qssigne les adresse IP
publiques. L'IANA alloue des blocs IP au [RIR *Registre Internet
Régionaux*][l_lir] qui les découpent et les attibuent aux LIR *Local Internet
Regitery* qui les rnds disponibles aux utilisateurs finaux.

Les attributions sont enregistrées dans la base de donnée [*Whois*][l_whois]

En IPv4, les adresses sont en passe d'être épuisée. Il n'y en a plus de
disponible aux États-Unis par exemple. De par le fait un marché noir de l'IPv4
se dévellope.

[l_lir]:https://fr.wikipedia.org/wiki/Registre_Internet_r%C3%A9gional
[l_whois]:https://fr.wikipedia.org/wiki/Whois
