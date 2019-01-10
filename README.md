Notes de cours et TD machine
-----------------------------

Dépôt git pour la construction d'un site statique avec
[Hugo](https://gohugo.io) reprenant l'ensemble de mes notres de cours et TM
machine mises au propre (J'ai beaucoup de retard...)

Je rajouterai plus tard un `makefile` pour la construction de fichiers PDF avec
[pandoc](https://pandoc.org/)

## Construction du site

### Installer le thème

Pour que la construction du site fonctionne, il faut installer le thème 
mainroad. Il est présent sous la forme d'un `submodule` git qu'il faut
initialiser (dans le répertoire racine du dépôt).

```shell
git submodule init && git submodule update
```

### Version locale du site

Pour construite le site, il faut au préalable installer hugo, sur Archlinux : 

```shell
# pacman -S hugo
```

Pour activer le serveur web intégré à Hugo, il suffit de lancer la commande
(dans le répertoire racine du dépôt) :

```shell
$ hugo server -D --disableFastRender -v
```

Il est possible de rajouter l'option `--debug` afin d'afficher plus
d'informations sur la sortie standard. Une fois ce mode activé, il suffit
d'ouvrir son navigateur à l'adresse *http://localhost:1313*.

## Bugs

C'est la première fois que j'utilise un générateur de site statique, le
fonctionnement du site n'est donc pas optimal
