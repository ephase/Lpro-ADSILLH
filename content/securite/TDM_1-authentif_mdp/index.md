---
title: "TDM : Authentification et mots de passes"
date: 2019-01-09
categories: ['Sécurité', 'TD machine']
tags : ['hash', 'MD5', 'mot de passe', 'kali']
---

Lors de ce TD nous utiliserons principalement **john the ripper**, un outil de
cassage de mots de passe. C'est un logiciel opensource (licence GPL V.2)
connaissant de base plusieurs algorithmes de hachages (MD4, MD5, SHA256,
Blowfish, NTLM etc). Il est de plus extensible par des plugins.

Il est disponible sur beaucoup de plates-formes (GNU/Linux, *BSD, Windows, BeOS
etc.). Pour l'installer sur Debian ( et ses dérivés comme Ubuntu) :

```
apt install john
```

## Utiliser john

John permet de fonctionner dans 3 modes différents : single, par dictionnaire et
incrémentale

**Single** : dans ce mode, `john` utilise le *hash* du login et éventuellement
des variantes de celui-ci.

**Dictionnaire** : `john` essaye un à un les mos de passes d'une liste établie

**Incrémentale** : aussi appelé *Bruteforce*, dans ce mode, `john` essaye un
ensemble de combinaisons composé d'une suite de caractères donnés.

### Le mode single

C'est le mode le plus rapide du logiciel, pour tester notre fichier de mot de
passe dans ce mode il suffit de faire : 

```
john -single -format=raw_MD5 logins.txt
```

Voici la sortie de la commande :

```
Created directory: /root/.john
Using default input encoding: UTF-8
Loaded 8 password hashes with no different salts (Raw-MD5 [MD5 256/256 AVX2 8x3])
Press 'q' or Ctrl-C to abort, almost any other key for status
AndyGollum       (AndyGollum)
Warning: Only 18 candidates buffered for the current salt, minimum 24
needed for performance.
Almost done: Processing the remaining buffered candidate passwords, if any
Warning: Only 4 candidates buffered for the current salt, minimum 24
needed for performance.
1g 0:00:00:08 DONE (2019-01-09 23:14) 0.1142g/s 718.4p/s 718.4c/s 5031C/s indianaford1900..brucemclane1900
Use the --show --format=Raw-MD5 options to display all of the cracked passwords reliably
Session completed
```

`john` sauvegarde automatiquement **tous** les *hashes* qu'il retrouve dans sa
base de donnée qu'il stocke dans le fichier `~/.john/john.pot`. Il écrit aussi
un fichier journal afin de reprendre les opérations là où il les a arrêtées
(`~/.john/john.log`).

Il est possible de retrouver les mots de passes que `john` a trouvé avec la
commande :

```
john -show -format=raw_MD5 logins.txt
```

Et le résultat :

```
Using default input encoding: UTF-8
Loaded 8 password hashes with no different salts (Raw-MD5 [MD5 256/256 AVX2 8x3])
Remaining 7 password hashes with no different salts
Proceeding with single, rules:Wordlist
Press 'q' or Ctrl-C to abort, almost any other key for status
Warning: Only 15 candidates buffered for the current salt, minimum 24
needed for performance.
0g 0:00:00:01  1/3 (2019-01-09 23:40) 0g/s 549.0p/s 549.0c/s 3843C/s Indianaforded..maximuscr0we
Almost done: Processing the remaining buffered candidate passwords, if any
Warning: Only 4 candidates buffered for the current salt, minimum 24
needed for performance.
Proceeding with wordlist:/usr/share/john/password.lst, rules:Wordlist
Proceeding with incremental:ASCII
pwnetp           (SylvesterBalboa)
avnrflr          (bt1)
2g 0:00:51:49  3/3 (2019-01-10 00:32) 0.000643g/s 4060Kp/s 4060Kc/s 23260KC/s aslepred2..aslepruma
```

### La recherche incrémentale

`john` va tester un ensemble de combinaisons, elle est communément appelée
*bruteforce*. Comme pour les précédentes commandes, il est **nécessaire** de
spécifier la fonction de hashage via le paramètre `-format=<hash>` :

```
john -format=raw_MD5 logins.txt
```


Cette commande est bien plus longue que la précédente, les combinaisons à tester
étant très nombreuses. Voici le résultat : 

```
Using default input encoding: UTF-8
Loaded 8 password hashes with no different salts (Raw-MD5 [MD5 256/256 AVX2 8x3])
Remaining 7 password hashes with no different salts
Proceeding with single, rules:Wordlist
Press 'q' or Ctrl-C to abort, almost any other key for status
Warning: Only 15 candidates buffered for the current salt, minimum 24
needed for performance.
0g 0:00:00:01  1/3 (2019-01-09 23:40) 0g/s 549.0p/s 549.0c/s 3843C/s Indianaforded..maximuscr0we
Almost done: Processing the remaining buffered candidate passwords, if any
Warning: Only 4 candidates buffered for the current salt, minimum 24
needed for performance.
Proceeding with wordlist:/usr/share/john/password.lst, rules:Wordlist
Proceeding with incremental:ASCII
pwnetp           (SylvesterBalboa)
avnrflr          (bt1)
2g 0:00:51:49  3/3 (2019-01-10 00:32) 0.000643g/s 4060Kp/s 4060Kc/s 23260KC/s aslepred2..aslepruma
```

La commande a été interrompue, mais a trouvé deux mots de passe. Il est possible
de la lancer en changeant le format par `raw-SHA256`.

### les dictionnaires

Plus rapide que la technique précédente, celle-ci est basée sur une liste
définie de mots de passe> Voici la commande :

```
john -wordlist=top500movies.txt -format=raw_MD5 logins.txt
```

Et le résultat : 

```
Using default input encoding: UTF-8
Loaded 8 password hashes with no different salts (Raw-MD5 [MD5 256/256 AVX2 8x3])
Remaining 5 password hashes with no different salts
Press 'q' or Ctrl-C to abort, almost any other key for status
Le silence des agneaux (MaximusCrowe)
1g 0:00:00:00 DONE (2019-01-10 00:34) 25.00g/s 13500p/s 13500c/s 63600C/s The King of New York..Indiana Jones et le temple maudit
Use the --show --format=Raw-MD5 options to display all of the cracked passwords reliably
Session completed
```

## Utilisation avancée de john

Nous avons vu ici trois utilisations de `john the ripper`, mais grâce à son
fichier de configuration situé dans le répertoire `/etc/john/john.conf`. En plus
des paramètres de bases du logiciel, il est possible de définit des règles plus
fine pour la recherche.

Ces règle sont définie dans des sections commençant par 

```
[List.Rules.<name>]
```

Où `<name>` est la nom donné à notre liste. Suive ensuite un ensemble de règle
étendant les recherche de bases. 

### Premier exemple : étendre notre dictionnaire

Il est courant qu'un utilisateur choisisse un mot courant, puis ajoute son année 
de naissance puis un caractère spécial à la fin, composant ainsi un mot de passe
correspondant à la politique de sécurité de son organisation.

Nous allons donc étendre notre liste de mots en prenant en compte ceci. Parton
du principe que les personnes de l'organisation on moins de 40 ans, voici la
règle créée :

```
[List.Rules:MaListe]
$1$9$[7-9]$[0-9]$[.,;:@-_!#]
```

Le caractère `$` remplace un caractère et `[]` est là pour définir une plage de
caractères.

Pour l'appliquer modifions notre dernière commande `john` comme ceci :

```
john -format=raw_MD5 -wordlist=top500movies.txt -rules=MaListe logins.txt
```

```
Using default input encoding: UTF-8
Loaded 8 password hashes with no different salts (Raw-MD5 [MD5 256/256 AVX2 8x3])
Remaining 4 password hashes with no different salts
Press 'q' or Ctrl-C to abort, almost any other key for status
Pulp Fiction1989! (IndianaFord)
Use the --show --format=Raw-MD5 options to display all of the cracked passwords reliably
Session completed
```

### Second exemple : modifier la recherche

Prenons le cas d'un utilisateur qui aurait changé les voyelles d'un mot par des
chiffres pour en faire un mot de passe. Il est possible via des règles de tester
ce cas de figure.

```
[List.Rules:Voyelles]
s?v[0-9]
```

 - `s` : remplacer
 - `?v` : rechercher la classe v : les voyelles
 - `[0-9]` : remplacer par la classe d : un chiffre

Nous pouvons maintenant lancer la recherche avec la commande

```
john -wordlist=top500movies.txt -rules=Voyelles logins.txt
```

Et voilà la sortie :

```
Using default input encoding: UTF-8
Loaded 8 password hashes with no different salts (Raw-MD5 [MD5 256/256 AVX2 8x3])
Remaining 3 password hashes with no different salts
Press 'q' or Ctrl-C to abort, almost any other key for status
Sl1md1g M1ll11n11r1 (KevinSoze)
1g 0:00:00:00 DONE (2019-01-10 01:40) 16.66g/s 90000p/s 90000c/s 192800C/s Th9 Pl9y9r..9nd99n9 J9n9s 9t l9 t9mpl9 m99d9t
Use the --show --format=Raw-MD5 options to display all of the cracked passwords reliably
Session completed
```

## Bibliographie

 - [Site officiel](https://www.openwall.com/john/) de John the Ripper
 - [Page Wikipedia](https://fr.wikipedia.org/wiki/John_the_Ripper)
