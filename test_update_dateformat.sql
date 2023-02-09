-- UPDATE Stations_fav SET date_recolte = substr(date_recolte, 0, 14) || ':' || substr(date_recolte, 15, 2) || 'Z';

-- SELECT strftime('%H', date_recolte) FROM Stations_fav;

-- SELECT COUNT(DISTINCT(strftime('%H', date_recolte))) as Hour FROM Stations_fav;

SELECT (strftime('%H', date_recolte)) as Hour FROM Stations_fav GROUP BY (strftime('%H', date_recolte));

-- SELECT strftime('%H', date_recolte) as Hour,
-- 		AVG(disponible) as Avg_dispo FROM Stations_fav
-- 		WHERE adresse_station = '108 Rue Balard 75015 Paris' 
-- 		GROUP BY strftime('%H', date_recolte);
-- 		
-- SELECT strftime('%H', date_recolte) as Hour,
-- 		AVG(disponible) as Avg1 FROM Stations_fav
-- 		WHERE adresse_station = '109 Avenue Félix Faure 75015 Paris' 
-- 	GROUP BY strftime('%H', date_recolte);
-- 		
-- SELECT strftime('%H', date_recolte) as Hour,
-- 		AVG(disponible) as Avg1 FROM Stations_fav
-- 		WHERE adresse_station = '19 Rue Leblanc 75015 Paris' 
-- 		GROUP BY strftime('%H', date_recolte)	;
-- 
-- SELECT strftime('%H', date_recolte) as Hour,
-- 		AVG(disponible) as Avg1 FROM Stations_fav
-- 		WHERE adresse_station = '44 Rue Sébastien Mercier 75015 Paris' 
-- 	GROUP BY strftime('%H', date_recolte)	;
-- 
-- SELECT strftime('%H', date_recolte) as Hour,
-- 		AVG(disponible) as Avg1 FROM Stations_fav
-- 		WHERE adresse_station = '56 Rue Lacordaire 75015 Paris' 
-- 		GROUP BY strftime('%H', date_recolte)	;

