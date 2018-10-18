---
title: "Installation : Postfix + LDAP"
categories: ["Installation", "TD machine"]
tags: ["Postfix", "LDAP", "maildir"]
date: 2018-10-4
modify: 2018-10-18
---

Le but de ce TD machine est d'installer et de configurer postfix et de le
coupler ensuite avec notre annuaire LDAP pour lagestion des comptes.

## Installation de Postfix

L'installation sur debian est simple, il suffit d'installer le paquet adéquat et
de répondre aux deux questions posée par dpkg : 

```shell
# apt install postfix
```

### Configuration 

La configuration de postfix se fait dans le répertoire `/etc/postfix`. Le
premier fichier à modifier est `main.cf`, on y configurera entre autre les
domaines concernés par notre serveur SMTP.

### test de fonctionnement

Les premiers tests d'envoi se font en telnet via le port 25

```shell
# telnet localhost 25
Connected to localhost.
Escape character is '^]'.
220 postfixsrv.u-bordeaux.fr ESMTP Postfix (Debian/GNU)
mail from: jcelineau@u-bordeaux.fr
rcpt to:tech@localhost
data
To:ybarbanneau@u-bordeaux.fr
From: jcelineau@u-bordeaux.fr
Subject: Essai de notre serveur SMTP
Ceci est un test d'envoi de courriel via notre serveur SMTP fraîchement
installé

.
250 2.0.0 Ok: queued as 0F66332
```

Pour l'instant notre serveur postfix accepte seulement de recevoir des courriel
pour le serveur (ici localhost).

## Coupage avec LDAP

Postfix peut aller n'importe quelle information utiles (boites, boites
virtuelles, domaines etc.) dans l'annuaire. Il suffira seulement de lui indiquer
ou chercher via des fichiers de configurations officiant comme "requêtes"

### "requête" LDAP pour retrouver les utilisateurs

Je créé un fichier `ldap_users.cf` dans le répertoire `/etc/postfix`. ce fichier
permettra à LDAP d'aller chercher les utilisateurs dans l'annuaire LDAP. C'est
en quelque sorte la requête pour aller checrher les informations dans
l'annuaire.

```conf
server_host = ldap://ldapsrv.u-bordeaux.fr
search_base = dmdName=users,dc=u-bordeaux,dc=fr
bind = yes
bind_dn = cn=admin,dc=u-bordeaux,dc=fr
bind_pw = test
query_filter = (&(objectClass=organizationalPerson)(uid=%s))
result_attribute = uid
result_format = %s/
```

Le slash à la fin de notre chaine de retour permet de forcer postfix à utiliser
le format de stockage [maildir][l_maildir].

[l_maildir](https://fr.wikipedia.org/wiki/Maildir)

#### Tester le retour de postfix

La commande `postmap` nous postfix dialogue bien avec l'annuaire, pour vérifier
les utilisateurs on va utiliser le fichier créé ci-dessus avec la commande :

```shell
# postman -q yorick.barbanneau@u-bordeaux.fr ldap:/etc/postfix/ldap_users.cf
```
