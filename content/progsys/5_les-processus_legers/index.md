---
title: "Les processus légers"
categories: ["Programmation système", "cours"]
tags: ["C", "programmation", "threads", "mutex"]
date: 2018-10-02
---

Les processus légers son similaires aux processus dans la mesure ou il permettent
l'exécution d'actions concurrentes (réception de données et mise à jour de
l'interface graphique par exemple). Ils permettent aussi une véritable exécution
en parallèle sur une architecture multi-cœurs / multi-processeurs.

Contrairement aux processus, les threads partagent tous un même espace mémoire
(mais avec une pile d'exécution différente)

Il comportent plusieurs avantages :

 - le partage d'information est plus simple avec les processus légers (pas de
     mise en place d'*IPC*)
 - ils sont... légers,  de l'ordre de facteur 10 pour leurs création par le
     noyau
 - le partage d'information entre processus légers est de facto plus simple
     puisqu'ils partagent le même espace mémoire

Ils partagent :

 - comme on le disait, leur espace mémoire
 - le *PID* et *PPID* (id de processus et id du processus parent)
 - la table des fichiers ouverts
 - le gestionnaire de signaux

Ils ne partagent pas :

 - le *thread ID*, id de processus léger
 - leurs piles d'exécution ; pas de partage de variables locales lors d'appels
     de fonctions
 - `errno` car cela présenterai un risque de "collision" entre threads
 - la mémoire locale du processus appelée *Thread Local Storage*

## L'API Thread POSIX (Pthreads)

Issue de POSIX .1c et intégrée dans la norme SUSv3, cette API est devenue
standards sous les système de type UNIX. Elle couvre les types de données, la
gestion des processus légers (creation, annulation, attente, etc.), les verrous,
les exclusions mutuelles, les variables conditionnelles etc.

Voir [Pthread][w_pthreads] sur Wikipedia, ou la [version anglaise][w_pthreads_e]
un peu plus complète.

[w_pthreads]:https://fr.wikipedia.org/wiki/Threads_POSIX
[w_pthreads_e]:https://en.wikipedia.org/wiki/POSIX_Threads

### convention de l'API

Habituellement, un processus renvoie 0 ou un entier positif en cas de succès. En
cas d'échec il renvoie -1 et `errno` est positionné. Dans le cadre de Pthreads,
0 est renvoyé en cas de succès et seulement `errno` est positionné en cas
d'échec.

## Compiler un programme C avec des threads

Pour compiler un programme C faisant appel au threads, il faut rajouter
`-ptread` à gcc

```shell
$ gcc -Wall -pthread mon_prog.c -o monprog.bin
```

## Création de processus légers

```C
#include <pthread.h>

int pthread_create(pthread_t *thread, const pthread_attr_t *attr,
    void *(*start_routine) (*void), void *arg);
```

 - **start_routine** pointe vers la fonction appelée lors de la création du
     processus léger.
 - **arg** sera passé en argument de cette fonction.
 - **thread** sera rempli avec l'ID du thread.
 - **attr** permet de définir des attributs pour le processus léger.

Par convention, processus est considéré comme le thread principal.

## fin d'un processus léger

Un thread se termine : 

 - lorsque sa fonction principale se termine
 - lorsqu'il est annulé avec l'appel de `pthread_cancel()`
 - lorsque l'un des thread appelle `exit()`, dans ce cas tous les processus
     légers associés se terminent.
 - lorsque le thread appelle `pthread_exit()` :
     ```c
     #include <pthread.h>
     void pthread_exit(void *retval);
     ```
     Cette fonction ne retourne rien à l'appelant, `retval` sera renvoyé aux
     autres processus en attentes de sa terminaison.

## Attendre la fin d'un thread

```c
#include <pthread.h>
int pthread_join(pthread_t thread, void **retval)
```

Cette fonction renvoie 0 en cas de succès, sinon `errno` est positionné. C'est
un appel bloquant, le processus appelant sera bloqué jusqu'à la terminaison du
thread attendu. `retval` prendra la valeur de retour du processus léger attendu.

## Exemple : hello world avec un thread

```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
void *tmain(void *arg) {
    char *msg = (char *) arg;
    printf("[T] %s", msg) ;
    pthread_exit((void *) strlen(msg));
}
int main(int argc, char **argv) {
    pthread_t t;
    int r;
    void *res;
    printf("Creating thread...\n");
    r = pthread_create(&t, NULL, tmain, (void *) "Hello World!\n");
    if (r != 0) {
        perror("Unable to create thread");
        exit(EXIT_FAILURE);
    }
    r = pthread_join(t, &res);
    if (r != 0) {
        perror("Unable to join thread");
        exit(EXIT_FAILURE);
    }
    printf("Thread return: %ld\n", (long) res);
    exit(EXIT_SUCCESS);
}
```

## Processus légers zombies

Tout comme un processus léger normal, un processus léger zombie est un processus
léger dont aucun autre n'a pris connaissance de sa fin. Les threads zombies
posent les mêmes problèmes de consommation ressources que les processus.

Sur certains systèmes UNIX, le nombres de processus légers par processus est
limité au niveau du noyau :

```shell
ephase@archlinux$ cat /proc/sys/kernel/threads-max   
125626
```

## Détacher un processus léger

Les processus légers détachés ne sont plus joignables et seront pris en compte
automatiquement à leurs terminaison (sans devenir zombie).

```c
#include <pthread.h>
int pthread_detach(pthread_t thread);
```

Retourne 0 en cas de succès, sinon `errno` est positionnée.

## Identité d'un processus léger

Les identifiants de threads doivent être considéré comme opaque. Il n'existe pas
de façons portables de comparer directement deux valeurs de `pthread_t`

```c
#include <pthread.h>

pthread_t pthread_self(void);
// Retourne l'identifiant du thread appelant
// Cette fonction réussit toujours

int pthread_equal(pthread_t t1, pthread_t t2);
// Retourne une valeur non nulle si les 2
// identifiants sont égaux
// Retourne 0 dans le cas contraire
```

## Les section critiques

### définition

Une section critique est un fragment de code qui accède à un ressource partagées
(variable globale par exemple) et devant être exécuter de façon atomique au
regard des autres entités qui veulent y accéder.

Une section critique peut être protégée par un **mutex**, un **sémaphore** ou 
d'autres primitives de programmation concurrente.

Voir [section critique][l_seccrit] sur Wikipedia

[l_seccrit]:https://fr.wikipedia.org/wiki/Section_critique


#### Atomicité

Opération ou ensemble d'opération devant s'exécuter entièrement sans pouvoir
être interrompues.

Voir [atomicité][l_atomicite] sur Wikipedia

[l_atomicité]:https://fr.wikipedia.org/wiki/Atomicit%C3%A9_(informatique)

### Exemple

On souhaite incrémenter une variable global`static long glob = 0;` depuis deux
(ou plus) threads qui s’exécute simultanément.

#### Code C

```C
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

#define LOOP_COUNT 100000000
#define THREADS_COUNT 2

static long glob = 0;

void *tmain(void *arg) {
    long loc;
    for(long i = 0; i< LOOP_COUNT; i++) {
        loc = glob;
        loc++;
        glob = loc;
    }
    return NULL;
}
int main(int argc, char **argv) {
    pthread_t tlist[THREADS_COUNT];
    int r;
    void *res;
    for(int i=0; i<THREADS_COUNT; i++) {
        printf("Creating thread %d...\n", i);
        r = pthread_create(&tlist[i], NULL, tmain, NULL);
        if (r != 0) {
            perror("Unable to create thread");
            exit(EXIT_FAILURE);
        }
        printf("Thread %d running...\n", i);
    }
    for(int i=0; i<THREADS_COUNT; i++) {
        printf("Joining thread %d...\n", i);
        r = pthread_join(tlist[i], &res);
        if (r != 0) {
            perror("Unable to join thread");
            exit(EXIT_FAILURE);
        }
        printf("Thread %d terminated...\n", i);
    }
    printf("%ld =? %ld\n", glob, (long) THREADS_COUNT * (long) LOOP_COUNT);
    exit(EXIT_SUCCESS);
}
```

#### Exécution

```shell
$ gcc -Wall -pthread critical-section-pthread.c -o critical-section-
$ ./critical-section-pthread
Creating thread 0...
Thread 0 running...
Creating thread 1...
Thread 1 running...
Joining thread 0...
Thread 0 terminated...
Joining thread 1...
Thread 1 terminated...
100954120 =? 200000000
```

On voit bien ici que le résultat ne correspond pas du tout à ce qui est attendu.
Pire encore, il est aléatoire (plusieurs exécution à la suite ne donnent pas le
même résultat.

## Les mutex

Pour protéger les sections critiques, il faut un mécanisme d'exclusion
mutuelle. L'API POSIX fournie les **mutex** comme mécanisme de synchronisation
entre threads pour protéger les section critiques.

### Création de mutex

Il y a deux façon de créer un mutex : **statique**, qui sous-entends
l'utilisation d'options par défaut et **dynamique**. Dans le cas du mutex
dynamique, une zone mémoire est allouée sur le tas (*heap*).

```C
// mutex statique
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

// mutex dynamique
// retourne 0 en vas de succès sinon errno est positionné
int pthread_mutex_init(pthread_mutex_t *mutex,
        const pthread_mutexattr_t *mutexattr);
```

### Destruction de mutex

Les mutex alloués dynamiquement doivent être détruits lorsqu'il ne sont plus
utiles. Un mutex doit être détruits seulement lorsqu'il n'est plus verrouillé
et seulement s'il ne sera plus utilisé.

```c
int pthread_mutex_destroy(pthread_mutex_t *mutex);
```

Retourne 0 en cas de réussite sinon `errno` est positionné.

### le verrouillage / déverrouillage en C

```c
int pthread_mutex_lock( pthread_mutex_t *mutex );
int pthread_mutex_unlock( pthread_mutex_t *mutex );
int pthread_mutex_trylock( phread_mutex_t *mutex ):
```

Ces trois fonctions retournent 0 en cas de succès et positionnent `errno` en cas
d'erreur. `pthread_mutex_trylock` est la version non bloquante de
`phtread_mutex_lock`

### L'interblocage

Lorsque deux threads attendent la libération d'une ressource possédé par
l'autre, la situation est alors bloquées : on parle de *deadlock* ou
interblocage.

> T1 possède R1 mais essaye d'accéder à R2 possédée par T2 qui essaye d'accéder
> à R1

### Les cas limites

 1. Un thread tente de déverrouiller un mutex pour la seconde fois
 2. Un thread tente de déverrouiller un mutex qui n'est pas verrouillé.
 3. Un thread qui tente de déverrouiller un mutex qu'il n'as pas verrouillé
    lui-même

### Les différents types de mutex

 - PTHREAD_MUTEX_NORMAL : il n'y a pas de détection d'interblocage avec ce type
     de mutex, seul le cas limite 1 finira en erreur.
 
 - PTHREAD_MUTEX_ERRORCHECK : détection complète de l'interblocage, ainsi les 3
     cas limites cités plus haut retournerons une erreur. C'est cependant plus
     lent au niveau de l'exécution.

 - PTHREAD_MUTEX_RECURSIVE : les verrouillages multiples sur les mutex peuvent
     sont possibles mais requièrent le même nombre de déverrouillages. Les cas
     limite # retournera cependant une erreur.

## Les variables conditions

Les mutex empêchent l'accès à une section critique par plusieurs threads, mais
l'utilisation de boucles d'attentes induites par leurs utilisations entraîne
une utilisation intensive du CPU. Il est donc possible pour un thread d'utiliser
des **variables conditions** afin d'informer ses pairs d'une changement d'une
ressource partagée. Ainsi les autres threads peuvent se mettre en attente
jusqu'à obtention du changement d'état de la ressource demandée.

Il existe deux états pour une variable condition : `wait` (bloque jusqu'à
l'arrivée d'une notification) et `signal` (notification). Une variable condition
est *sans état* : si une notification est envoyée mais qu'aucun thread ne
l'attend, elle est perdue.

### Déclaration

Tout comme les mutex, il existe deux façon de déclarer des variables condition :
statique ou dynamique (sur le tas) : 

```C
// statique
pthread_cond_t cond = PTHREAD_COND_INITIALIZER ;

// dynamique
int pthread_cond_init(pthread_cond_t *cond, pthread_condattr_t *cond_attr);
```

La version dynamique renvoie 0 en cas de succès, sinon `errno` est positionnée.

### Mise en attente d'un thread

```C
int pthread_cond_wait(pthread_cond_t *cond, pthread_mutex_t *mutex);
```

Bloque le thread appelant jusqu'à l'arrivée d'une notification. L'appel de cette
fonction déverrouille atomiquement le mutex quoi doit être verrouillé par le
thread appelant avant. Lorsque `pthread_cond_wait` rend la main au thread
appelant, il reverrouille le mutex comme `pthread_lock_mutex`

Comme pour les mutex, la fonction envoie 0 en cas de succès et positionne
`errno` dans le cas contraire.

### Envoi d'une notification

```C
int pthread_cond_signal(pthread_cond_t *cond);
int pthread_cond_broadcast(pthread_cond_t *cond);
```

`pthread_cond_signal` relance l'un de thread mis en attente par
`pthread_cond_wait` avec comme variable de signal `cond`. si plusieurs threads
attendent, seul l'un d'eux sera relancé mais impossible de savoir lequel. Si
aucun thread attend, rien ne se passe

`pthread_cond_broadcast` relance tous les threads mis en attente avec comme
variable de signal `cond`. si aucun thread n'est en attente, rien ne se passe.

### Détruire une variable condition.

Les variables conditions allouées dynamiquement doivent être détruites une fois
qu'elle n'ont plus d'utilité.

```C
in pthread_cond_destroy(pthread_cond_t *cond);
```

Une variable condition ne doit être détruite seulement si aucun thread n'est en
attente sur cette même condition. Comme d'habitude, retourne 0 en cas de succès
et positionne `errno` en cas d'erreur.

### Exemple de code C avec mutex et variable condition

```C
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

static long goods = 0;
static pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t avail = PTHREAD_COND_INITIALIZER;

void *producer(void *arg) {
    while(1) {
        sleep(1); // simulate time to produce
        if (pthread_mutex_lock(&mutex) != 0) {
            perror("Mutex lock failed");
            exit(EXIT_FAILURE);
        }
        goods++;
        if (pthread_mutex_unlock(&mutex) != 0) {
            perror("Mutex unlock failed");
            exit(EXIT_FAILURE);
        }
        if (pthread_cond_signal(&avail) != 0) {
            perror("Condition signal failed");
            exit(EXIT_FAILURE);
        }
    }
    return NULL;
}

void *consumer(void *arg) {
    while(1) {
        if (pthread_mutex_lock(&mutex) != 0) {
            perror("Mutex lock failed");
            exit(EXIT_FAILURE);
        }
        while(goods == 0) {
            if (pthread_cond_wait(&avail, &mutex) != 0) {
                perror("Condition wait failed");
                exit(EXIT_FAILURE);
            }
        }
        while(goods > 0) {
            goods--;
            printf("Consuming...\n");
        }
        if (pthread_mutex_unlock(&mutex) != 0) {
            perror("Mutex unlock failed");
            exit(EXIT_FAILURE);
        }
    }
    return NULL;
}

int main(int argc, char **argv) {
    pthread_t t_producer, t_consumer;
    printf("Creating producer thread...\n");
    if (pthread_create(&t_producer, NULL, producer, NULL) != 0) {
        perror("Unable to create producer thread");
        exit(EXIT_FAILURE);
    }
    printf("Creating consumer thread...\n");
    if (pthread_create(&t_consumer, NULL, consumer, NULL) != 0) {
        perror("Unable to create consumer thread");
        exit(EXIT_FAILURE);
    }
    if (pthread_join(t_producer, NULL) != 0) {
        perror("Unable to join producer thread");
        exit(EXIT_FAILURE);
    }
    if (pthread_join(t_consumer, NULL) != 0) {
        perror("Unable to join consumer thread");
        exit(EXIT_FAILURE);
    }
    exit(EXIT_SUCCESS);
}
```

## Les fonctions *Threads Safe*

On dit d'une fonction quell est *threads safe*lorsqu'elle est capable de
fonctionner correctement lorsqu'elle est exécutée simultanément au sin d'un même
espace d'adressage par plusieurs threads.

Les threads ne devraient pas invoquer de fonction non thread safe sans mettre
en place des mécanisme de synchronisation entre eux comme les mutex.

## Bibliographie

 - [Posix Thread Programming][l_posixtprog] sur le site du Livemore Computing
     Center (en)
 - [Initiation à la programmation multitâches en C avec pthreads][l_progcmulti]
     par Franck Hecht sur Developpez.com. Avec un test de performance sur un
     programme C avec et sans variable condition.

[l_posixtprog]:https://computing.llnl.gov/tutorials/pthreads/
[l_progcmulti]:https://franckh.developpez.com/tutoriels/posix/pthreads/
