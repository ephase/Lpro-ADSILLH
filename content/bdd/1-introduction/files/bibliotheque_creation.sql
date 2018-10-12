CREATE SCHEMA Bibliotheque;
SET search_path TO Bibliotheque, public;
CREATE TYPE Bibliotheque.Sexe AS ENUM ('F', 'M');
CREATE TYPE Bibliotheque.Etat AS ENUM ('Neuf', 'Bon', 'Use');
CREATE TABLE Bibliotheque.Lecteur (
  num_lecteur serial NOT NULL,
  nom text NOT NULL,
  sexe Bibliotheque.Sexe NOT NULL,
  -- age devient date de naissance
  naissance date NOT NULL,
  ville text NOT NULL,
  -- clefs candidates
  PRIMARY KEY (num_lecteur)
);
CREATE TABLE Bibliotheque.Livre (
  isbn integer NOT NULL,
  titre text NOT NULL,
  auteur text NOT NULL,
  editeur text NOT NULL,
  annee interval YEAR NOT NULL,
  -- clefs candidates
  PRIMARY KEY (isbn)
);
CREATE TABLE Bibliotheque.Exemplaire (
  num_exemplaire serial NOT NULL,
  isbn integer NOT NULL,
  date_achat date NOT NULL,
  etat Bibliotheque.Etat NOT NULL,
  -- clefs candidates
  PRIMARY KEY (num_exemplaire),
  -- Clefs étrangères
  FOREIGN KEY (isbn) REFERENCES Bibliotheque.Livre(isbn)
);
CREATE TABLE Bibliotheque.Emprunt (
  num_exemplaire serial NOT NULL,
  num_lecteur serial NOT NULL,
  date_emprunt date NOT NULL,
  -- clefs candidates
  PRIMARY KEY (num_exemplaire, num_lecteur),
  -- Clefs étrangères
  FOREIGN KEY (num_exemplaire) REFERENCES Bibliotheque.Exemplaire(num_exemplaire),
  FOREIGN KEY (num_lecteur) REFERENCES Bibliotheque.Lecteur(num_lecteur)
);
