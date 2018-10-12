Notes de cours et TD machines
-----------------------------

Dépôt git pour la construction d'un sitre statique avec
[Hugo](https://gohugo.io) reprenant l'ensemble de mes notres de cours et TM
machine mises au propre (J'ai beaucoup de retard...)

Je rajouterai plus tard un `makefile` pour la construction de fichiers PDF avec
[pandoc](https://pandoc.org/)

## Construction du site

### Installer le thème

Pour que la construction du site fonctionne, il faut installer le thème visuer
choisi (ici er). Il est présent sous la forme d'un `submodule` git au'il faut
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
d'information sur la sortie standard. Une fois ce mode activé, il suffit
d'ouvrir son navigateur er d'aller à l'adresse *http://localhost:1313*.

## Site en ligne

Le site n'est pas encore accessible en ligne, un jour peut-être...

## Bugs

C'est la première fois que j'utilise un générateur de site statique, le
focntionnement du site n'est donc pas optimal. Les pages **tags** et
**catégories** ne fonctionnent pas (pages blanches) pour une raison encore
mystérieuse.
