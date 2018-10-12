-- schema de Base de données pour la gestion d'une compagnie aérienne.
CREATE SCHEMA aerocremi;
SET search_path TO aerocremi, public;

CREATE TABLE aerocremi.avion(
    -- Je pars du principe que le numéro de série est donné par le fabricant
    -- de l'avion et qu'il ne contient que des chiffres. Il ne peut donc pas
    -- être du type serial.
    num_serie integer NOT NULL,
    type text NOT NULL,
    capacite integer NOT NULL,
    PRIMARY KEY (num_serie)
);

CREATE TABLE aerocremi.pilote(
    matricule serial NOT NULL,
    nom text NOT_NULL,
    anciennete date,
    PRIMARY KEY (matricule)
);

CREATE TABLE aerocremi.vol(
    num_vol text NOT NULL,
    -- Je part du principe qu'il faut gérer les times zones sur les horaires
    -- de vol (logique pour une compagnie aérienne)
    -- https://www.postgresql.org/docs/current/static/datatype-datetime.html
    heure_depart time with time zone NOT NULL,
    heure_arrivee time with time zone NOT NULL,
    PRIMARY_KEY (num_vol)
);

CREATE TABLE aerocremi.planning(
    num_planning serial NOT NULL,
    num_vol text NOT NULL,
    matricule integer NOT NULL,
    date_vol date NOT NULL
    PRIMARY KEY (num_planning),
    FOREIGN KEY num_vol REFERENCES aerocremi.vol(num_vol),
    FOREIGN KEY num_avion REFERENCES aerocremi.avion(num_serie),
    FOREIGN KEY matricule REFERENCES aecrocremi.pilote(matricule)
);
