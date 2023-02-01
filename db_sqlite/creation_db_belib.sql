-- Creation d'une db pour le stockage des donnees quotidiennes belib
-- Table Bornes pour l'ensemble des donnes, recuperees une fois par jour
CREATE TABLE "Bornes" (
	"ID" INTEGER NOT NULL UNIQUE, 
	"last_updated" TEXT NOT NULL, 
	"id_pdc" TEXT NOT NULL, 
	"statut_pdc" TEXT NOT NULL, 
	"adresse_station" TEXT NOT NULL,
	"lon" REAL NOT NULL, 
	"lat" REAL NOT NULL, 
	PRIMARY KEY("ID" AUTOINCREMENT)
);

-- Table General pour un apercu global du statut de l'ensemble des bornes
CREATE TABLE "General" (
	"ID" INTEGER NOT NULL UNIQUE, 
	"date_recolte" TEXT NOT NULL, 
	"disponible" INTEGER NOT NULL, 
	"occupe" INTEGER NOT NULL, 
	"en_maintenance" INTEGER NOT NULL, 
	"inconnu" INTEGER NOT NULL, 
	"supprime" INTEGER NOT NULL, 
	"reserve" INTEGER NOT NULL, 
	"en_cours_mes" INTEGER NOT NULL, 
	"mes_planifiee" INTEGER NOT NULL, 
	"non_implemente" INTEGER NOT NULL, 
	PRIMARY KEY("ID" AUTOINCREMENT)
);

-- Table Stations_favf pour les stations en favori  autour de la position
-- enregistree
CREATE TABLE "Stations_fav" (
	"ID" INTEGER NOT NULL UNIQUE, 
	"date_recolte" TEXT NOT NULL, 
	"adresse_station" TEXT NOT NULL,
	"lon" REAL NOT NULL,
	"lat" REAL NOT NULL,
	"disponible" INTEGER NOT NULL, 
	"occupe" INTEGER NOT NULL, 
	"en_maintenance" INTEGER NOT NULL, 
	"inconnu" INTEGER NOT NULL, 
	"supprime" INTEGER NOT NULL, 
	"reserve" INTEGER NOT NULL, 
	"en_cours_mes" INTEGER NOT NULL, 
	"mes_planifiee" INTEGER NOT NULL, 
	"non_implemente" INTEGER NOT NULL, 
	PRIMARY KEY("ID" AUTOINCREMENT)
);

-- Table Stations_live pour les stations autour de la position demandee
CREATE TABLE "Stations_live" (
	"ID" INTEGER NOT NULL UNIQUE, 
	"date_recolte" TEXT NOT NULL, 
	"adresse_station" TEXT NOT NULL,
	"lon" REAL NOT NULL,
	"lat" REAL NOT NULL,
	"disponible" INTEGER NOT NULL, 
	"occupe" INTEGER NOT NULL, 
	"en_maintenance" INTEGER NOT NULL, 
	"inconnu" INTEGER NOT NULL, 
	"supprime" INTEGER NOT NULL, 
	"reserve" INTEGER NOT NULL, 
	"en_cours_mes" INTEGER NOT NULL, 
	"mes_planifiee" INTEGER NOT NULL, 
	"non_implemente" INTEGER NOT NULL, 
	PRIMARY KEY("ID" AUTOINCREMENT)
);
