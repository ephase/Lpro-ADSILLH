---
title: "TDM : les librairies"
date: 2018-11-16
categories: ["Programmation système", "TD machine"]
---

Le but de ce TD est de détourner la commande `uname` en réécrivant la fonction
`uname()` dans une librairie partagée et de charger celle-ci grâce à un
`LD_PRELOAD`

Pour afficher de information sur la la fonction `uname`, il suffit de faire un
`man 2 uname`

## code de ma fonction de remplacement

```c
#include <sys/utsname.h>
#include <string.h>

int uname(struct utsname *buf)
{

    strcpy(buf->sysname,"Windows GNU/LINUX");
    strcpy(buf->nodename, "");
    strcpy(buf->release, "10.0.0.1");
    strcpy(buf->version, "10.2");
    strcpy(buf->machine, "Shittycorp processors inc.");

    return 1;
}
```

Il faut charger les entêtes `sys/utsname.h` (définition de la structure de
donnée `utsname`) et `string.h` (pour l'utilisation de `strcpy()`

Cette dernière est nécessaire pour manipuler les chaines de caractères avec le
pointer `*buf` vers la structure de données `utsname`.

## compilation et exécution

la compilation de la librairie se fait donc avec `gcc` et les arguments `-fPIC`
et `-shared`:

```shell
gcc -Wall -fPIC -shared ./src/libuname.c -o src/libuname.so 
```

Comme indiqué en cours, il suffit d'exécuter le programme `uname` en
préchargeant notre librairie : 

```shell
LD_PRELOAD=./libuname.so uname
```

*Attention*, il fait bien indiqué le répertoire dans lequel se trouve la
librairie dans le `LD_PRELOAD`, même si elle se situe dans le répertoire
courant. Ici j'ai bien spécifié `./libuname.so`, en ne mettant que `libuname.so`
dans le `./` le chargement de la librairie ne fonctionne pas : 

```shell
LD_PRELOAD=libuname.so uname 
ERROR: ld.so: object 'libuname.so' from LD_PRELOAD cannot be preloaded (cannot open shared object file): ignored.                                                        Linux
```
