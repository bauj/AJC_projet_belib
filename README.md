# Traitement de données Belib'

Ce projet avait pour but de traiter des données en Open data dynamique sur une 
carte embarquée de type Raspberry Pi de manière automatique. Le data set 
choisi : [Statut des bornes Belib' parisiennes](https://parisdata.opendatasoft.com/explore/dataset/belib-points-de-recharge-pour-vehicules-electriques-disponibilite-temps-reel/information/?disjunctive.statut_pdc&disjunctive.arrondissement)
+ Deux besoins créés à partir du dataset :
    1. Disponibilité des bornes de stations en favoris définies autour d'une 
position dans un périmètre donné par un rayon de recherche
    2. Tracé de la moyenne horaire de la disponibilité des bornes dans ces 
stations favorites.  
 
+ Deux étapes majeures :
    1. Récolte, traitement et stockage des données : Python (urllib3, ujson, sqlite3)
    2. Génération de figures pour visualiser les données : C (sqlite3, libgd)  
+ Pour compléter ce projet : 
    + Ajout d'une carte géographique localisant les stations favorites, ainsi 
que la position de recherche (API Mapbox).
    + Ajout d'une fonctionnalité interactive : trouver des stations en 
spécifiant une adresse et un rayon de recherche -> renvoie un instantané de la 
disponibilité des bornes dans ces stations.

+ L'ensemble du projet est porté sur une carte de type Raspberry Pi3 (aarch64)
    +  Une distribution Linux est construite pour cette carte à l'aide de 
[Buildroot](https://buildroot.org), puis flashée sur la carte
    +  Buildroot génère une toolchain de cross-compilation permettant de compiler le code C et 
de générer les exécutables aarch64
    + Automatisation via sh script (+ crond): exécution du script Python 
de récupération des données (étape 1) et génération de figures avec exécutable 
cross-compilé (étape 2) toutes les heures  

+ L'ensemble des figures ainsi que la fonctionnalité interactive sont 
accessibles sur une page web hébergée à l'aide d'un serveur apache (httpd) 
installé sur la carte.  
Exemple d'interface contenant la mapbox et les figures obtenues avec libgd (langage C) :
  
![figures demo](figs_readme/figs_demo.png "Figures obtenues en C avec libgd")


Le détail des différentes étapes de dev peuvent être retrouvées dans la 
[Roadmap](Roadmap.md). Une documentation doxygen est disponible pour chacune 
des deux étapes du projet : doc python (etape 1) + doc C (etape 2). 
