---
title: "Installation d'un serveur DHCP et DNS"
date: 2018-11-15
categories: ["Installations", "TD machine"]
---

Avant toutes choses, j'ai modifié mon réseau virtuel pour ne plus avoir de DHCP.
J'ai ensuite commencé l'installation d'une nouvelle machine virtuelle. Comme je
n'ai plus de DHCP sur le réseau, j'ai mis une adresse IPv4 manuelle :

```
ip : 192.168.122.254/24
passerelle : 192.168.122.1
```

## Installation d'un serveur DHCP

Le serveur DHCP le plus utilisé est `isc-dhcp-server` il suffit de l'installer
avec la commande :

```shell
# apt install isc-dhcp-server
```

Il existe aussi une version avec le support de LDAP en tant que backend, mais
nous verrons celà plus tard.

### Configuration

La configuration du serveur se fait dans le fichier `/etc/dhcp/dhcpd.conf`. Elle
est succinte et rapide à mettre en place

```
# option definitions common to all supported networks...
option domain-name "u-bordeaux.com";
option domain-name-servers 192.168.122.1;

default-lease-time 3600;
max-lease-time 7200;

ddns-update-style none;

# Our subnet definition
subnet 192.168.122.0 netmask 255.255.255.0 {
        range 192.168.122.100 192.168.122.120;
        option routers 192.168.122.1;
}

# The only dhcp server in our network
authoritative;
```

## DNS

Le paquet à instller est `bind9` avec la commande

```shell
# apt install bind9
```

Le fichier de configurion est situé dans le répertoire `/etc/bind/named.conf`

### Configuration

J'ai choisis de séparer les fihiers de zone et de les mettres dans une dossier
`zone` dans le dossier de configuration de bind.

Le fichier `named.conf`

```
// Include options
include "/etc/bind/named.conf.options";
include "/etc/bind/named.conf.default-zones";
zone "u-bordeaux.fr" {
        type master;
        file "/etc/bind/zones/db.u-bordeaux.fr.zone";
        allow-update { key rndc-key; };
};
zone "122.168.192.in-addr.arpa" {
        type master;
        file "/etc/bind/zones/inv.db.u-bordeaux.fr.zone";
        allow-update { key rndc.key; };
};

```

Et les fichiers de zones, en premier la zone "normale puis la zone inverse

```
$TTL    3600
@       IN      SOA     u-bordeaux.fr. root.u-borderaux.fr. (
                   2007010401           ; Serial
                         3600           ; Refresh [1h]
                          600           ; Retry   [10m]
                        86400           ; Expire  [1d]
                          600 )         ; Negative Cache TTL [1h]
;
@               IN      NS      ns
ns              IN      A       192.168.122.254
```

```
$TTL    604800
@       IN      SOA     u-bordeaux.com admin.u-bordeaux.fr (
                              1         ; Serial
                         604800         ; Refresh
                          86400         ; Retry
                        2419200         ; Expire
                         604800 )       ; Negative Cache TTL
;
@       IN      NS      localhost.      ; delete this line
254     IN      PTR     ns.u-bordeaux.fr
```

## Interfacer DHCP et DNS

Il s'agit maintenant de permettre la mise jour du serveur DNS avec 
