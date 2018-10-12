---
title: "Les processus"
date: 2018-09-18
categories: ["Programmation système", "Cours"]
tags: ["processus", "programme", "c"]
---

Il ne faut pas confondre processus et programme, un processus étant une
instance chargée en mémoire et en cours d'exécution d'un programmme. Plusieurs
instance d'un programme peuvent s'exécuter en même temps.

À chaque processus est assicié un identidiant (PID) et un espace de stockage en
mémoire pou y stocker la pile, les données, le code etc. (espace d'adressage).

Un programme contient toutes les informations nécessaire à la création du
processus :

 - un format binaire ELF (historiquement a.out, Coff)
 - le point d'entrée de la première instruction
 - les données
 - les information de débogage
 - les information sur les librairies partagées

Un processus est une entité abstraite connue du noyau aui lui alloue des
ressources pour son execution. Du point de vue du noyau un processus consiste en
:

 - En mode **user**
     - une portion d'espace mémoire
     - le code du programme
     - les variables auquelles le programme a accès
 - em mode **kernel**
     - Une structure de données de l'état du processus
     - la table des fichiers ouverts
     - la table de la mémoire allouée
     - le répertoire courant
     - la priorité
     - le gestionnaire d'évènements (signaux, fin de programmes ...)
     - les limites du processus

## le PID

Il est créé par le noyau et aussi utilisé dans l'espace utilisateur (`kill`,
`ps`, `nice`)

```c
#include <unistd.h>

pid_t getpid(void)
```

Renvoi le PID du processus appelant. Cette fonction est ouvent utilisée par des
routines qui créées des fichiers temporaires uniques.

`pid_t` est un type abstrait pouvant être transtypé en `int`, sa valeur maximale
est définie dans `/proc/sys/kernel/pid_max`

## Démarrage d'un programme en C

Un programme en C commence toujours par la fonction `main()` : 

```c
int main (int argc, char **argv) {
}
```

 - `argc` : entier, nombres d'arguments passés
 - `**argv` : tableau de pointeurs, arguments passés

L’invocateur du programme (ex: le shell) exécute l’appel système execvp et
utilise les arguments passés au programme comme paramètres.

### exemple 

```shell
$ mycommand --color -l -s

argv = ["mycommand", "--color", "-l", "-s"]
argc = 4
```

on récupèrera les paramètres avec le code c suivant :

```c
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv){
    for (int i=0; i<argc; i++){
        printf("Argument %i value %s",i, argv[i]);
    }
}
```

La boucle aurait pu aussi être écrite comme suit :

```c
for(int i=0; argv[i] != NULL; i++){}
```

## Terminer un programme

Il y a deux façons de terminer un programme :

 - **nomale** : par la fin du `main()`, un `exit()` : nettoyage de toutes les
     ressources, appel de `close()` sur tous les descripteurs de fichiers etr
     appel du getionnaire de sorties (*exit handlers*)
  - **anormale** : par la réception d'un signal (kill par exemple)

Par conventionm un programme qui termine correctement renvoi 0, dinon il renvoi
un entier positif.

### atexit

```c
#include <unistd.h>
int atexit(void (*function)(void));
```

La fonction `atexit()` enregistre la fonction donnée pour que celle-ci soit
automatiquement appelée lorsque le programme se termine normalement avec exit(3)
ou lors de la fin de la fonction main. Les fonctions ainsi enregistrées sont
invoquées dans l'ordre inverse de leur enregistrement ; aucun argument n'est
transmis `atexit()` renvoie 0 en cas de succès et une valeur non nulle en cas
d'échec 

## Mémoire allouée

La mémoire allouée à chaque processus est divisée en segments :

 - **le segment texte**, en lecture seule contient les instructions en langage
     machine (le binaire su programme au format *ELF*)
 - **le segment des données initialisées** qui contient les variables globales
     et statiques explicitement initialisées. Les valeurs de ces variables sont
     lues depuis le fichiers exécutables lorsqu'il est chargé.
 - **le segment des données non-initialisées** aui contient des variables
     globales et statiques non explicitement initialisées (BSS)
 - **la pile** appelées aussi *stack* est un segment dynamique aui s'étend et se
     réduit. Il contient des blocs de pile appelés *stack frame* par fonction
     appelée. Une *frame* contient les variables locales et la/les valeur(s) de
     retour de la fonction.
 - **le tas** aussi appelé *heap* est une zone où la mémoire peut être
     dynamiquement allouée pour les variables à l'exécution du programme.

L'espace mémoire d'un processus est virtuel car celui-ci n'adresse pas
directement la mémoire. Les processus sont isolées du noyau et le noyau
maintient un tableau de pages mémoires aui adresse les pages de la mémoire
physique. Un processus peut adressser plus de mémoire que le système en a de
disponible.

Sous Unix, un processus est définis sous forme arborescente : chacun d'eux a un
processus parent (mis à pat le 1 - processus d'init) et chacun a aucun, un ou
plusieurs fils.

## Création de processus

```c
#include <unistd.h>

pid_t fork(void);
```

Après le `fork()`, le processus parent continue normalement son exécution. Le
processus fils commence son exécution juste après. On distingue le parent du
fils par la valeur de retour du `fork()` 0 pour le fils, le *PID* du fils pour
le père). Le fils est une copie du père cependant seule les zones modifiées sont
vraiment copiées. Le segment *text* est partagé entre les deux 

Après  le `fork()`, la table des fichiers est partagées entre le parent et
l'enfant. Ainsi si l'offset est modifié dans un des processus, il le sera aussi
pour l'autre. Cela permet entres autres de faire collaborer les processus sur
les mêmes données.

### exemple de `fork()`

```c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
int main (int argc, char **argv) {
    pid_t pid;
    printf("Starting process with PID=%d\n", getpid());
    pid = fork();
    if (pid < 0) {
        perror("Unable to fork");
    }
    else if (pid == 0) {
        printf("Starting child with PID=%d (my parent PID=%d)\n", getpid
    }
    else {
        printf("Still in process with PID=%d\n", getpid());
    }
    printf("Finishing process with PID=%d\n", getpid());
    exit(EXIT_SUCCESS);
}
```

## Terminaison de processus

Quelle que soit la façon dont se termine un processus (père comme fils), le
noyau nettoie les ressources allouées. Il stocke cependant le status de
terminaison du fils jusqu'au moment ou le père en prend connaissance.

Lorsque le père se termine, le processus fils devient enfant de 1 (*PID* 1)

### Processus zombies

Lorsqu'un processus enfant est terminé mais que le parent n'a toujours pas pris
connaissance de sa terminaison, il devient alors un processus zombie. Ce dernier
continue de consommer des ressources noyau (code de terminaison, entrée dans la
table des processus etc.)

### Attendre la fin d'un processus enfant

```c
#include <sys/types.h>
#include <sys/wait.h>
pid_t waitpid(pid_t pid, int *status, int options);
pid_t wait(int *status); // waitpid(-1, &status, 0);
// en cas de réussite, l'identifiant du processus fils
// terminé (ou changé) est renvoyé ; en cas d'erreur,
// la valeur de retour est -1
```

L'appel système `waitpid()` permet d'attendre la fin d'un processus. C'est un
appel bloquant (sauf avec certaines options). Il retourne une erreur s'il n'y a
pas de processus fils. Lorsque le processus fils se termine, la variable
`status` contient son code de retour.
## Création d'un nouveau processus

Sous Unix, la création d'un nouveau processus et l'exécution d'un programme sont
deux choses distinctes. L'exécution d'un nouveau processus ne créé pas de
nouveau processus, il remplace juste le programme appelant en allant chercher
les données sur le système de fichiers.

À l'exécution, tous les segments son réinitialisés comme si le programme avait
été exécuté normalement.

L'exécution d'un programme se fait grâce à l'appel `execve()`

```c
#include <unistd.h>
extern char **environ;
int execl(const char *path, const char *arg, ..., (char *) NULL);
int execlp(const char *file, const char *arg, ..., (char *) NULL);
int execle(const char *path, const char *arg, ...,(char *) NULL, char *const envp[]);
int execv(const char *path, char *const argv[]);
int execvp(const char *file, char *const argv[]);
int execvpe(const char *file, char *const argv[], char *const envp[]);

// Une sortie des fonctions exec() n'intervient que si une erreur s'est
// produite. La valeur de retour est -1, et errno contient le code
// d'erreur.
```

## Le format ELF

ELF pour *Executable and Linkable Format* est utilisé pour l'exécution de code
compilé. Dévellopé par Unix System Labortatories, il est utilisé par Linux, BSD,
System V, IRIX, SPARC.


