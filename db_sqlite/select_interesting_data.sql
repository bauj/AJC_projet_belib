-- Dates recolte des stations en fav
SELECT DISTINCT(date_recolte) FROM 'Stations_fav';

-- Statuts station 1
SELECT disponible, occupe, en_maintenance, inconnu FROM 'Stations_fav' WHERE adresse_station = '108 Rue Balard 75015 Paris';

-- Liste des adresses en favoris 
-- SELECT DISTINCT(adresse_station) FROM 'Stations_fav';

-- Nb stations en favoris
-- SELECT COUNT(DISTINCT adresse_station) FROM 'Stations_fav';

-- Nb rows par Stations_fav
-- SELECT COUNT(DISTINCT date_recolte) FROM 'Stations_fav';