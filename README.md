# Projet Formation AJC : Traitement de données Belib'

URL dataset : [Statut des bornes Belib' parisiennes](https://parisdata.opendatasoft.com/explore/dataset/belib-points-de-recharge-pour-vehicules-electriques-disponibilite-temps-reel/information/?disjunctive.statut_pdc&disjunctive.arrondissement)


### Récupération des données  :heavy_check_mark:

+ Récupération des données a l'aide du **shell script** `get_data.sh` construisant la requete pour l'API open data Paris
+ Deux fichiers de données au format `JSON` sont récupérés quotidiennement :
    + `raw_data_tot_belib_<DDJ>.json` : L'ensemble des données (/exports) obtenu à l'aide de de la fonction : `Get_all_data()`.
L'API limite les requetes à 100 records seulement. Le téléchargement de l'ensemble des données est nécessaire si on souhaite exploiter plus de données par la suite.
Taille du fichier : ~1.3Mo.

    + `raw_data_nb_bornes_belib_<DDJ>.json` : Données obtenues à l'aide de de la fonction : `Get_grouped_by_status_data()`.
Fichier contenant les résultats de base qui seront exploités pour le moment. Taille du fichier : ~1.5Ko.

    + ** AJOUTER MODELE JSON ** 

+ Les données seront récupérées tous les jours à 17h20 (**crontab**)


### Traitement des données brutes et bdd sqlite :heavy_check_mark:

+ **En C** : parsing du fichier `.json` du jour à l'aide d'une bibliothèque faite pour (voir [librairie JSMN](https://github.com/zserge/jsmn)). 

+ Traitement des données : Lecture et parsing du fichier `raw_data_nb_bornes_belib_<DDJ>.json`. Le champ `nb_bornes` associé à chaque type de `statut_pdc` est récupéré et stocké dans une bdd SQLite. 

+ Construction / Insertion bdd sqlite. Liste des noms de colonnes :
    + `DATERECOLTE`  |  `DISPONIBLE`  | `OCCUPE`  | `MAINTENANCE`  |  `INCONNU` ....

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

+ Passer un coup de Valgrind + ElectricFence

### Lecture bdd sqlite et plotting      :soon:
 
+ **En Python** : lecture de la bdd via bibliothèque python sqlite3
+ Plot de deux figures (*pour l'instant*) en utilisant la bibliothèque matplotlib
    + Fig 1 : Evolution temporelle (quotidienne) du nombre de points de charge en maintenance, disponibles et occupés
    + Fig 2 : Répartition des statuts des points de charge du jour.
+ Fichiers figures enregistrés au format PDF ou PNG.

### Update page HTML et CSS      :soon:

+ Un peu d'HTML et de CSS pour mettre en valeur les résultats, décrire le projet, etc ...

### Mise en place de l'ensemble sur la carte embarque (voir Overlay_BR)     :soon:

**IMPORTANT** : Prendre en compte la compilation des librairies si non déjà disponibles dans buildroot.

