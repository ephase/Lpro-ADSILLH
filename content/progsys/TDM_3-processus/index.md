---
Title: "TDM : Les Processus"
Date: 2018-09-18
Categories: ["Programmation système", "TD machine"]
tags: ['TD Machine', 'programmation', 'C', 'processus']
---

[Télécharger les questions](./files/tdm3.pdf)
## Question 1

Sans exécuter le programme, je pense qu'il y aura Il y a 4 processus de créé.

```
q1.bin─┬─q1.bin
       │      
       ├─q1.bin
       └─q1.bin
```

Les processus se "clonent" et l'arbre des processus est plus complexe que prévu
: il y a 8 processus au total.

```shell
$ ./q1.bin & 
[1] 8895
$ pstree 8895
q1.bin─┬─q1.bin─┬─q1.bin───q1.bin
       │        └─q1.bin 
       ├─q1.bin───q1.bin
       └─q1.bin
```

## Question 2

Je suis parti au départ pour créer la commande avec une boucle for pour
parcourir `*argv` mais il est plus simple de construire la commande avec
` &argv[1]` qui retourne le tableau après l'élément 1.

```C
int out = execvp(argv[1], &argv[1]);
```
J'ai aussi fait en sorte que le programme père se termine sur un `EXIT_FAILURE`
si le fils se termine sur une erreur.

```C
while ((pid = waitpid(pid, &status, 0)) > 0){
    (status == 0) ? exit(EXIT_SUCCESS) : exit(EXIT_FAILURE);
}
```
Télécharger mon [code](./src/td3/q2.c).

## Question 3

Ma proposition était un peu complexe et surtout ne correspondant pas vraiment à
la demande. J'ai donc modifié ma réponde (q3bis.c)

Télécharger mon [code](./src/td3/q3.c).
Et la version remaniée [code](./src/td3/q3bis.c).

