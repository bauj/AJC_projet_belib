# Projet Formation AJC : Traitement de données Belib'

URL dataset : [Statut des bornes Belib' parisiennes](https://parisdata.opendatasoft.com/explore/dataset/belib-points-de-recharge-pour-vehicules-electriques-disponibilite-temps-reel/information/?disjunctive.statut_pdc&disjunctive.arrondissement)


### Récupération des données 

+ Récupération des données a l'aide d'un **shell script** construisant la requete pour l'API open data Paris
    + La requete est realisee pour l'ensemble des données (/exports) à l'aide du script `get_data.sh`.
    + ATTENTION : retirer données 2022

+ Données enregistrées quotidiennement au format JSON dans un fichier `raw_data_belib_DDJ.json` (DDJ = Date Du Jour).

+ Fichier assez lourd : 1.3Mo. L'API limite la requete à 100 records seulement, téléchargement de l'ensemble des données nécessaire.

+ Les données seront récupérées tous les jours à 17h20 (**crontab**)


### Traitement des données brutes et bdd sqlite

+ **En C** : parsing du fichier .json du jour à l'aide d'une bibliothèque faite pour (voir [librairie JSMN](https://github.com/zserge/jsmn)). 

+ Traitement des données : on ne recupere que les records du jour.

+ Calcul du nombre de points de charge pour chaque `statut_pdc` (i.e. en maintenance, disponible, occupé ...)

+ Construction / Insertion bdd sqlite simple. Liste des noms de colonnes :
    + `date`  |  `nb_maintenance`  | `nb_occupe`  | `nb_disponible`  |  `nb_inconnu` ....

+ *INFO* -- Statuts points de charge :  
    + Disponible
    + Pas implémenté
    + Occupé (en charge)
    + En cours de mise en service
    + En maintenance
    + Mise en service planifiée
    + Supprimé
    + Réservé
    + Inconnu

+ Une fois le fichier json traité, on le supprime (lourd : 1.3Mo)

### Lecture bdd sqlite et plotting

+ **En Python** : lecture de la bdd via bibliothèque python sqlite3
+ Plot de deux figures (*pour l'instant*) en utilisant la bibliothèque matplotlib
    + Fig 1 : Evolution temporelle (quotidienne) du nombre de points de charge en maintenance, disponibles et occupés
    + Fig 2 : Répartition des statuts des points de charge du jour.
+ Fichiers figures enregistrés au format PDF ou PNG.

### Update page HTML et CSS

+ Un peu d'HTML et de CSS pour mettre en valeur les résultats, décrire le projet, etc ...


**IMPORTANT** : Prendre en compte la compilation des librairies si non déjà disponibles dans buildroot.

