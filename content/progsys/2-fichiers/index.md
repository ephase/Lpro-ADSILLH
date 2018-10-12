---
title: "Les fichiers"
date: 2018-09-11
categories: ["Programmation système", "Cours"]
tags: ["fichiers", "descripteurs", "c"]
---

Dans la philosophie Unix, tout est fichiers, les entrees sorties sont donc
symbolisées par des manipulations de fichiers. Celles-ci se font par le biais de
**descripteurs de fichiers**, il représente le point d'entrée d'un fichier pout
un processus.

Pour chaque programme en cours d'exécution, le noyau maintient une table des
fichiers ouverts. Cette table contient entres autres le descripteur de fihers
(un nombre entier non signé) et la **position du curseur** ( *file offset* ).

Le curseur est la position où aura lieu la prochaine opération (lecture /
écriture). Cette position est exprimée en octets.

Il existe cependant 3 types particuliers de descripteur de fichiers :

 - l'entrée standard (0 ou `STDIN_FILENO`)
 - la sortie standard (1 ou `STDOUT_FILENO`)
 - la sortie d'erreur (2 ou `STDERR_FILENO`)

## Les appels système de base

### open()

```c
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int open(const char *pathname, int flags);
int open(const char *pathname, int flags, mode_t mode);
```

Cet appel système ouvre le fichier dont le chemin est `pathname` et retourne un
descripteur de fichier. En vas de problème, la fonction `open()`retourne -1.

#### flag

`flag` est un masquage de bits permettant de spécifier le mode d'accès : 

flag     | utilisation
---------|------------
O RDONLY | Lecture seule
O WRONLY | Écriture seule
O RDWR   | Lecture et écriture
O CREAT  | Création du fichier s'il n'existe pas
O TRUNC  | Troncature du fichier
...      | ...
O APPEND | Écriture à la fin du fichier

#### mode

C'est aussi un masquage de bits définissant le node du fichier en vas de
création :

| mode    | octal | utilisation                                           |
|---------|-------|--------------------------------------------------------
| S_IRWXU | 00700 | Le propriétaire a le droit de lire, écrire, exécuter  |
| S_IRUSR | 00400 | L’utilisateur a le droit de lire                      |
| S IWUSR | 00200 | L'utilisateur a le droit d'écrire                     |
| S IXUSR | 00100 | L'utilisateur a le droit d'exécuter                   |
| S_IRWXG | 00070 | Le groupe a le droit de lire, écrire, exécuter        |
| S_IRGRP | 00040 | Le groupe a le droit de lire                          |
| S IWGRP | 00020 | Le groupe a le droit d'écrire                         |
| S IXGRP | 00010 | Le groupe a le droit d'exécuter                       |
| S_IRWXO | 00007 | Le propriétaire a le droit de lire, écrire, exécuter  |
| S_IROTH | 00004 | L’utilisateur a le droit de lire                      |
| S IWOTH | 00002 | L'utilisateur a le droit d'écrire                     |
| S IXOTH | 00001 | L'utilisateur a le droit d'exécuter                   |

#### Exemple en C

```c
fd = open("test1.txt", O_RDWR | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
```
Ouvre un fichier `test.txt`, s'il n'existe pas il sera créé. Il est ouvert en
lecture - écritur seul le propriétaire a le droit de lire et d'écrire.

#### Exemple en Python

Il est possible de faire les même action en Python avec les modules `stat` et
`os`.

```python
import os, stat
f = os.open("test1.txt",
os.O_RDWR | os.O_CREAT | os.O_TRUNC,
stat.S_IRUSR | stat.S_IWUSR)
f = open("test1.txt", 'w+', 0o600) # umask issue
```

Ce programme réalise les mêmes actions que ci-dessus mais en Python

### read()

Cet appel système lit des données d'un fichiers

```c
#include <unistd.h>

ssize_t read(int fd, void *buf, size_t count);
```

Lit au plus `count` octets  depuis le fichier référencé par le descripteur `fd`
et les stocke dans `*buff`. Renvoie 0 s'il n'y a plus d'octets à lire  ou -1 si
la commande échoue

### write()

Écrits des données sur un fichier

```c
#include <unistd.h>
ssize_t write(int fd, const void *buf, size_t count);
```

Lit au plus `count` octets depuis la zone mémoire depuis `*buf` et les écrits
dans le fichier référencé par `fd`. Renvoie le nombre d'octets écrits ou -1 si
la commande échoue. Le nombre d'octets écrits peut être inférieur  à `count`
(plus de place disponible par exemple).

### close()

Ferme un fichier, rend libère le descripteur associé et les ressources associées
allouées par le noyau.

```c
#include <unistd.h>
int close(int fd);
```

Renvoie 0 en cas de réussite et -1 s'il échoue.

### Exemple de lecture-ecriture

### en c

```c
#include <unistd.h>
#include <stdlib.h>
#define BUFMAX 32

int main (int argc, char **argv) {
    char buffer[BUFMAX];
    int count;
    count = read(STDIN_FILENO, buffer, BUFMAX);
    if (count == -1) {
        write(STDERR_FILENO, "Unable to read stdin\n", 21);
        exit(EXIT_FAILURE);
    }
    write(STDOUT_FILENO, "Input data was: ", 16);
    write(STDOUT_FILENO, buffer, count);
    exit(EXIT_SUCCESS);
}
```

### en Python

```Python
#!/usr/bin/python3
import os, sys
def main():
    #buf = sys.stdin.read() # Ctrl-D for EOF
    try:
        buf = input()
    except EOFError:
        buf = ''
    except Exception as e:
        sys.stderr.write("Unable to read stdin: %s", e)
        sys.exit(os.EX_DATAERR)
    sys.stdout.write("Input data was: %s" % (buf))
    sys.exit(os.EX_OK)
main()
```

### lseek()

```c
#include <unistd.h>
off_t lseek(int fd, off_t offset, int whence);
```

Renvoie en octets depuis le début du fichier la nouvelle position du curseur.
Cet appel système modifie seulement la table de fichiers ouverts mais ne
provoque pas d'accès au système de fichiers.

### whence

position | utilisation
---------|------------
SEEK_SET | le curseur est placé à `offset` octets depuis le début du fichier
SEEK_CUP | le curseur est déplacé de `offset` depuis sa position courante
SEEK_END | le curseur est déplacé de `offset` octets après la fin de fichier

### fichiers à trou

Si l'on positionne le curseur après la fin du fichier puis une opération
d'écriture, on créé alors un **trou**. Les données à l'intérieur du trou
contiennent 0 pour notre fichier, mais ne prenne pas d'espace disque.

Ces **fichiers à trou** aussi appelés *sparse files* permettent d'utiliser le
système de fichier de manière plus efficace surtout lorsque le fichier est
majoritairement vide. Ils sont utilisés pour les bases de données, les images
disques, les fichiers journaux etc.

### Unlink ()

Détruit un nom dans le système de fichiers. Si ce nom était le dernier lien vers
un fichier et que celui-ci n'est ouvert dans aucun processus, il est effacé et
l'espace qu'il occupait est marqué comme disponible.

Si ce fichier et encore ouvert par un processus, il continu d'exister jusqu'à ce
que le dernier descripteur de fichier le référençant soit fermé.

```c
#include <unistd.h>
int unlink(const char *pathname);
```

## La librairie stdio

La librairie sdtio.h fourni par la librairie standard permet de manipuler les
fichiers avec des fonction de plus haut niveau.

Les fonction de cette librairie n'opèrent pas directement sur les descripteurs
de fichiers (notion spécifiques à Unix) mais sur des pointeur sur une structure
de type *FILE*. Elle contient entres autres :

 - le descripteur de fichier
 - un champ *flag* indiquant l'état du flux
 - une mémoire tampon
 - la position courante dans le tampon

Chaque appel système de gestion de fichiers a un équivalent dans `stdio`.

```c
#include <stdio.h>
FILE *fopen(const char *path, const char *mode);
FILE *fdopen(int fd, const char *mode);
int fclose(FILE *stream);
size_t fread(void *ptr, size_t size, size_t nmemb, FILE *stream)
size_t fwrite(const void *ptr, size_t size, size_t nmemb, FILE *stream);
int fseek(FILE *stream, long offset, int whence);
int fflush(FILE *stream);
int fprintf(FILE *stream, const char *format, ...);
```
