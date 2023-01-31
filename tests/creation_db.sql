-- Creation d'une db pour le stockage des donnees quotidiennes belib
CREATE TABLE "Bornes" (
	"ID" INTEGER NOT NULL UNIQUE, 
	"last_updated" TEXT NOT NULL, 
	"id_pdc" TEXT NOT NULL, 
	"statut_pdc" TEXT NOT NULL, 
	"adresse_station" TEXT NOT NULL,
	"coordonneesxy" TEXT, 
	PRIMARY KEY("ID" AUTOINCREMENT)
);
