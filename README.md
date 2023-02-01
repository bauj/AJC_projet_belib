# Projet Formation AJC : Traitement de données Belib'

URL dataset : [Statut des bornes Belib' parisiennes](https://parisdata.opendatasoft.com/explore/dataset/belib-points-de-recharge-pour-vehicules-electriques-disponibilite-temps-reel/information/?disjunctive.statut_pdc&disjunctive.arrondissement)


## Récupération et traitement des données brutes :heavy_check_mark: 

### Création d'une base de données SQLite3 `belib_data.db` comprenant 4 tables

+ **Table Bornes** : contient l'ensemble des données des bornes Belib. 
Cette table est mise à jour quotidiennement à 17h20. Il s'agit d'une **table 
temporaire**, sauvegardée dans le cas où l'on souhaite exploiter plus de 
données dans la suite du projet. Elle n'est pas mise à jour sur la carte qemu.  
En-tête de la table :  

| ID | last_updated | id_pdc | statut_pdc | adresse_station | lon | lat |
| --- | --- | --- | --- | --- | --- | --- |

+ **Table General** : contient le nombre total de bornes associées à chaque
statut : disponible, occupé, en maintenance, etc ... Cette table est utilisée 
pour un affichage qui permet un aperçu global du service.  
En-tête de la table :  

| ID | date_recolte | disponible | occupe | en_maintenance | ... |
| --- | --- | --- | --- | --- | --- |


+ **Table Stations_fav** : contient les données de stations Belib 
enregistrées autour d'un lieu favori inséré dans l'application. Les données 
sont récupérées 3x par jour (9h40, 12h, 17h20) via **crontab** et permettent de
 tracer une évolution temporelle du statut de ces bornes pour ces 3 moments de 
la journée.   
*[Perspectives] Ajout de la possibilite d'ajouter des favoris.*  
*[Perspectives] On pourrait penser à inclure la possibilite de modifier les 
horaires de recuperation.*  
En-tête de la table :  

| ID | date_recolte | adresse_station | lon | lat | disponible | occupe | ... |
| --- | --- | --- | --- | --- | --- | --- | --- |

+ **Table Stations_live** : contient les données de stations Belib 
trouvées suite à la requête d'un utilisateur. Elle est nettoyée au lancement 
d'une requête pour ne garder que les résultats voulus.  
*[Perspectives] Tracer l'historique des bornes trouvées ? est-ce utile ?*  
En-tête de la table :  

| ID | date_recolte | adresse_station | lon | lat | disponible | occupe | ... |
| --- | --- | --- | --- | --- | --- | --- | --- |


### Récupération et injection des données dans la BDD (Base de Données) 

** DOC PYTHON A COMPLETER !! **

+ Récupération et injection des données `belib_data.db` dans chaque table de la 
bdd à l'aide du script python `recuperation_data_belib.py`. Bibliothèques 
python (modules) utilisées : 
    + **urllib3** : pour effectuer les requêtes GET sur les API [OpenDatasoft 
de ParisData](https://parisdata.opendatasoft.com/api/v2/console), [Adresse de 
data gouv](https://adresse.data.gouv.fr/api-doc/adresse), et [Static Images de 
mapbox](https://docs.mapbox.com/playground/static/).  

    + **ujson** : UltraJSON, pour le traitement des contenus JSON renvoyés par 
les requêtes. Bibliothèque de traitement de JSON ultrarapide et légère.  

    + **sqlite3**  : pour l'interfaçage avec la bdd `belib_data.db`.  

    + **datetime** : pour des traitements de chaines de type `date`.  

    + **argparse** : pour parser simplement les arguments entré via appel CLI.

### Options de récupération des données

+ Quatre options de récupération sont possibles avec le script 
`recuperation_data_belib.py` en fonction de la table de la bdd visée :
    + `-b` `--bornes` : récupération des données de l'ensemble des bornes et 
injection dans la **table Bornes** de la bdd.  

    + `-g` `--general` : récupération des données de l'ensemble des bornes 
groupées par statut et injection dans la **table General** de la bdd.  

    + `-f` `--favoris` : récupération des données des stations en favori et 
injection  dans la **table Stations_fav** de la bdd.  

    + `-l` `--live` : récupération des données des stations situées dans un 
rayon `<distance>` de l'adresse `<adresse>` entrée et injection dans la **table 
Stations_live** de la bdd.
        + `-a` `--adresse` <adresse>   : permet d'entrer une `<adresse>` sous 
la forme d'une chaine de caractères. 
        + `-d` `--distance` <distance> : permet d'entrer une `<distance>` sous
 la forme d'une chaine de caractères (de type "0.5km").


## Lecture bdd sqlite et plotting      :soon:
 
+ Section réalisée en C avec les bibliothèques sqlite3 et libgd. 
+ Plot de deux figures (*pour l'instant*) en utilisant la bibliothèque matplotlib
    + Fig 1 : Evolution temporelle du nombre de points de charge disponibles, 
occupés et en maintenance (courbes) pour les stations en favoris.
    + Fig 2 : Histogramme des statuts des points de charge des stations favs au
moment de la derniere recolte.

+ Fichiers figures enregistrés au format PNG.

+ Passer un coup de Valgrind + ElectricFence <!-- :heavy_check_mark: -->

## Update page HTML et CSS      :soon:

+ Un peu d'HTML et de CSS pour mettre en valeur les résultats, décrire le projet, etc ...

## Mise en place de l'ensemble du projet sur la carte embarquée (voir Overlay_BR)     :soon:

+ Recuperation des data :heavy_check_mark:

+ Bibliotheques utilisees :
    + **Python** : sqlite3, urllib3 (ssl et CA Certificates), ujson, sqlite3.
    + **C** : libgd, sqlite3.

