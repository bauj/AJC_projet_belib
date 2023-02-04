-- Data d'une station en fav
-- SELECT date_recolte,disponible,occupe,en_maintenance,inconnu FROM 'Stations_fav' WHERE adresse_station = '108 Rue Balard 75015 Paris';

-- Liste des adresses en favoris 
SELECT DISTINCT(adresse_station) FROM 'Stations_fav';

-- Nb stations en favoris
-- SELECT COUNT(DISTINCT adresse_station) FROM 'Stations_fav';