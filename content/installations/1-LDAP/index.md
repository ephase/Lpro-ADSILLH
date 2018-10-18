---
title: Les annuaires LDAP
date: 2018-09-06
tags: ["annuaire", "LDAP", "LDIG"]
categories: ["Installation", "Cours"]
---

**LDAP** pour *Lightweight Directory Access Protocol* est à l'origine un
protocole utilisé pour la modification et la consultation de services
d'annuaire. Il a évolué pour devenir une norme pour les services d'annuaire
incluant un modèle de données, un modèle de nommage, un modèle fonctionnel, un
modèle de sécurité et un modèle de réplication.

Dérivé du protocole x500, réputé "usine à gaz" complexe à mettre en place et
peu performant, LDAP se veux, comme son nom l'indique plus léger, il est
standardisé par l'IETF. La version actuelle est la LDAPv3 définie dans plusieurs
RFC en commençant par la RFC 4510

# Les modèles du LDAPv3

Ils sont au nombre de 4 : 

 - **le modèle d'information** qui reprend entres autres le schéma, les OID, les
     attributs, les classes d'objets
 - **le modèle de désignation** qui s'attache au nommage des entrées, à la
     hiérarchisation des données, au lien LDAP - DNS
 - **le modèle de services** qui décrit les services offerts par le service
     d'annuaire
 - **le modèle de sécurité** qui défini l'authentification, la confidentialité,
     l'intégrité et la gestion des habilitations.

# Le modèle d'information

Il décrit principalement le schéma de l'annuaire : les *attributs*, les
*syntaxes*, les *classes d'objets* et les *règles de recherches*.

## les attributs

Se sont les éléments de base du schéma décrit comme suit : 

```
attributetype (
    <OID>                # identifiant OID unique
    NAME <nom>           # nom de l'attribut
    DESC <desc>          # description de l'attribut
    OBSOLETE             # indique aue l'attribut est obsolète
    SUP <classe>         # classe parente de l'attribut
    EQUALITY <règle>     # nom de la règle de recherche
    ORDERING <règle>     # nom de la règle d'ordenancement
    SUBSTR <règle>       # nom de la règle de recherche dabs une sous-chaine
                         # de caractère
    SYNTAX <syntax>      # syntaxe de l'attribut
    SINGLE-VALUE         # indique d'une seule valeur est attendue
    COLLECTIVE           # indique que le schéma est collectif
    NO-USER-MODIFICATION # indique aue l'utilisateur ne peut modifier l'attribut
    USAGE                # usage de l'attribut.
)
```

Voici un exemple concret :

```
attributetype ( 
    2.5.4.16 
    NAME 'postalAddress'
    DESC 'RFC2256: postal address'
    EQUALITY caseIgnoreListMatch
    SUBSTR caseIgnoreListSubstringsMatch
    SYNTAX 1.3.6.1.4.1.1466.115.121.1.41
)
```

## Les attributs

On distingue les **attributs utilisateurs** qui caractérisent l'objet manipulé
par les utilisateurs des **attributs opérationnels** manipulés seulement par le
serveur afin de modifier les données.

### Les OID - Object IDentifiers

Ce sont des identifiants universels, représentés par une suite d'entiers séparés
par des points. Il sont organisés sous forme hiérarchique ainsi seul l'organisme
1.2.3 peur donner la signification de 1.2.3.4.

Leur objectif est d'assurer l'interopérabilité entre différents logiciels et son
utilisés dans le LDAP, mais aussi le SNMP.

Voir la [liste des OID](https://ldap.com/ldap-oid-reference-guide/)(en). Il est
possible d'utiliser [ce site](http://oid-info.com/get/)(en) pour faire une
recherche par OID.

## Les syntaxes

Elles permettent d'indiquer la règle à suivre pour enseigner l'attribut. Si, par
exemple, l'attribut suit une syntaxe dn et l'utilisateur saisit une chaîne de
caractère lambda, l'attribut ne pourra être renseigné.

## Les classes d'objets

Ils représentent une collection d'attributs . On peut y définir, par exemple, ce
qui est obligatoire ou facultatif. Voici une description du format : 

```
ObjectClassDescription (
    <OID>           # OID de la classe
    NAME <nom>      # nom de la classe
    DESC <desc>     # description de la classe
    OBSOLETE        # indique que la classe est obsolete 
    SUP <classe>    # indique la classe parente
    ABSTRACT        # Les 3 éléments suivant sont au choix ( voir l'explication
    STUCTURAL       # ci-dessous
    AUXILIARY
    MUST ()         # Liste des attributs obligatoires
    MAY  ()         # liste des attributs facultatifs
)
```

Voici un exemple concret avec les classes `person` et `organisationnalPerson`: 

```
( 2.5.6.6
    NAME 'person'
    SUP top
    STRUCTURAL
    MUST ( sn $ cn )
    MAY ( userPassword $ telephoneNumber $ seeAlso $
    description ) 
)
( 2.5.6.7
    NAME 'organizationalPerson'
    SUP person
    STRUCTURAL
    MAY ( title $ x121Address $ registeredAddress $
        destinationIndicator $ preferredDeliveryMethod $
    telexNumber $ teletexTerminalIdentifier $
    telephoneNumber $ internationaliSDNNumber $
    facsimileTelephoneNumber $ street $ postOfficeBox $
    postalCode $ postalAddress $
    physicalDeliveryOfficeName $ ou $ st $ l ) 
 )
```

### Les types de classes d`objets

Il en existe trois :

 - **abstraites** : ce sont des classe de plus haut niveau non instanciable mais
     pouvant être dérivées. la classe d'objets la plus haute étant `top`  dont
     toute classe dérive
 - **structurelles**  : sont des classes instanciables.
 - **auxiliaires** : sont des classes permettant d'ajouter des attributs
     facultatifs à des classes structurelles

Voir diaporama du cours page 41.

## Les règles de recherche

Elles permettent aux serveur de comparer les valeurs des attributs avec celles
demandées dans les requêtes.

L'exemple ci-dessous présente une règle de recherche sur un entier : 

```
(
2.5.13.14
NAME
'integerMatch'
1.3.6.1.4.1.1466.115.121.1.27 )
```

# Le modèle de désignation

Afin de pouvoir identifier tous les objets dans l'annuaire, il est important de
définir des règles de nommage communes  a tous le service d'annuaire. deux
notions importante apparaissent : 

 - le **RDN** pour *Relative Distinguished Name* est un nom unique seulement
     dans le niveau considéré.
 - le **DN** pour *Distinguished Name** est un nom unique identifiant un objets
     dans l'arbre. Il est la somme de tous les RDN

Pour un nommage cohérent, il est  important de définir des règle commune de
nommage pour s'assurer de la cohérence des RND. Ceci permet de résoudre les
problèmes d'homonymie.

Exemple :

```
RDN = CN=DURAND Yves
CN=DURAND Yves,L=Paris,L=Ile de France,C=fr
```

# L'organisation hiérarchique

Un annuaire se présente sous la forme d'un arbre (DIT pour *Directory
information Tree*).

Il peut refléter l'organisation hiérarchique de la
structure ou son implantation géographique, voir même mixer les deux.

Il peut aussi représenter l'organisation de l'entreprise sous forme sémantique
en groupant les informations qui ont un même sens (personnes, groupes,
applications et.). Cette configuration facilite l'administration des droits
d'accès. Par exemple en affectant les droits d'écriture au DRH seulement sur
`ou=Personnes`

# Le modèle de services

LDAP est basé sur un modèle **Client <> Serveur**. Le client transmet une
requête décrivant une ou plusieurs actions à réaliser sur l'annuaire. Le serveur
est lui responsable de sa réalisation. Le serveur renvoi alors le résultat ou
une ou plusieurs références à d'autres serveurs susceptibles d'accéder à la
demande du client.

Toutes les opération sont encapsulées dans une envelope commune : **le message
LDAP** *(LDAPMEssage)*

Chaque requête possède son propre identifiant Il est possible de réutiliser cet
identifiant si : 

 - la requête n'a pas été abandonnée
 - elle n'ai pas eu de réponse à l'instant donné.

Quelle que soit la requête, la réponse sera donné sous la forme d'un message
*LDAPResult*

# Le modèle de sécurité

L'accès à l'annuaire LDAP peut se faire : 

 - en accès anonyme
 - par authentification via un mot de passe
 - par authentification par mot de passe via une liaison chiffrée TLS
 - par authentification forte via certificats
 - par authentification forte via certificats et liaison chiffrée TLS

# Le format LDIF

Le **LDIF** *LDAP Data Interchange Format* est un format de fichier de type
texte (ASCII) utilisé pour la représentation des données issues d'un annuaire
LDAP ou  d'opération sur les données (mais pas les deux à la fois).

Il est donc utilisé pour faciliter les opérations d'importation et/ou
d'exportation massive d'informations. Il contient une série d'enregistrements
séparés par des séparateur de lignes. Un enregistrement décrit un enregistrement
ou une modification.

Exemple : 

```ldif
dn: dmdName=Devices,ou=iut_mont-de-marsan,dc=universite, dc=education,dc=gouv,dc=fr
dmdName: Devices
objectClass: dmd
objectClass: top
dn: dmdName=Applications,ou=iut_mont-de-marsan,dc=universite,dc=education,dc=gouv,dc=fr
dmdName: Applications
objectClass: dmd
objectClass: top
```

Voir le format
[LDIF](https://fr.wikipedia.org/wiki/LDAP_Data_Interchange_Format) sur
Wikipedia.
