---
title: TDM1 OpenLDAP - notes d'installation
date: 2018-09-13
lastmod: 2018-08-27
categorie: ["Installations", "TD machines"]
tags: ['OpenLDAP', 'Apache Directory Studio', 'PAM']
---

Le but de ce TD Machine est d'installer OpenLDAP sur une machine virtuelle, de
configurer un DIT en `dc=u-bordeaux,dc=fr`, de gérer les personnes, matériels et
fonctions et enfin de gérer l'authentification par le LDAP.

# Installation  d'openLDAP sur Debian

Lors de l'installation, j'ai choisi de nommer la machine `openldap` et le
`domaine u-bordeaux.fr`, ainsi lors du paramétrage de mon service, il sera
accessible directement sur `openldap.u-bordeaux.fr`

Sur une Debian 9 il suffit d'installer les paquets `slapd` et `ldap-utils` avec
la commande :

```shell
apt install slapd ldap-utils
```

Il est possible de définir simplement le domaine et le mot de passe de
l'administrateur du serveur LDAP en reconfigurant le paquet avec la commande :

```shell
dpkg-reconfigure -pHIGH slapd
```

Il est aussi possible de le faire directement par l'intégration de fichiers LDIF
dans l'annuaire.

# Intégration des personnes, fonctions et matériels

Nous allons créer 3 objets de type `dmd` *directory management domain* reprenant
les trois catégories demandées. Lors de la création de mon arbre, j'ai cependant
choisi cd créer des `ou` *organizational unit* mais c'est sémantiquement pas
juste (ce serait valable pour le service marketing ou le site de Pessac par
example).

À partir du moment ou le serveur LDAP est lancé, fonctionnel et l'administrateur
créé, il est tout à fait possible de lancer cette étape depuis un logiciel comme
Apache Directory studio ou phpldapmyadmin. Mais le but ici était de le faire
depuis un fichier LDIF.

```LDIF
dn: dmdName=users,dc=u-bordeaux,dc=fr
dmdName: users
objectClass: dMD

dn: dmdName=roles,dc=u-bordeaux,dc=fr
dmdName: fonctions
objectClass: dMD

dn: dmdName=machines,dc=u-bordeaux,dc=fr
dmdName: machines
objectClass: dMD
```

Il suffit d'intégrer ce fichier à l'annuaire avec la commande `ldapadd` :

```shell
ldapadd -H ldap://openldap.u-bordeaux.fr -D "cn=admin,dc=u-bordeaux,dc=fr" \
    -W -f tree.ldif
```

 - `-H <uri>` : URI d'accès au serveur LDAP
 - `-D <dn>` : dn du compte autorisé pour l'ajout (ici l'admin) 
 - `-W` : demande le mot de passe
 - `-f <path>` : fichier LDIF contenant les entrées à ajouter.

## Effectuer une recherche dans l'annuaire

La recherche dans l'annuaire est importante dans la mesure ou elle va nous
permettre de vérifier au fur et à mesure les données que l'on y ajoute.

```shell
ldapsearch -x -W -D "cn=admin,dc=u-bordeaux,dc=fr" -b "dc=u-bordeaux,dc=fr" \
    "(objectClass=dMD)"
```
  - `-W` : demande le mot de passe
  - `-D` : dn de l'admiistrateur du serveur
  - `-b` : chemin ou effectuer la rechercheo
  - `-x` : uriliser l'anthentification simple plutôt que SASL **déconseillé**
  - `(*)` : recherche

## Ajouter un mot de passe 

```shell
ldappasswd -H "ldap://ldapsrv.u-bordeaux.fr" -D "cn=admin,dc=u-bordeaux,dc=fr" \
    -W "uid=yorick.barbanneau,dmdName=users,dc=u-bordeaux,dc=fr"
```

  - `-H` : host du serveur LDAP à contacter

Les autres options sont similaires aux comances ci-dessus.

# Apache Directory Studio

C'est un logiciel permettant la gestion complète d'un annuaire LDAP. Écrit en
java, c'est une application open-source sous licence Apache 2.

Pour l'installer, il faut disposer d'une interface graphique et d'un
environnement d'exécution Java. Pour le TD, j'ai donc installé le bureau `Lxde` et
`openjdk-8-jre`.

```shell
apt install lxde openjdk-8-jre
```

j'ai ensuite téléchargé la dernière version d'Apache Directory Studio sur [le
site Apache](https://directory.apache.org/studio/download/download-linux.html).

Une fois le logiciel téléchargé et décompressé, je l'ai lancé puis ajoute la
connexion au serveur LDAP dans *LDAP > nouvelle connexion*.

J'ai créé un groupe enfant de `dmdName=fonction,dc=u-bordeaux,dc=fr` avec comme `cn`
users et un `gidNumber` de 2000

J'ai ensuite créé un utilisateur enfant de `dmdName=personnes,dc=u-bordeaux,dc=fr`
avec comme classe `OrganizationalPerson` et `PosixAccount`. J'ai fait choisi
un `uidMumber` suffisamment haut pour ne pas gêner ceux du système (2000)

On pourrait le modéliser dans le fichier LDIF suivant (à Corriger):

```LDIF
dn: uid=yorick.barbanneau,dmdName=users,dc=u-bordeaux,dc=fr
objectClass: top
objectClass: person
objectClass: organizationalPerson
objectClass: posixAccount
objectClass: shadowAccount
uid: yorick.barbanneau
sn: Barbanneau
cn: Yorick Barbanneau
userpassword: {SSHA}......
uidnumber: 2000
gidnumber: 2000
homedirectory: /home/yorick.barbanneau
loginshell: /bin/bash
```

# Authentification sur la machine avec les comptes LDAP

L'authentification machine depuis les données de l'annuaire nécessite
l'installation et la configuration de deux paquets Debian :

```shell
apt install libpam-ldap libnss-ldap
```

L'installation entraine ici automatiquement le lancement de la configuration de
certains éléments via debconf. il reste cependant des choses à configurer. 

Tout d'abord le fichier `/etc/nsswitch.conf` :

```
passwd:         files ldap
group:          files ldap
shadow:         files ldap
```

Ensuite il faut modifier le fichier `/etc/libnss-ldap.conf` et y modifier les champs
`base` et `uri` en fonction de notre serveur

```
uri ldap://openldap.u-bordeaux.fr
base dc=u-bordeaux,dc=fr
```

et redémarrer le service nscd :

```shell
systemctl restart nscd
```
Enfin, il reste à configurer PAM afin qu'il créé le dossier utilisateur s'il
n'existe pas. Dans le fichier `/etc/pam.d/common-account` rajouter la ligne : 

```
session         required        pam_mkhomedir.so skel=/etc/skel umask=0022
```

Pour vérifier que tout fonctionne, lancer `getent passwd`, les utilisateurs
venant du LDAP devraient apparaître dans la liste obtenue.

# Voir aussi

 - [LDAP PAM sur la documentation Debian](https://wiki.debian.org/LDAP/PAM)
 - [LDAP Authentication (Archlinux)](https://wiki.archlinux.org/index.php/LDAP_authentication)
 - [SSSD](https://docs.pagure.org/SSSD.sssd/)

## SSSD

C'est une méthode d'authentification couplée à PAM qui semble intéressante : elle
supporte la connexion même en mode hors-ligne grâce à un cache.
