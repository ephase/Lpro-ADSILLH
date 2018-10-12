---
title: "Base de données : définitions"
date: 2018-09-17
modify: 2018-09-24
categories: ["Base de données", "cours"]
---

## Le domaine

Il représente l'ensemble des valeurs autorisées pour une information. Cette
notion est très proche [de domaine de définition](w_dom-def) en mathématiques.
On le décompose en deux grandes familles.:

 - **élémentaire** : les types de base comme les entiers, réels, chaines de
     caractères mais aussi les intervales ou les listes de valeurs
 - **structuré** : des types de valeurs plus structurée comme pas exemple la
     notion de point définit pat une composant `X` et `Y`.

[w_dom-def]:https://fr.wikipedia.org/wiki/Ensemble_de_d%C3%A9finition

## Le produit cartésien

Le produit cartésien des domaines $D_{1} D_{2}$ représente l'ensemble
des valeurs contenue dans $D_{1}$ et $D_{2}$, il est noté

$$
 D_{1} \times D_{2}
$$

## La relation

Elle représente un sousrensemble r du produit carthésien. Elle est caractérisée
par un nom.

## Tuple ou n-tuple

Il représente une ligne dans une relation

## Attributs

Nom donné à un colonne. Il est composé d'un indentifiant et d'un domaine

## Schéma de relation

Il ets définis par un ensemble d'attributs U et un ensenble de contraintes. on
le note courrament R(U), R(U) décrit l'intention de la relation. La relation
(tableau + valeurs) définit une extention.

Une relation `r` est une instance finie d'un schema de relation notée `r:R(U)`,
exemple :

```
AlimentsPreferes(nom,type,origine,bio)
```

## La base de données

Un schéma de base de données et un ensemble de schemas de relation liés par des
dépendances référencielles (un type de contraines). Une base de données est
alors un ensemble de relations (extensions) associées au schéma et verifiamts
ses contraines.

## Le modèle relationnel

Il est définis en 1970 par *E. F. Codd* (IBM Research Lab) et se veut
indépendant de la representation physique des données. Il a une assise
mathématique forte ([algèbre relationnelle](w_algebre-r), [formes
normales](w_form-norm)).

[w_algebre-r]:https://fr.wikipedia.org/wiki/Alg%C3%A8bre_relationnelle
[w_form-norm]:https://fr.wikipedia.org/wiki/Forme_normale_(bases_de_donn%C3%A9es_relationnelles)

### Objectifs généraux

  - Éliminer les comportements anormaux lors des mises à jour des données.
  - Éliminer les données redondantes.
  - Meilleures compréhension des relationssémantiques entres les données.

### Buts

Éliminer les anomalies de la relation universelle (tous les attributs sur toutes
les colonnes) pour faciliter la manipulation des relation. En un mot normaliser
les relations.

### Méthode

Décomposer la relation universelle en sous-relations qui ne souffre pas des
anomalies de la relation universelle.

 - conserve toutes les données
 - conserve un minimum de contraintes

### Les types de contraintes

 - sur les attributs : valeur nulle autorisées (ou non), valeur imposée ...
 - sur les n-uplets (plusieurs valeurs) : l'exemple d'une bibliothèque, la date
     de retour d'un livre doit être suppérieure à la date d'emprunt.
 - sur les relations : clé, cardinalité
 - sur la base de données : clé étrangères
 - sur l'évolution temporelle de la base de données : exemple de l'état civil,
     on passe, dans l'ordre de célibataire à marié puis divorcé. On ne peut pas
     passer de célibataire à divorcé.

# La notion de clé

Une clé doit permettre de trouver un tuple dans une relation avec seulement une
partie des attributs, dit plus simplement, identifier sans équivoque un
enregistrement.

Du point de vue mathématique, il ne doit pas avoir deux fois le même éléments
dans un ensemble : deux tuples d'une même relation ne peuvent être égaux.

Soit un ensemble $U\{A_{1}, A_{2}, ..., A_{n}\}$
