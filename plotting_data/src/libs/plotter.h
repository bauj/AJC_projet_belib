/* ----------------------------------------------------------------------------
*  Bibliotheque de plot basée sur libgd developpee dans le but de visualiser
* le dataset recensant les bornes Belib sur Paris.
*  
*  Author : Juba Hamma. 2023.
* ---------------------------------------------------------------------------- 
*/

#ifndef PLOTTER_H
#define PLOTTER_H

#include "consts.h"
#include <stdlib.h>
#include "getter.h"
#include <gd.h>
#include <math.h>

#define PI 3.141592

/* --------------------------------------------------------------------------- */
/**
 * @brief Enumeration permettant d'atteindre les differentes polices pour chaque
 * élément de la figure : labels, titres, legende, annotation ...
 * 
 */
typedef enum {label_f, annotation_f, title_f, ticklabel_f, subtitle_f, leg_f} fontsFig;

/* --------------------------------------------------------------------------- */
/**
 * @brief Structure stockant des donnees par categorie et les associent a des 
 * couleurs dans le but de tracer un BarPlot les représentant.
 * Dans le cadre de Belib : un BarData par station.
 * 
 */
typedef struct BarData_s {
    size_t nb_ctg;        /**< Nombre de categories*/
    char **ctg_names;     /**< Labels des categories*/
    size_t nb_tot;        /**< Nombre total d'éléments dans l'ensemble des ctg*/
    int *nb_in_ctg;        /**< Vecteur contenant le nombre d'elements par ctg */
    int (*colors)[3];      /**< Vecteur contenant une couleur pour chaque ctg*/
    char* label;           /**< Label associé au BarData */
    int idx;                /**< Index donné lorsqu'ajouté à la figure (pour posX)*/
} BarData; 

/* --------------------------------------------------------------------------- */
// Structures -> Check l'ordre des arguments pour la contiguite memoire
/* --------------------------------------------------------------------------- */

/**
 * @brief Structure definissant un style de plot (ligne, point)
 * 
 */
typedef struct LineStyle_s {
    char style;     /**< Style de plot : '-' : line, ':' dashed,
                                          '' : point seulement */
    int  w;         /**< Epaisseur du trait*/
    char marker;    /**< Type de marker : 's' : carré, 'o' : cercle,
                                           '' : pas de marker */
    int  ms;        /**< Markersize*/
    int  color[3];  /**< Couleur rgb*/
} LineStyle;

/* --------------------------------------------------------------------------- */
/**
 * @brief Structure associant un vecteur de donnees labelisé (int) à un LineStyle
 * 
 */
typedef struct LineData_s {
    size_t len_data;      /**< Taille du vecteur de data */
    int *x;               /**< Vecteur de data X */
    int *y;               /**< Vecteur de data Y */
    int max_X;            /**< Maximum des valeurs X */
    int max_Y;            /**< Maximum des valeurs Y */
    char* label;
    LineStyle *linestyle;  /**< LineStyle */
} LineData; 

/* --------------------------------------------------------------------------- */
/**
 * @brief Structure associant un vecteur de donnees labelisé (float) à un LineStyle
 * 
 */
typedef struct fLineData_s {
    size_t len_data;      /**< Taille du vecteur de data */
    int *x;               /**< Vecteur de data X */
    float *y;               /**< Vecteur de data Y */
    int  max_X;            /**< Maximum des valeurs X */
    float fmax_Y;            /**< Maximum des valeurs Y */
    char* label;
    LineStyle *linestyle;  /**< LineStyle */
} fLineData; 


/* --------------------------------------------------------------------------- */
/**
 * @brief Structure stockant les infos d'une police : chemin, taille et couleur
 * 
 */
typedef struct Font_s {
    char* path;         /**< Chemin vers le fichier police*/
    int size;           /**< Taille de la police*/
    int color[3];       /**< Couleur de la police (rgb) */
} Font;


/* --------------------------------------------------------------------------- */
/**
 * @brief Structure stockant un ensemble de structures (Struct bardata, linedata ou flinedata)
 *  et de données relatives 
 * à la création d'une figure 
 * 
 */
typedef struct Figure_s {
    gdImage *img;        /**< Pointeur sur objet gdImage */
    size_t nb_linedata;  /**< Nombre de linedata dans la figure */
    size_t nb_flinedata;  /**< Nombre de linedata dans la figure */
    size_t nb_bardata;  /**< Nombre de linedata dans la figure */
    fLineData **flinedata;  /**< Vecteur de fLineData */
    LineData **linedata;  /**< Vecteur de LineData */
    BarData **bardata;   /**< Vecteur de BarData */
    Font fonts[6];       /**< Fonts used in the fig*/
    int max_X;           /**< max de l'ensemble des max_X de linedata[] */
    int max_Y;           /**< max de l'ensemble des max_Y de linedata[] */
    float fmax_Y;        /**< max de l'ensemble des max_Y (float) de linedata[] */
    int padX[2];         /**< Padding gauche/droite pour definir canvas (cvs) et orig */
    int padY[2];         /**< Padding haut/bas pour definir canvas (cvs) et orig */
    int orig[2];         /**< Coordonnees de l'origine des plots */    
    int margin[2];       /**< Marge droite et haut (evite de toucher les bords)*/    
    int color_bg[3];     /**< Couleur du fond de la figure */
    int color_cvs_bg[3]; /**< Couleur du fond du canvas */
    int color_axes[3];   /**< Couleur des axes*/
} Figure;

/* --------------------------------------------------------------------------- */
// Declaration fonctions
/* --------------------------------------------------------------------------- */

/**
 * @brief Renvoie le max entre 2 entiers
 * 
 * @param x Entier 1
 * @param y Entier 2
 * @return int Max des deux entiers
 */
int Max_int(int x, int y);

/**
 * @brief Renvoie le min entre 2 entiers
 * 
 * @param x Entier 1
 * @param y Entier 2
 * @return int Min des deux entiers
 */
int Min_int(int x, int y);

/**
 * @brief 
 * 
 * @param x_array Tableau d'entiers 
 * @param n Taille du tableau d'entiers
 * @return int Valeur max dans le tableau
 */
int Maxval_array(const int x_array[], size_t n);


/**
 * @brief 
 * 
 * @param x_array Tableau d'entiers 
 * @param n Taille du tableau d'entiers
 * @return int Valeur min dans le tableau
 */
int Minval_array(const int x_array[], size_t n);


/**
 * @brief Renvoie le max entre 2 float
 * 
 * @param x Float 1
 * @param y Float 2
 * @return float Max des deux float
 */
float Max_float(float x, float y);

/**
 * @brief Renvoie le max d'un tableau de float
 * 
 * @param x_array Tableau de float
 * @param n Taille du tableau de float
 * @return float Valeur max dans le tableau
 */
float fMaxval_array(const float x_array[], size_t n);

/**
 * @brief Transforme les degrés en radians
 * 
 * @param angle Angle en degrés
 * @return float Angle en radians
 */
float Deg2rad(float angle);

/**
 * @brief Recupere la valeur de la couleur (int) associée à un vecteur d'entier
 * rgb (0-255) 
 * 
 * @param im_fig Pointeur vers un objet de type gdImage
 * @param couleur Tableau de 3 entiers rgb (0-255)
 * @return int Valeur entiere associée à la couleur dans im_fig
 */
int GetCouleur(gdImagePtr im_fig, const int couleur[3]);

/**
 * @brief Sauvegarde une figure au format png
 * 
 * @param fig Pointeur vers un objet de type Figure
 * @param dir_figures Dossier de sauvegarde des figures (output)
 * @param filename_fig Nom du fichier image à sauvegarder
 */
void Save_to_png(Figure *fig, const char *dir_figures, const char *filename_fig);


/**
 * @brief Permet d'initialiser un objet de type Figure 
 * 
 * @param fig Pointeur vers un objet de type Figure
 * @param figsize Vecteur de deux entiers contenant la taille de la figure en pixel selon X et Y (idx 0 et 1 resp.)
 * @param padX Vecteur de deux entiers contenant la taille en pixel du padding a gauche et a droite de la figure (idx 0 et 1 resp.)
 * @param padY Vecteur de deux entiers contenant la taille en pixel du padding en haut et en bas de la figure (idx 0 et 1 resp.)
 * @param margin Vecteur de deux entiers contenant la taille en pixel de la marge aux extremites du canvas, i.e. de la zone de dessin
 * @param wAxes Caractere 'y' ou 'n'. Spécifier si l'on veut afficher des axes autour de la figure ou pas.
 */
void Init_figure(Figure *fig, int figsize[2],int padX[2], int padY[2], int margin[2], char wAxes);


/**
 * @brief Crée le fond et le canvas (zone de dessin) de la figure. Utilisé dans Init_figure.
 * 
 * @param fig Pointeur vers un objet de type Figure
 * @param color_bg Couleur du fond de la figure : vecteur de 3 entiers (0-255) 
 * @param color_canvas_bg Couleur du fond du canvas (zone de dessin) : vecteur de 3 entiers (0-255)
 */
void Make_background(Figure *fig,const int color_bg[3], const int color_canvas_bg[3]);

/**
 * @brief Crée le support des axes de la figure
 * 
 * @param fig Pointeur vers un objet de type Figure
 * @param couleur Couleur des axes de la figure : vecteur de 3 entiers (0-255) 
 */
void Make_support_axes(Figure *fig, const int couleur[3]);


/**
 * @brief Permet de modifier la couleur de fond d'une figure
 * 
 * @param fig Pointeur vers un objet de type Figure
 * @param color Couleur du fond de la figure : vecteur de 3 entiers (0-255) 
 */
void Change_fig_bg(Figure *fig, int color[3]);

/**
 * @brief Permet de modifier la couleur de fond d'un canvas (zone de dessin) d'une figure
 * 
 * @param fig Pointeur vers un objet de type Figure
 * @param color Couleur du fond du canvas de la figure : vecteur de 3 entiers (0-255) 
 */
void Change_fig_cvs_bg(Figure *fig, int color[3]);


/**
 * @brief Permet de modifier la couleur des axes
 * 
 * @param fig Pointeur vers un objet de type Figure
 * @param color Couleur des axes de la figure : vecteur de 3 entiers (0-255) 
 */
void Change_fig_axes_color(Figure *fig, int color[3]);

/**
 * @brief Permet de modifier la police d'un des éléments de la figure (voir enum fontsFig)
 * 
 * @param fig Pointeur vers un objet de type Figure
 * @param textType Type d'élément de figure à modifier : index de l'enum fontsFig (labels, title, ...)
 * @param path_f Chemin vers la police souhaitée
 */
void Change_font(Figure *fig, int textType, char* path_f);

/**
 * @brief Permet de modifier la police d'un des éléments de la figure (voir enum fontsFig)
 * 
 * @param fig Pointeur vers un objet de type Figure
 * @param textType Type d'objet de figure à modifier : index de l'enum fontsFig (labels, title, ...)
 * @param size Taille de la police
 */
void Change_fontsize(Figure *fig, int textType, int size);

/**
 * @brief Permet de modifier la couleur de la police d'un des éléments de la figure (voir enum fontsFig)
 * 
 * @param fig Pointeur vers un objet de type Figure
 * @param textType Type d'objet de figure à modifier : index de l'enum fontsFig (labels, title, ...)
 * @param color Couleur de la police : vecteur de 3 entiers (0-255) 
 */
void Change_fontcolor(Figure *fig, int textType, int color[3]);

/**
 * @brief Initialise un object de type LineStyle
 * 
 * @param linestyle Pointeur vers objet de type LineStyle
 * @param style Type de trait : '-' pour trait plein, ':' pour pointillés, ' ' pour pas de trait
 * @param color Couleur du style: vecteur de 3 entiers (0-255) 
 * @param width Epaisseur dans le cas d'un trait
 * @param marker Type de marker : 'o' pour des points circulaires
 * @param ms Taille du marker
 * @note Seul le type de marker 'o' est disponible pour le moment
 */
void Init_linestyle(LineStyle *linestyle, char style,const int color[3], int width, char marker, int ms);

/**
 * @brief Initialise un objet de type BarData, utilisé pour un Barplot multi-catégories
 * 
 * @param bardata Pointeur vers un objet de type BarData
 * @param nb_ctg Nombre de catégories dans les données
 * @param labels_ctg Labels associés à chaque catégorie
 * @param nb_tot Nombre total d'éléments dans les data
 * @param nb_in_ctg Nombre d'éléments par catégorie (tableau de taille nb_ctg)
 * @param colors Couleurs associées à chaque catégorie (tableau de taille nb_ctg)
 * @param label Label associé a l'object BarData
 */
void Init_bardata(BarData *bardata, int nb_ctg, char *labels_ctg[nb_ctg],int nb_tot, int nb_in_ctg[nb_ctg],int colors[nb_ctg][3], char* label);

/**
 * @brief Initialise un objet de type LineData, utilisé pour un lineplot de données entières
 * 
 * @param linedata Pointeur vers un objet de type LineData
 * @param len_data Nombre d'éléments (X,Y) dans les données
 * @param ptx Vecteur des X
 * @param pty Vecteur des Y
 * @param label Label associé au LineData, retrouvé dans la légende
 * @param linestyle Objet de type LineStyle associé au LineData
 */
void Init_linedata(LineData *linedata, int len_data, int ptx[], int pty[],char* label, LineStyle *linestyle);

/**
 * @brief Initialise un objet de type fLineData, utilisé pour un lineplot de données de type float en Y
 * 
 * @param flinedata Pointeur vers un objet de type fLineData
 * @param len_data Nombre d'éléments (X,Y) dans les données
 * @param ptx Vecteur des X 
 * @param pty Vecteur des Y 
 * @param label Label associé au fLineData, retrouvé dans la légende
 * @param linestyle Objet de type LineStyle associé au LineData
 */
void Init_flinedata(fLineData *flinedata, int len_data, int ptx[], float pty[],char* label, LineStyle *linestyle);

/**
 * @brief Affiche un point dans la zone de dessin d'une figure
 * 
 * @param fig Pointeur vers un objet de type Figure
 * @param x1 Abcisse du point
 * @param y1 Ordonnée du point
 * @param linestyle Objet de type LineStyle (type de point)
 */
void PlotPoint(Figure *fig, const int x1, const int y1,LineStyle *linestyle);

/**
 * @brief Trace le contenu d'un LineData dans la zone de dessin d'une figure
 * 
 * @param fig Pointeur vers un objet de type Figure
 * @param linedata Objet de type LineData
 */
void PlotLine(Figure *fig, LineData *linedata);

/**
 * @brief Trace le contenu d'un fLineData dans la zone de dessin d'une figure
 * 
 * @param fig Pointeur vers un objet de type Figure
 * @param flinedata Objet de type fLineData
 */
void PlotFLine(Figure *fig, fLineData *flinedata);

/**
 * @brief Trace le contenu d'un BarData dans la zone de dessin d'une figure
 * 
 * @param fig Pointeur vers un objet de type Figure
 * @param bardata Objet de type BarData
 * @param wlabels Caractère 'y' ou 'n' pour afficher ou non le nombre d'éléments dans chaque catégorie a coté du plot
 */
void PlotBarplot(Figure *fig, BarData *bardata, char wlabels);

/**
 * @brief Fonction interne permettant de changer le référentiel des données d'entrée selon X (int) pour qu'il s'adapte à la zone de dessin
 * Cas d'un fLineData. Renvoie un vecteur d'entier (pixels).
 *  
 * @param fig Pointeur vers objet de type Figure
 * @param len_pts Taille du vecteur X
 * @param pts Vecteur d'entier X
 * @return int* Vecteur d'entier X dans le référentiel de la zone de dessin (en pixel)
 */
int *Transform_fdataX_to_plot(Figure *fig, size_t len_pts,const int pts[]);

/**
 * @brief Fonction interne permettant de changer le référentiel des données d'entrée selon Y (float) pour qu'il s'adapte à la zone de dessin.
 * Cas d'un fLineData. Renvoie un vecteur d'entier (pixels).
 * 
 * @param fig Pointeur vers objet de type Figure
 * @param len_pts Taille du vecteur Y
 * @param pts Vecteur de float Y
 * @return int* Vecteur d'entier Y dans le référentiel de la zone de dessin
 */
int *Transform_fdataY_to_plot(Figure *fig, size_t len_pts,const float pts[]);

/**
 * @brief Fonction interne permettant de changer le référentiel des données d'entrée d'un BarData pour qu'il s'adapte à la zone de dessin.
 * Cas d'un BarData. Renvoie un vecteur d'entier (pixels).
 * 
 * @param fig Pointeur vers un objet de type Figure
 * @param nb_ctg Nombre de catégorie dans le Bardata
 * @param pts 
 * @return int* 
 * @warning Malloc fait, desallocation en interne
 * @note nb_ctg peut normalement etre recupéré à partir de fig->BarData
 */
int *Transform_data_to_plot_bar(Figure *fig, size_t nb_ctg,const int pts[]);

/**
 * @brief Fonction interne permettant de changer le référentiel des données d'entrée selon X (int) pour qu'il s'adapte à la zone de dessin
 * Cas d'un LineData. Renvoie un vecteur d'entier (pixels).
 * 
 * @param fig Pointeur vers objet de type Figure
 * @param len_pts Taille du vecteur X
 * @param pts Vecteur d'entier X
 * @param pts_dessin Pointeur vers un tableau d'entier dans le réferentiel de la zone de dessin (en pixel)
 */
void Transform_dataX_to_plot(Figure *fig, size_t len_pts,const int pts[], int* pts_dessin);

/**
  * @brief Fonction interne permettant de changer le référentiel des données d'entrée selon Y (int) pour qu'il s'adapte à la zone de dessin
 * Cas d'un LineData. Renvoie un vecteur d'entier (pixels).
 * 
 * @param fig Pointeur vers objet de type Figure
 * @param len_pts Taille du vecteur Y
 * @param pts Vecteur d'entier Y
 * @param pts_dessin Pointeur vers un tableau d'entier dans le réferentiel de la zone de dessin (en pixel)
 */
void Transform_dataY_to_plot(Figure *fig, size_t len_pts,const int pts[], int* pts_dessin);

/**
 * @brief Fonction interne appelant Transform_dataX_to_plot ou Transform_dataY_to_plot en fonction de la direction spécifiée
 * 
 * @param fig Pointeur vers objet de type Figure
 * @param len_pts Taille du vecteur X ou Y
 * @param pts Vecteur d'entier X ou Y en input 
 * @param direction Caractere 'x' ou 'y'
 * @return int* Tableau d'entier dans le réferentiel de la zone de dessin (en pixel)
 */
int *Transform_data_to_plot(Figure *fig, size_t len_pts,const int pts[], char direction);


/**
 * @brief Ajoute un objet de type LineData à la Figure. Permet un update des maxima et de gérer les tracés
 * sur la figure.
 * 
 * @param fig Pointeur vers objet de type Figure
 * @param linedata Pointeur vers objet de type LineData à ajouter à la figure
 */
void Add_line_to_fig(Figure *fig, LineData *linedata);

/**
  * @brief Ajoute un objet de type fLineData à la Figure. Permet un update des maxima et de gérer les tracés
 * sur la figure.
 * 
 * @param fig Pointeur vers objet de type Figure
 * @param flinedata Pointeur vers objet de type fLineData à ajouter à la figure
 */
void Add_fline_to_fig(Figure *fig, fLineData *flinedata);

/**
 * @brief Ajoute un objet de type BarData à la Figure. Permet un update des maxima et de gérer les tracés
 * sur la figure.
 * 
 * @param fig Pointeur vers objet de type Figure
 * @param bardata Pointeur vers objet de type BarData à ajouter à la figure
 */
void Add_barplot_to_fig(Figure *fig, BarData *bardata);

/**
 * @brief Ajoute une annotation sur la figure
 * 
 * @param fig Pointeur vers objet de type Figure
 * @param text Texte de l'annotation
 * @param decalage_X Decalage selon X en pixel (vers la droite)
 * @param decalage_Y Decalage selon Y en pixel (vers le haut)
 */
void Make_annotation(Figure *fig, char* text,int decalage_X, int decalage_Y);

/**
 * @brief Ajoute un titre à la figure, sous l'axe des abcisses par défaut
 * 
 * @param fig Pointeur vers objet de type Figure
 * @param title Texte du titre
 * @param decalage_X Decalage selon X en pixel (vers la droite)
 * @param decalage_Y Decalage selon Y en pixel (vers le haut)
 * @return int* Vecteur de 8 entiers correspondant aux paires de coordonnées x,y des points so,se,ne,no
 */
int *Make_title(Figure *fig, char* title,int decalage_X, int decalage_Y);

/**
 * @brief Ajoute un titre à la figure, sous l'axe des abcisses par défaut
 * 
 * @param fig Pointeur vers objet de type Figure
 * @param subtitle Texte du sous-titre
 * @param bbox_title Vecteur de 8 entiers correspondant a la bbox du titre. Paires de coordonnées x,y des points so,se,ne,no de la bbox du titre.
 * @param decalage_X Decalage selon X en pixel (vers la droite)
 * @param decalage_Y Decalage selon Y en pixel (vers le haut)
 */
void Make_subtitle(Figure *fig, char* subtitle,int bbox_title[8], int decalage_X, int decalage_Y);

/**
 * @brief Fonction permettant de construire une chaine de caractères "Du ... au ..." à partir de deux objets de type Date.
 * 
 * @param date1 Objet 1 de type Date
 * @param date2 Objet 2 de type Date
 * @param str_out Chaine de caractère du type "Du <Date1> au <Date2>"
 * @note Fonction à déplacer dans traitement.h
 */
void Const_str_dudate1_audate2(Date *date1, Date *date2, char* str_out);

/**
 * @brief Ajoute un label à l'axe des abcisses
 * 
 * @param fig Pointeur vers objet de type figure
 * @param xlabel Texte du label X
 * @param decalage_X Decalage selon X en pixel (vers la droite)
 * @param decalage_Y Decalage selon Y en pixel (vers le haut)
 */
void Make_xlabel(Figure *fig, char* xlabel,int decalage_X, int decalage_Y);

/**
 * @brief Ajoute un label à l'axe des ordonnées
 * 
 * @param fig Pointeur vers objet de type figure
 * @param ylabel Texte du label Y
 * @param decalage_X Decalage selon X en pixel (vers la droite)
 * @param decalage_Y Decalage selon Y en pixel (vers le haut)
 */
void Make_ylabel(Figure *fig, char* ylabel,int decalage_X, int decalage_Y);

/**
 * @brief Crée la légende pour un barplot
 * 
 * @param fig Pointeur vers un objet de type figure
 * @param decal_X Decalage selon X en pixel (vers la droite)
 * @param decal_Y Decalage selon Y en pixel (vers le haut)
 * @param ecart Ecart vertical entre les éléments de la légende
 * @note Limité à 4 catégories pour le moment. Servi dans le cas de Belib.
 */
void Make_legend_barplot(Figure *fig,int decal_X, int decal_Y, int ecart);

/**
 * @brief Crée la légende pour un plot
 * 
 * @param fig Pointeur vers un objet de type figure
 * @param decal_X Decalage selon X en pixel (vers la droite)
 * @param decal_Y Decalage selon Y en pixel (vers le haut)
 * @param ecart Ecart vertical entre les éléments de la légende
 * @note Limité à 8 éléments pour le moment.
 */
void Make_legend(Figure *fig,int decal_X, int decal_Y, int ecart);

/**
 * @brief Crée les ticks et les labels associés pour un barplot
 * 
 * @param fig Pointeur vers un objet de type figure
 * @param angle_labels Inclinaison des labels sous l'axe des X
 */
void Make_xticks_barplot(Figure *fig, float angle_labels);

/**
 * @brief Fonction spécifique au projet Belib. Crée les ticks et labels pour la courbe "moyenne horaire"
 * 
 * @param fig Pointeur vers un objet de type figure
 * @param nb_ticks Taille de tableau_avg_hours
 * @param tableau_avg_hours Tableau des horaires sélectionnés
 */
void Make_xticks_xgrid_time_avgH(Figure *fig, int nb_ticks,int tableau_avg_hours[nb_ticks]);

/**
 * @brief Crée les ticks pour l'axe des X dans le cas d'un axe X représentant des Date
 * 
 * @param fig Pointeur vers un objet de type figure
 * @param date_init Objet de type Date correspondant à l'origine des temps
 */
void Make_xticks_xgrid_time(Figure *fig, Date date_init);

/**
 * @brief Crée les ticks pour l'axe Y dans le cas d'un tracé de fLineData (récup du max flottant)
 * 
 * @param fig Pointeur vers objet de type Figure
 * @param wTicks Caractère 'y' ou 'n'. Affiche ou non les ticks (seuls les labels affichés).
 */
void Make_fyticks_ygrid(Figure *fig, char wTicks);

/**
 * @brief Crée les ticks pour l'axe Y dans le cas d'un tracé de LineData (récup du max entier)
 * 
 * @param fig Pointeur vers objet de type Figure
 * @param wTicks Caractère 'y' ou 'n'. Affiche ou non les ticks (seuls les labels affichés).
 */
void Make_yticks_ygrid(Figure *fig, char wTicks);

/**
 * @brief Crée un vecteur de Datetick à partir du tableau des Date
 * 
 * @param nb_rows Nombre de date dans le tableau des Date
 * @param vect_dateticks Vecteur des DateTick rempli à partir de tableau_date_recolte_fav
 * @param tableau_date_recolte_fav Vecteur de Date 
 */
void Make_dateticks_vect(int nb_rows, Datetick vect_dateticks[nb_rows], Date tableau_date_recolte_fav[nb_rows]);

/**
 * @brief Fonction de debug affichant des informations sur un objet de type Figure
 * 
 * @param fig Pointeur vers objet de type Figure
 */
void Print_debug_fig(Figure *fig);

/**
 * @brief Fonction de debug affichant des informations sur un objet de type LineStyle
 * 
 * @param linestyle Pointeur vers objet de type LineStyle
 */
void Print_debug_ls(LineStyle *linestyle);

/**
 * @brief Fonction de debug affichant des informations sur un objet de type LineData
 * 
 * @param linedata Pointeur vers objet de type LineData
 * @param w_xy Caractère 'y' ou 'n'. Affiche les données des vecteurs X et Y.
 */
void Print_debug_ld(LineData *linedata, char w_xy);

/**
 * @brief Fonction de debug affichant des informations sur un objet de type fLineData
 * 
 * @param flinedata Pointeur vers objet de type fLineData
 * @param w_xy Caractère 'y' ou 'n'. Affiche les données des vecteurs X et Y.
 */
void Print_debug_fld(fLineData *flinedata, char w_xy);

/**
 * @brief Fonction de debug affichant des informations sur un objet de type Bardata
 * 
 * @param bardata Pointeur vers objet de type BarData
 * @param w_cat Caractère 'y' ou 'n'. Affiche les données contenues dans le BarData.
 */
void Print_debug_bd(BarData *bardata, char w_cat);

/* --------------------------------------------------------------------------- */
// Définition des fonctions
/* --------------------------------------------------------------------------- */


void Make_dateticks_vect(int nb_rows, Datetick vect_dateticks[nb_rows], Date tableau_date_recolte_fav[nb_rows])
{
    for (int i = 0; i < nb_rows; i++)
    {
        Datetick tick_i;
        Init_Datetick(&tick_i,\
                &tableau_date_recolte_fav[i], &tableau_date_recolte_fav[0]);

        vect_dateticks[i] =tick_i;
    }      
}


/* --------------------------------------------------------------------------- */
int Maxval_array(const int x_array[], size_t n)
{
    int t, i;
    t = x_array[0];
    for(i = 1; i < n; i++)
    {
        t = (x_array[i] > t) ? x_array[i] : t;
    }
    return t;
}

/* --------------------------------------------------------------------------- */
int Minval_array(const int x_array[], size_t n)
{
    int t, i;
    t = x_array[0];
    for(i = 1; i < n; i++)
    {
        t = (x_array[i] > t) ? t : x_array[i];
    }
    return t;
}

/* --------------------------------------------------------------------------- */
float fMaxval_array(const float x_array[], size_t n)
{
    int i;
    float t = x_array[0];
    for(i = 1; i < n; i++)
    {
        t = (x_array[i] > t) ? x_array[i] : t;
    }
    return t;
}

/* --------------------------------------------------------------------------- */
float Deg2rad(float angle)
{
    return angle*PI/180;
}

/* --------------------------------------------------------------------------- */
void Save_to_png(Figure *fig, const char *dir_figures, const char *filename_fig)
{
    FILE *pngout_fig;

    char path_outputFig[400] = {""};
    strcat(path_outputFig, dir_figures);
    strcat(path_outputFig, filename_fig);
    pngout_fig = fopen(path_outputFig, "wb");

    /* Output the image to the disk file in PNG format. */
    gdImagePngEx(fig->img, pngout_fig,5);

    /* Close the files. */
    fclose(pngout_fig);

    // Free des tableau de data dans la figure
    free(fig->flinedata);
    free(fig->linedata);
    free(fig->bardata);

}


/* --------------------------------------------------------------------------- */
void Make_background(Figure *fig,\
                        const int color_bg[3], const int color_canvas_bg[3])
{
    /* Remplissage du fond */
    gdImageFilledRectangle(fig->img,\
                        0, 0,\
                        fig->img->sx-1, fig->img->sy-1,
                        GetCouleur(fig->img, color_bg));

    /* Remplissage du canvas */
    gdImageFilledRectangle(fig->img,\
                        fig->padX[0], fig->padY[0],\
                        (fig->img->sx-1)-fig->padX[1], (fig->img->sy-1) - fig->padY[1],\
                        GetCouleur(fig->img, color_canvas_bg));
}

/* --------------------------------------------------------------------------- */
void ImageLineEpaisseur(gdImagePtr im_fig,\
    const int x1, const int y1, const int x2, const int y2, LineStyle *linestyle)
{
                    
    gdImageSetThickness(im_fig, linestyle->w);
    // gdImageSetAntiAliased(im_fig, GetCouleur(im_fig, linestyle->color));
    if (linestyle->style == '-') {
        gdImageLine(im_fig, x1, y1,   x2,   y2,\
                   GetCouleur(im_fig, linestyle->color));
    } else if (linestyle->style == ':') {
        gdImageDashedLine(im_fig, x1, y1,   x2,   y2,\
                   GetCouleur(im_fig, linestyle->color));
    } else {

    }
    
    // gdImageSetThickness(im_fig, 1);
}


/* --------------------------------------------------------------------------- */
void Make_support_axes(Figure *fig, const int couleur[3])
{
    int w_axes = 3;
    LineStyle linestyle_axe;
    Init_linestyle(&linestyle_axe, '-', couleur, w_axes, ' ', 0);

    // axe vertical
    ImageLineEpaisseur(fig->img,\
                            fig->orig[0]-(w_axes-1), fig->orig[1]+(w_axes-1),\
                            fig->orig[0]-(w_axes-1), fig->padY[0],\
                            &linestyle_axe); 

    // axe horizontal
    ImageLineEpaisseur(fig->img,\
                            fig->orig[0]-(w_axes-1), fig->orig[1]+(w_axes-1),\
                            (fig->img->sx-1)-fig->padX[1],  fig->orig[1]+(w_axes-1),
                            &linestyle_axe); 
}

/* --------------------------------------------------------------------------- */
void PlotPoint(Figure *fig, const int x1, const int y1,\
                        LineStyle *linestyle)
{
    if (x1 == fig->orig[0] && y1 != fig->orig[1])
    {   
        // Arc lorsque pt sur axe horizontal
        gdImageFilledArc (fig->img, x1, y1, linestyle->ms, linestyle->ms,\
                          -90, 90,\
                          GetCouleur(fig->img, linestyle->color), gdArc);
    } else if (x1 != fig->orig[0] && y1 == fig->orig[1])
    {
        // Arc lorsque pt sur axe vertical
        gdImageFilledArc (fig->img, x1, y1, linestyle->ms, linestyle->ms,\
                          -180, 0,\
                          GetCouleur(fig->img, linestyle->color), gdArc);
    } else if (x1 == fig->orig[0] && y1 == fig->orig[1])
    {
        // Arc lorsque pt sur l'origine
        gdImageFilledArc (fig->img, x1, y1, linestyle->ms, linestyle->ms,\
                          -90, 0,\
                          GetCouleur(fig->img, linestyle->color), gdArc);
    } else {
        gdImageFilledEllipse(fig->img, x1, y1,\
            linestyle->ms, linestyle->ms, GetCouleur(fig->img, linestyle->color));
    }

}

/* --------------------------------------------------------------------------- */
void PlotLine(Figure *fig, LineData *linedata)
{
    int *x_plot = Transform_data_to_plot(fig, linedata->len_data, linedata->x, 'x');
    int *y_plot = Transform_data_to_plot(fig, linedata->len_data, linedata->y, 'y');   

    for (int i=0; i < linedata->len_data-1; i++) 
    {
        ImageLineEpaisseur(fig->img,\
                x_plot[i]   + fig->orig[0],   y_plot[i] + fig->orig[1],\
                x_plot[i+1] + fig->orig[0], y_plot[i+1] + fig->orig[1],\
                linedata->linestyle);

        
        /* PlotPoint(fig,\
            x_plot[i] + fig->orig[0], y_plot[i] + fig->orig[1], linedata->linestyle);

        if (i == linedata->len_data-2) {
            PlotPoint(fig,\
                            x_plot[i+1] + fig->orig[0],\
                            y_plot[i+1] + fig->orig[1],\
                            linedata->linestyle);
        } */

    }

    free(x_plot);
    free(y_plot);
}

/* --------------------------------------------------------------------------- */
void PlotFLine(Figure *fig, fLineData *flinedata)
{
    int *x_plot = Transform_fdataX_to_plot(fig, flinedata->len_data, flinedata->x);
    int *y_plot = Transform_fdataY_to_plot(fig, flinedata->len_data, flinedata->y);

    for (int i=0; i < flinedata->len_data-1; i++) 
    {
        
        ImageLineEpaisseur(fig->img,\
                x_plot[i]   + fig->orig[0],   y_plot[i] + fig->orig[1],\
                x_plot[i+1] + fig->orig[0], y_plot[i+1] + fig->orig[1],\
                flinedata->linestyle);

        
        PlotPoint(fig,\
            x_plot[i] + fig->orig[0], y_plot[i] + fig->orig[1], flinedata->linestyle);

        if (i == flinedata->len_data-2) {
            PlotPoint(fig,\
                            x_plot[i+1] + fig->orig[0],\
                            y_plot[i+1] + fig->orig[1],\
                            flinedata->linestyle);
        }

    }

    free(x_plot);
    free(y_plot);
}

/* --------------------------------------------------------------------------- */
int *Transform_fdataX_to_plot(Figure *fig, size_t len_pts,\
                                             const int pts[])
{
    // Taile de la zone de dessin
    // (Nx-1) - orig X (0) - margin X (0) - padX droite (1)
    const int w_dessin = (fig->img->sx-1) - fig->orig[0] - fig->margin[0] - fig->padX[1];

    int *pts_dessin = malloc(len_pts * sizeof(int));
    int itv_pixels = (w_dessin) / (fig->max_X - pts[0]);

    for (int i = 0; i < len_pts; i++) {
        // Decalage de l'origine au point initial (lorsque ne debute pas à 0)
        pts_dessin[i] = i * itv_pixels;
        // pts_dessin[i] = pts_dessin[i] - pts_dessin[0]; 
        // printf("Point : %d, %d \n", i, pts_dessin[i]);
    }

    return pts_dessin;

}

/* --------------------------------------------------------------------------- */
int *Transform_fdataY_to_plot(Figure *fig, size_t len_pts, \
                                    const float pts[])
{
    // Taile de la zone de dessin
    // Orig - padding haut (0) - margin Y (1)
    const int h_dessin = fig->orig[1] - fig->padY[0] - fig->margin[1];   

    int *pts_dessin = malloc(len_pts * sizeof(float));

    for (int i = 0; i < len_pts; i++) {
        pts_dessin[i] = -(pts[i] * h_dessin) / fig->fmax_Y;
        // printf("Point : %d, %d \n", i, pts_dessin[i]);
    }

    return pts_dessin;
}


/* --------------------------------------------------------------------------- */
int *Transform_data_to_plot_bar(Figure *fig, size_t nb_ctg, \
                                    const int pts[])
{
    const int h_dessin = fig->orig[1] - fig->padY[0] - fig->margin[1];   
    
    // MALLOC
    int *y_dessin_ctg = malloc(nb_ctg * sizeof(int));

    for (int i = 0; i < nb_ctg; i++) {
        if (i == 0)
            y_dessin_ctg[i] = -(pts[i] * h_dessin) / fig->max_Y;
        else
            y_dessin_ctg[i] = y_dessin_ctg[i-1] - (pts[i] * h_dessin) / fig->max_Y;
    }

    return y_dessin_ctg;
}      

/* --------------------------------------------------------------------------- */
void Change_font(Figure *fig, int textType, char* path_f)
{
    fig->fonts[textType].path = path_f;
}

/* --------------------------------------------------------------------------- */
void Change_fontsize(Figure *fig, int textType, int size)
{
    fig->fonts[textType].size = size;
}

/* --------------------------------------------------------------------------- */
void Change_fontcolor(Figure *fig, int textType, int color[3])
{
    for (int i = 0; i < 3; i++)
        fig->fonts[textType].color[i] = color[i];
}


/* --------------------------------------------------------------------------- */
void PlotBarplot(Figure *fig, BarData *bardata, char wlabels)
{
    int *y_bars = \
        Transform_data_to_plot_bar(fig, bardata->nb_ctg, bardata->nb_in_ctg);   

    // largeur zone de dessin
    int w_dessin = (fig->img->sx-1) - fig->orig[0] - fig->margin[0] - fig->padX[1];

    // intervalle en px entre 2 centres de barplot selon X
    int itv_posX = w_dessin/(fig->nb_bardata+1); 
    
    // position des ticks/centre des barplot selon X
    int posX_center = fig->orig[0] + bardata->idx * itv_posX;

    // Plot les rectangles
    int y1_rect = 0;
    int y2_rect = fig->orig[1];

    for (int ctg = 0; ctg < bardata->nb_ctg; ctg++)
    {
        y1_rect = fig->orig[1] + y_bars[ctg];

        // Dessins rect 
        if (y1_rect != y2_rect) {
            // Ajout des labels
            if (wlabels == 'y') {
                int posX_label = posX_center + itv_posX/3 - 4;
                int posY_label = y2_rect - (y2_rect-y1_rect)/2 \
                                + fig->fonts[label_f].size / 2;

                char nb_in_ctg[fig->max_Y]; // pas de surprises
                sprintf(nb_in_ctg, "%d", bardata->nb_in_ctg[ctg]);

                gdImageStringFT(fig->img, NULL,\
                            GetCouleur(fig->img,\
                            bardata->colors[ctg]),\
                            fig->fonts[title_f].path,\
                            fig->fonts[label_f].size,\
                            0.,\
                            posX_label,\
                            posY_label,\
                            nb_in_ctg);  
            }

            // Plot rect
            gdImageFilledRectangle(fig->img, \
                posX_center - itv_posX/4, y1_rect,\
                posX_center + itv_posX/4, y2_rect,\
                GetCouleur(fig->img, bardata->colors[ctg]));
        }

        // Update y2=y1
        y2_rect = y1_rect;

    }

    free(y_bars);
}



/* --------------------------------------------------------------------------- */
void Transform_dataX_to_plot(Figure *fig, size_t len_pts,\
                                    const int pts[], int* pts_dessin)
{
    // Taile de la zone de dessin
    // (Nx-1) - orig X (0) - margin X (0) - padX droite (1)
    const int w_dessin = (fig->img->sx-1) - fig->orig[0] - fig->margin[0] - fig->padX[1];

    for (int i = 0; i < len_pts; i++)
    {
        pts_dessin[i] = ((pts[i]-pts[0]) * w_dessin) / (fig->max_X-pts[0]);
        // printf("Point : %d, %d \n", i, pts_dessin[i]);
    }

}
                           

/* --------------------------------------------------------------------------- */
void Transform_dataY_to_plot(Figure *fig, size_t len_pts, \
                                    const int pts[], int* pts_dessin)
{
    // Taile de la zone de dessin
    // Orig - padding haut (0) - margin Y (1)
    const int h_dessin = fig->orig[1] - fig->padY[0] - fig->margin[1];   
    for (int i = 0; i < len_pts; i++){
        pts_dessin[i] = -(pts[i] * h_dessin) / fig->max_Y;
        // printf("Point : %d, %d \n", i, pts_dessin[i]);
    }

}

/* --------------------------------------------------------------------------- */
int *Transform_data_to_plot(Figure *fig, size_t len_pts,\
                                  const int pts[], char direction)
{
    int *pts_dessin = malloc(len_pts * sizeof(int));

    if (pts_dessin == NULL) {
        printf("Erreur : Pas assez de memoire.\n");
        free(pts_dessin);
        exit(EXIT_FAILURE);
    }    

    // Taile de la zone de dessin
    if (direction == 'x') {
        Transform_dataX_to_plot(fig, len_pts, pts, pts_dessin);
    } else if (direction == 'y')
    {
        Transform_dataY_to_plot(fig, len_pts, pts, pts_dessin);
    }
    else 
    {
        printf("Erreur : Direction de tracé inconnue.\n");
        free(pts_dessin);
        exit(EXIT_FAILURE);
    }
    
    return pts_dessin;
}

/* --------------------------------------------------------------------------- */
int GetCouleur(gdImagePtr im_fig, const int couleur[3])
{
    return gdImageColorAllocate(im_fig, couleur[0], couleur[1], couleur[2]);
}

/* --------------------------------------------------------------------------- */
void Init_figure(Figure *fig, int figsize[2],\
                int padX[2], int padY[2], int margin[2], char wAxes)
{

    /* Initialisation de tous les parametres*/

    // ATTENTION ! Important d'utiliser True Color au lieu de gdImageColor ... 
    // Ecrasement des plots un peu bizarre sinon ...
    fig->img = gdImageCreateTrueColor(figsize[0],figsize[1]);

    fig->padX[0] = padX[0]; /**< left */
    fig->padX[1] = padX[1]; /**< right */
    fig->padY[0] = padY[0]; /**< haut */
    fig->padY[1] = padY[1]; /**< bas */   
    fig->orig[0] = fig->padX[0];
    fig->orig[1] = (fig->img->sy-1) - fig->padY[1];
    fig->margin[0] = margin[0];
    fig->margin[1] = margin[1];

    int col_fond[3] = {37, 44, 56};     // bleu-gris
    int col_cvs[3] = {0, 0, 0}; 
    int col_axes[3] = {255, 255, 255}; 

    for (int i = 0; i < 3; i++) {
        fig->color_bg[i] = col_fond[i];
        fig->color_cvs_bg[i] = col_cvs[i];        
        fig->color_axes[i] = col_axes[i];
    }

    // Fonts
    fig->fonts[label_f].path = fonts_fig[0];
    fig->fonts[title_f].path = fonts_fig[1];
    fig->fonts[annotation_f].path = fonts_fig[2];
    fig->fonts[ticklabel_f].path = fonts_fig[0];
    fig->fonts[subtitle_f].path = fonts_fig[1];
    fig->fonts[leg_f].path = fonts_fig[0];

    fig->fonts[label_f].size = 15;
    fig->fonts[title_f].size = 18;
    fig->fonts[annotation_f].size = 10;
    fig->fonts[ticklabel_f].size = 13;
    fig->fonts[subtitle_f].size = 14;
    fig->fonts[leg_f].size = 11;

    for (int i = 0; i < 3; i++) {
        fig->fonts[label_f].color[i] = col_axes[i];
        fig->fonts[title_f].color[i] = col_axes[i];
        fig->fonts[annotation_f].color[i] = col_axes[i];
        fig->fonts[ticklabel_f].color[i] = col_axes[i];
        fig->fonts[subtitle_f].color[i] = col_axes[i];
        fig->fonts[leg_f].color[i] = col_axes[i];
    }
    
    // Lenovo PC
    // fig.fonts[0] = "/usr/share/fonts/lato/Lato-Light.ttf";
    // fig.fonts[1] = "/usr/share/fonts/lato/Lato-Medium.ttf";
    // fig.fonts[2] = "/usr/share/fonts/lato/Lato-LightItalic.ttf";

    Make_background(fig, fig->color_bg, fig->color_cvs_bg);
    if ( wAxes == 'y')
        Make_support_axes(fig, fig->color_axes);


    fig->nb_flinedata = 0;
    fig->nb_linedata = 0;
    fig->nb_bardata  = 0;
    fig->flinedata = NULL;
    fig->linedata = NULL;
    fig->bardata = NULL;

    fig->max_X = 0;
    fig->max_Y = 0;
    fig->fmax_Y = 0.;
}

/* --------------------------------------------------------------------------- */
void Change_fig_bg(Figure *fig, int color[3]) 
{
    for (int i = 0; i < 3; i++)
    {
        fig->color_bg[i] = color[i];
    }
    Make_background(fig, fig->color_bg, fig->color_cvs_bg);                                           
}

/* --------------------------------------------------------------------------- */
void Change_fig_cvs_bg(Figure *fig, int color[3]) 
{
    for (int i = 0; i < 3; i++)
    {
        fig->color_cvs_bg[i] = color[i];        
    }
    Make_background(fig, fig->color_bg, fig->color_cvs_bg);
}

/* --------------------------------------------------------------------------- */
void Change_fig_axes_color(Figure *fig, int color[3]) 
{
    for (int i = 0; i < 3; i++)
    {
        fig->color_axes[i] = color[i];
    }
    Make_support_axes(fig, fig->color_axes);
}

/* --------------------------------------------------------------------------- */
void Init_bardata(BarData *bardata, int nb_ctg, char *labels_ctg[nb_ctg],\
                    int nb_tot, int nb_in_ctg[nb_ctg], \
                        int colors[nb_ctg][3], char* label)
{
    bardata->nb_ctg = nb_ctg;
    bardata->ctg_names = labels_ctg;
    bardata->nb_tot = nb_tot;
    bardata->nb_in_ctg = nb_in_ctg;
    bardata->colors = colors;
    bardata->label = label;

    bardata->idx = 0;
}

/* --------------------------------------------------------------------------- */
void Init_linedata(LineData *linedata, int len_data, int ptx[], int pty[],
                    char* label, LineStyle *linestyle)
{
    // int *ptx_plot = Transform_data_to_plot(fig, len_data, ptx, 'x');
    // int *pty_plot = Transform_data_to_plot(fig, len_data, pty, 'y');
    
    linedata->len_data = len_data;
    linedata->x = ptx; 
    linedata->y = pty;
    linedata->max_X = Maxval_array(linedata->x, linedata->len_data);
    linedata->max_Y = Maxval_array(linedata->y, linedata->len_data);
    linedata->label = label;
    linedata->linestyle = linestyle;
    // print_arr1D(len_data, linedata->y, 'n');
}


/* --------------------------------------------------------------------------- */
void Init_flinedata(fLineData *flinedata, int len_data, int ptx[], float pty[],
                    char* label, LineStyle *linestyle)
{
    
    flinedata->len_data = len_data;
    flinedata->x = ptx; 
    flinedata->y = pty;
    flinedata->max_X = Maxval_array(flinedata->x, flinedata->len_data);
    flinedata->fmax_Y = fMaxval_array(flinedata->y, flinedata->len_data);
    flinedata->label = label;
    flinedata->linestyle = linestyle;

    // print_arr1D(len_data, linedata->y, 'n');
}

/* --------------------------------------------------------------------------- */
void Init_linestyle(LineStyle *linestyle, char style,\
        const int color[3], int width, char marker, int ms)
{
    linestyle->style = style;
    for (int i = 0; i < 3; i++)
    {
        linestyle->color[i] = color[i];
    }    
    linestyle->w = width;
    linestyle->marker = marker;
    linestyle->ms = ms;
}

/* --------------------------------------------------------------------------- */
void Print_debug_fig(Figure *fig)
{
    printf("*** Debug Figure\n");
    printf("* Nb bardata   = %ld \n",  fig->nb_bardata);
    printf("* Nb linedata  = %ld \n",  fig->nb_linedata);
    printf("* Nb flinedata = %ld \n",  fig->nb_flinedata);
    printf("* Max x        = %d \n",   fig->max_X);
    printf("* Max y (int)  = %d \n",   fig->max_Y);
    printf("* Max y (float)= %.2f \n", fig->fmax_Y);
    printf("*** End Figure\n");
}


/* --------------------------------------------------------------------------- */
void Print_debug_ls(LineStyle *linestyle)
{
    printf("*** Debug linestyle\n");
    printf("* Style        = %c \n", linestyle->style);
    printf("* Width        = %d \n", linestyle->w);
    printf("* Marker       = %c \n", linestyle->marker);
    printf("* MarkerSize   = %d \n", linestyle->ms);
    printf("* Color        = %d,%d,%d \n", linestyle->color[0],\
                                           linestyle->color[1],\
                                           linestyle->color[2]);
    printf("*** End linestyle\n");
}

/* --------------------------------------------------------------------------- */
void Print_debug_ld(LineData *linedata, char w_xy)
{
    printf("*** Debug linedata --------------------------\n");
    printf("* Label    = %s \n", linedata->label);
    printf("* Len data = %ld \n", linedata->len_data);
    printf("* Max X    = %d \n", linedata->max_X);
    printf("* Max Y    = %d \n", linedata->max_Y);
    if (w_xy == 'y') {
        printf("* Vecteur X : \n");
        print_arr1D(linedata->len_data, linedata->x, 'n');
        printf("* Vecteur Y : \n");
        print_arr1D(linedata->len_data, linedata->y, 'n');
    }    
    Print_debug_ls(linedata->linestyle);
    printf("*** End linedata ----------------------------\n\n");
}

/* --------------------------------------------------------------------------- */
void Print_debug_fld(fLineData *flinedata, char w_xy)
{
    printf("*** Debug flinedata --------------------------\n");
    printf("* Label    = %s \n",   flinedata->label);
    printf("* Len data = %ld \n",  flinedata->len_data);
    printf("* Max X    = %d \n",   flinedata->max_X);
    printf("* Max Y    = %.1f \n", flinedata->fmax_Y);
    if (w_xy == 'y') {
        printf("* Vecteur X : \n");
        print_arr1D(flinedata->len_data, flinedata->x, 'n');
        printf("* Vecteur Y : \n");
        print_farr1D(flinedata->len_data, flinedata->y, 'n');
    }    
    Print_debug_ls(flinedata->linestyle);
    printf("*** End flinedata ----------------------------\n\n");
}


/* --------------------------------------------------------------------------- */
void Print_debug_bd(BarData *bardata, char w_cat)
{
    printf("*** Debug bardata --------------------------\n");
    printf("* Label    = %s \n",  bardata->label);
    printf("* Len data = %ld \n", bardata->nb_tot);
    printf("* Nb cat   = %ld \n", bardata->nb_ctg);
    if (w_cat == 'y') {
        printf("* Vecteur nb_in_ctg : \n");
        for (int i = 0; i < bardata->nb_ctg; i++){
            printf("  * Cat %d : %d ; Couleur : %d, %d, %d\n",\
                    i, bardata->nb_in_ctg[i],
                    bardata->colors[i][0],\
                    bardata->colors[i][1],\
                    bardata->colors[i][2]);
        }
    }    
    printf("*** End bardata ----------------------------\n\n");
}

/* --------------------------------------------------------------------------- */
int Max_int(int x, int y) 
{
    return (x < y) ? y : x ;
}

/* --------------------------------------------------------------------------- */
float Max_float(float x, float y) 
{
    return (x < y) ? y : x ;
}

/* --------------------------------------------------------------------------- */
int Min_int(int x, int y) 
{
    return (x > y) ? y : x ;
}

/* --------------------------------------------------------------------------- */
void Add_line_to_fig(Figure *fig, LineData *linedata)
{
    fig->nb_linedata++;
    fig->linedata = (LineData **)realloc(fig->linedata, fig->nb_linedata*sizeof(LineData));

    if (fig->linedata == NULL) {
        printf("Erreur : Pas assez de memoire.\n");
        free(fig->linedata);
        exit(EXIT_FAILURE);
    }

    // Index = nb line - 1 : on remplit les linedata de la figure
    fig->linedata[fig->nb_linedata-1] = linedata;

    // Print_debug_ld(fig->linedata[fig->nb_linedata]);

    fig->max_X = Max_int(fig->max_X, linedata->max_X);
    fig->max_Y = Max_int(fig->max_Y, linedata->max_Y);

}


/* --------------------------------------------------------------------------- */
void Add_fline_to_fig(Figure *fig, fLineData *flinedata)
{
    fig->nb_flinedata++;
    fig->flinedata = (fLineData **)realloc(fig->flinedata, fig->nb_flinedata*sizeof(fLineData));

    if (fig->flinedata == NULL) {
        printf("Erreur : Pas assez de memoire.\n");
        free(fig->flinedata);
        exit(EXIT_FAILURE);
    }

    // Index = nb line - 1 : on remplit les linedata de la figure
    fig->flinedata[fig->nb_flinedata-1] = flinedata;

    // Print_debug_ld(fig->linedata[fig->nb_linedata]);

    fig->max_X = Max_int(fig->max_X, flinedata->max_X);
    fig->fmax_Y = Max_float(fig->fmax_Y, flinedata->fmax_Y);
}   

/* --------------------------------------------------------------------------- */
void Add_barplot_to_fig(Figure *fig, BarData *bardata)
{
    fig->nb_bardata++;
    fig->bardata = (BarData **)realloc(fig->bardata, fig->nb_bardata*sizeof(BarData));

    if (fig->bardata== NULL) {
        printf("Erreur : Pas assez de memoire.\n");
        free(fig->bardata);
        exit(EXIT_FAILURE);
    }

    // Index = nb line - 1 : on remplit les linedata de la figure
    fig->bardata[fig->nb_bardata-1] = bardata;

    // Print_debug_ld(fig->linedata[fig->nb_linedata]);
    fig->max_X = 0;
    fig->max_Y = Max_int(fig->max_Y, bardata->nb_tot);

    bardata->idx = fig->nb_bardata; // On update l'index du bardata, pour posX
}

/* --------------------------------------------------------------------------- */
void Make_annotation(Figure *fig, char* text,\
                    int decalage_X, int decalage_Y)
{
    // Pos avec decalage : Y decal autre sens
    // Prise en compte de la longueur du label
    int posX_xlabel = 5 + decalage_X;
    int posY_xlabel = (fig->img->sy-1) - 5 - decalage_Y; 

    // int brect[8] = {0};
    gdImageStringFT(fig->img, NULL,\
                    GetCouleur(fig->img,
                    fig->fonts[annotation_f].color),\
                    fig->fonts[annotation_f].path,\
                    fig->fonts[annotation_f].size,\
                    0., posX_xlabel, posY_xlabel, text);

    //Avoid memory leaks
    gdFontCacheShutdown();
    // printf("%s \n", errStringFT); 
}

/* --------------------------------------------------------------------------- */
void Make_subtitle(Figure *fig, char* subtitle,\
                int bbox_title[8], int decalage_X, int decalage_Y)
{

    int ecartY_title = 6;
    int posX_subtitle = bbox_title[0] + (bbox_title[2]-bbox_title[0])/2 \
                    - strlen(subtitle) * fig->fonts[subtitle_f].size / 3;
    int posY_subtitle = bbox_title[1] + fig->fonts[subtitle_f].size \
                    + ecartY_title - decalage_Y; 

    gdImageStringFT(fig->img, NULL,\
                            GetCouleur(fig->img,\
                                fig->fonts[subtitle_f].color),
                            fig->fonts[subtitle_f].path,\
                            fig->fonts[subtitle_f].size,\
                            0., posX_subtitle, posY_subtitle, subtitle);    
    //Avoid memory leaks
    gdFontCacheShutdown();
}

/* --------------------------------------------------------------------------- */
void Const_str_dudate1_audate2(Date *date1, Date *date2, char* str_out)
{
    Datetick datetick_debut;
    Datetick datetick_fin;
    Init_Datetick(&datetick_debut, date1,date1);
    Init_Datetick(&datetick_fin, date2, date2);

    strcat(str_out, "du ");
    strcat(str_out, datetick_debut.labeldate);
    strcat(str_out, " au ");
    strcat(str_out, datetick_fin.labeldate);
}

/* --------------------------------------------------------------------------- */
int *Make_title(Figure *fig, char* title,\
                    int decalage_X, int decalage_Y)
{
    // Pos avec decalage : Y decal autre sens
    // Prise en compte de la longueur du label
    int largeur_cvs = (fig->img->sx-1 - fig->padX[0] - fig->padX[1]);
    int centre_cvs =  fig->orig[0] + largeur_cvs/2;
    int posX_xlabel = centre_cvs - (strlen(title) \
                        * fig->fonts[title_f].size/3.7) + decalage_X;
    int posY_xlabel = fig->img->sy - (fig->padY[1] - 3*fig->padY[1]/4) - decalage_Y; 

    static int brect_title[8] = {0};
    gdImageStringFT(fig->img, brect_title,\
                            GetCouleur(fig->img,\
                                fig->fonts[title_f].color),\
                            fig->fonts[title_f].path,\
                            fig->fonts[title_f].size,\
                            0., posX_xlabel, posY_xlabel, title);    
    //Avoid memory leaks
    gdFontCacheShutdown();
    // printf("%s \n", errStringFT); 

    return brect_title;
}


/* --------------------------------------------------------------------------- */
void Make_xlabel(Figure *fig, char* xlabel,\
                    int decalage_X, int decalage_Y)
{
    // Pos avec decalage : Y decal autre sens
    // Prise en compte de la longueur du label
    int posX_xlabel = fig->orig[0] +\
                     (fig->img->sx - fig->padX[0] - fig->padX[1])/2 \
                  - (strlen(xlabel) * fig->fonts[label_f].size / 3) + decalage_X;
                  
    int posY_xlabel = fig->img->sy - (fig->padY[1] - fig->padY[1]/3) + decalage_Y; 

    // int brect[8] = {0};
    gdImageStringFT(fig->img, NULL,\
                            GetCouleur(fig->img,\
                                fig->fonts[label_f].color),\
                            fig->fonts[label_f].path,\
                            fig->fonts[label_f].size,\
                            0., posX_xlabel, posY_xlabel, xlabel);    
    //Avoid memory leaks
    gdFontCacheShutdown();
    // printf("%s \n", errStringFT); 
}


/* --------------------------------------------------------------------------- */
void Make_ylabel(Figure *fig, char* ylabel,\
                     int decalage_X, int decalage_Y)
{
    // Pos avec decalage : Y decal autre sens
    int posX_ylabel = fig->padX[0]/2 - fig->padX[0]/4      + decalage_X;

    // Prise en compte de la longueur du label
    int posY_ylabel = (fig->orig[1] + (fig->padY[0]))/2 + \
                            (strlen(ylabel) * fig->fonts[label_f].size/3)\
                             + decalage_Y;   

    int brect[8] = {0};
    char *errStringFT = gdImageStringFT(fig->img, brect,\
                            GetCouleur(fig->img,\
                            fig->fonts[label_f].color),
                            fig->fonts[label_f].path,\
                            fig->fonts[label_f].size,\
                            Deg2rad(90.), posX_ylabel, posY_ylabel, ylabel);    

    if (errStringFT != NULL)
        printf("> Warning: police introuvable ou autre probleme police.\n");

    //Avoid memory leaks
    gdFontCacheShutdown();
}

/* --------------------------------------------------------------------------- */
void Make_legend_barplot(Figure *fig,\
                    int decal_X, int decal_Y, int ecart)
{
    // Position des 4 legendes possibles
    int w_dessin = (fig->img->sx-1) - fig->orig[0] - fig->margin[0] - fig->padX[1];
    int pos_midX = fig->orig[0] + w_dessin/2 + 5;
    
    int pos_X[4] = {\
                fig->orig[0] + 5 + decal_X, fig->orig[0] + 5 + decal_X,\
                pos_midX + decal_X, pos_midX + decal_X};

    int ecart_base = 20;
    int ecart_Y = ecart_base + ecart;

    int pos_Y[4] = {  (fig->fonts[leg_f].size + ecart_Y) - decal_Y,\
                    2*(fig->fonts[leg_f].size + ecart_Y) - decal_Y,\
                      (fig->fonts[leg_f].size + ecart_Y) - decal_Y,\
                    2*(fig->fonts[leg_f].size + ecart_Y) - decal_Y};

    LineStyle linestyle1;
    Init_linestyle(&linestyle1, ' ', color_lines[0], 0, 'o', 4);

    int h_rect = 15;
    int l_rect = 30;
    int posX_label = 0;
    int posY_label = 0;

    for (int i = 0; i < fig->bardata[0]->nb_ctg ; i++) {
        // Plot petits rectangles legendes
        gdImageFilledRectangle(fig->img,\
                    pos_X[i], pos_Y[i]-h_rect,\
                    pos_X[i]+l_rect, pos_Y[i],\
                    GetCouleur(fig->img, fig->bardata[0]->colors[i]));

        // Labels
        posX_label = pos_X[i]+l_rect+10;
        posY_label = pos_Y[i] ;
        gdImageStringFT(fig->img, NULL,\
                            GetCouleur(fig->img,\
                            fig->fonts[leg_f].color),
                            fig->fonts[leg_f].path,\
                            fig->fonts[leg_f].size,\
                            0, posX_label, posY_label,\
                                fig->bardata[0]->ctg_names[i]);  
        
    }                 

}

/* --------------------------------------------------------------------------- */
void Make_legend(Figure *fig,\
                    int decal_X, int decal_Y, int ecart)
{
    // Position des 8 legendes possibles
    int w_img = (fig->img->sx-1) - fig->orig[0];
    int pos_midX = fig->orig[0] + w_img/2 + 5;
    // printf("Done here.\n");

    int pos_X[8] = {fig->orig[0] + 5 + decal_X, fig->orig[0] + 5  + decal_X,\
                    fig->orig[0] + 5  + decal_X, fig->orig[0] + 5  + decal_X,\
                    pos_midX + decal_X, pos_midX + decal_X,\
                    pos_midX + decal_X, pos_midX + decal_X};

    int ecart_base = 8;
    int ecart_Y = ecart_base + ecart;
    int pos_Y[8] = {  (fig->fonts[leg_f].size + ecart_Y) - decal_Y,\
                    2*(fig->fonts[leg_f].size + ecart_Y) - decal_Y,\
                    3*(fig->fonts[leg_f].size + ecart_Y) - decal_Y,\
                    4*(fig->fonts[leg_f].size + ecart_Y) - decal_Y,\
                      (fig->fonts[leg_f].size + ecart_Y) - decal_Y,\
                    2*(fig->fonts[leg_f].size + ecart_Y) - decal_Y,\
                    3*(fig->fonts[leg_f].size + ecart_Y) - decal_Y,\
                    4*(fig->fonts[leg_f].size + ecart_Y) - decal_Y};

    // Longueur petit trait
    int long_trait_leg = 30;
    // Decalage apres trait
    int decalage_trait_leg = long_trait_leg + 10;

    char *errStringFT;
    int brect[8] = {0};

    if ( (fig->nb_linedata != 0) && (fig->nb_flinedata == 0) ) { 
        /* Pour les LineData*/
        // Print des petits traits de legende pour chaque plot
        for (int i = 0; i < fig->nb_linedata; i++) { 
            ImageLineEpaisseur(fig->img,\
                        pos_X[i],\
                        pos_Y[i] - fig->fonts[leg_f].size/2,\
                        pos_X[i] + long_trait_leg,\
                        pos_Y[i] - fig->fonts[leg_f].size/2,\
                        fig->linedata[i]->linestyle);
        }

        for (int i = 0; i < fig->nb_linedata; i++)
        {
            errStringFT=gdImageStringFT(fig->img, brect,\
                                GetCouleur(fig->img,\
                                    fig->fonts[leg_f].color),\
                                fig->fonts[leg_f].path,\
                                fig->fonts[leg_f].size,\
                                0.,\
                                decalage_trait_leg + pos_X[i],\
                                pos_Y[i],\
                                fig->linedata[i]->label);        
        }

        if (errStringFT != NULL) {
            printf("%s\n", errStringFT);
        }

    } else if ( (fig->nb_linedata == 0) && (fig->nb_flinedata != 0) ) {

        /* Pour les FLineData*/
        // Print des petits traits de legende pour chaque plot
        for (int i = 0; i < fig->nb_flinedata; i++) { 
            ImageLineEpaisseur(fig->img,\
                        pos_X[i],\
                        pos_Y[i] - fig->fonts[leg_f].size/2,\
                        pos_X[i] + long_trait_leg,\
                        pos_Y[i] - fig->fonts[leg_f].size/2,\
                        fig->flinedata[i]->linestyle);
        }


        for (int i = 0; i < fig->nb_flinedata; i++)
        {
            errStringFT= gdImageStringFT(fig->img, brect,\
                                GetCouleur(fig->img,\
                                    fig->fonts[leg_f].color),\
                                fig->fonts[leg_f].path,\
                                fig->fonts[leg_f].size,\
                                0.,\
                                decalage_trait_leg + pos_X[i],\
                                pos_Y[i],\
                                fig->flinedata[i]->label);        
        }

        if (errStringFT != NULL) {
            printf("%s\n", errStringFT);
        }        

    }
    //Avoid memory leaks
    gdFontCacheShutdown();
}


/* --------------------------------------------------------------------------- */
void Make_xticks_barplot(Figure *fig, float angle_labels)
{
    // largeur zone de dessin
    int w_dessin = (fig->img->sx-1) - fig->orig[0] - fig->margin[0] - fig->padX[1];

    // intervalle en px entre 2 centres de barplot selon X
    int itv_posX = w_dessin/(fig->nb_bardata+1); 
    
    // position des ticks/centre des barplot selon X
    int posX_center = 0;

    // position des labels
    int posX_label = 0;
    int posY_label = 0;

    // Recuperation de la longueur du label en px
    int brect[8] = {0};
    int len_label_px = 0;

    // Style tick
    int w_tick = 2;
    // int w_linegrid = 0.5;
    int long_tick = 9;
   
    LineStyle style_tick;
    Init_linestyle(&style_tick, '-', fig->color_axes, w_tick, 'o', 8);

    for (int bp = 0; bp < fig->nb_bardata ; bp ++)
    {
        // pos centre de la barre
        posX_center = fig->orig[0] + fig->bardata[bp]->idx * itv_posX;

        // plot tick
        ImageLineEpaisseur(fig->img,\
                        posX_center,\
                        fig->orig[1]+2,\
                        posX_center,\
                        fig->orig[1] + long_tick +2,\
                        &style_tick);
        

        // tick label
        // Fake call : recuperation du brect sur le label horizontal
        // On ecrit en 0,0
        gdImageStringFT(fig->img, brect,\
                            GetCouleur(fig->img, fig->color_bg),\
                            fig->fonts[ticklabel_f].path,\
                            fig->fonts[ticklabel_f].size, 0.,\
                            0, 0,\
                            fig->bardata[bp]->label);   

        // Longueur de l'adresse en px                                
        len_label_px = brect[2] - brect[0];

        posX_label = (int)(len_label_px * cos(Deg2rad(angle_labels)));
        posY_label = (int)(len_label_px * sin(Deg2rad(angle_labels)));

        gdImageStringFT(fig->img, NULL,\
                            GetCouleur(fig->img,\
                                    color_lines[bp]),\
                            fig->fonts[ticklabel_f].path,\
                            fig->fonts[ticklabel_f].size,\
                            Deg2rad(angle_labels),\
                            posX_center - posX_label,\
                            fig->orig[1] + (long_tick*3) + posY_label,\
                            fig->bardata[bp]->label);

    }

    //Avoid memory leaks
    gdFontCacheShutdown();

}

/* --------------------------------------------------------------------------- */
void Make_xticks_xgrid_time_avgH(Figure *fig, int nb_ticks,\
            int tableau_avg_hours[nb_ticks])
{
    if (fig->max_X == 0)
    {
        printf("Erreur : pas de ticks pour un max a 0.");
        exit(EXIT_FAILURE);
    }
    
    // Intervalles entre 2 ticks
    int itv_tickh = 1;

    // Largeur canvas
    int l_canvas = (fig->img->sx-1) - fig->orig[0] - fig->padX[1];
    // On pense a enlever la marge Y pour calculer l'ecart entre tick maj
    int l_max = l_canvas - fig->margin[0];

    // Intervalle entre 2 ticks en pix
    int itv_pixels = (itv_tickh*l_max) / (fig->max_X - fig->flinedata[0]->x[0]);
    // printf("Itv X en px = %d \n", itv_pixels);

    // Style tick
    int w_tick = 2;
    int w_linegrid = 0.5;
    int long_tick = 4;

    LineStyle style_tick;
    Init_linestyle(&style_tick, '-', fig->color_axes, w_tick, ' ', ' ');

    LineStyle style_linegrid;
    Init_linestyle(&style_linegrid, '-', gris_grid, w_linegrid, ' ', ' ');

    // Plot ticks
    // printf("Nb ticks : %d \n", nb_ticks);
    for (int i = 0; i < nb_ticks; i++)
    {
        // printf("Tick : %d \n", i);

        // tick
        ImageLineEpaisseur(fig->img,\
                        fig->orig[0] + i*itv_pixels,\
                        fig->orig[1]+2,\
                        fig->orig[0] + i*itv_pixels,\
                        fig->orig[1] + long_tick +2,\
                        &style_tick);

        //gridline
        ImageLineEpaisseur(fig->img,\
                        fig->orig[0] + i*itv_pixels,\
                        fig->orig[1],\
                        fig->orig[0] + i*itv_pixels,\
                        fig->padY[0],\
                        &style_linegrid);        

        char tickAvgH[6];
        sprintf(tickAvgH, "%2dh", i+fig->flinedata[0]->x[0]);

        int xlabel_date = fig->orig[0] - \
                strlen(tickAvgH)*fig->fonts[ticklabel_f].size/3.7 + i*itv_pixels;

        int ylabel_date = fig->orig[1] + \
                (long_tick+10) + fig->fonts[ticklabel_f].size;
        // tick label date
        if (fig->flinedata[0]->x[i]%2 != 0) {
        gdImageStringFT(fig->img, NULL,\
                            GetCouleur(fig->img,\
                                fig->fonts[ticklabel_f].color),\
                            fig->fonts[ticklabel_f].path,\
                            fig->fonts[ticklabel_f].size,\
                            0.,\
                            xlabel_date, ylabel_date, tickAvgH);
        }
        
    }

    //Avoid memory leaks
    gdFontCacheShutdown();

}

/* --------------------------------------------------------------------------- */
void Make_xticks_xgrid_time(Figure *fig, Date date_init)
{
    if (fig->max_X == 0)
    {
        printf("Erreur : pas de ticks pour un max a 0.");
        exit(EXIT_FAILURE);
    }
    
    int nb_ticks = 7;
    int nb_itv = nb_ticks-1;

    // Intervalles entre 2 ticks en sec
    int itv_sec = fig->max_X / nb_itv;

    // Largeur canvas
    int l_canvas = (fig->img->sx-1) - fig->orig[0] - fig->padX[1];
    // On pense a enlever la marge Y pour calculer l'ecart entre tick maj
    int l_max = l_canvas - fig->margin[0];

    // Intervalle entre 2 ticks en pix
    int itv_pixels = (itv_sec*l_max) / fig->max_X;
    // printf("Itv X en px = %d \n", itv_pixels);

    // Style tick
    int w_tick = 2;
    int w_linegrid = 0.5;
    int long_tick = 9;

    LineStyle style_tick;
    Init_linestyle(&style_tick, '-', fig->color_axes, w_tick, ' ', ' ');

    LineStyle style_linegrid;
    Init_linestyle(&style_linegrid, '-', gris_grid, w_linegrid, ' ', ' ');

    // Plot ticks
    // printf("Nb ticks : %d \n", nb_ticks);
    for (int i = 0; i < nb_ticks; i++)
    {
        // printf("Tick : %d \n", i);

        // tick
        ImageLineEpaisseur(fig->img,\
                        fig->orig[0] + i*itv_pixels,\
                        fig->orig[1]+2,\
                        fig->orig[0] + i*itv_pixels,\
                        fig->orig[1] + long_tick +2,\
                        &style_tick);

        //gridline
        ImageLineEpaisseur(fig->img,\
                        fig->orig[0] + i*itv_pixels,\
                        fig->orig[1],\
                        fig->orig[0] + i*itv_pixels,\
                        fig->padY[0],\
                        &style_linegrid);        

        #if defined(LENOVO) || defined(QEMU)
            time_t t_tick = date_init.ctime + i*itv_sec + 3600; // hack ajout 1h
        #else
            time_t t_tick = date_init.ctime + i*itv_sec;
        #endif

        struct tm *tm_tick;
        gmtime(&t_tick);
        tm_tick = gmtime(&t_tick);

        char tickdate[6];
        strftime(tickdate, 6, "%d/%m", tm_tick);
        // printf("%s \n", tickdate);

        char tickhour[6];
        strftime(tickhour, 6, "%H:%M", tm_tick);
        // printf("%s \n", tickhour);


        int xlabel_date = fig->padX[0]/2 -10 + i*itv_pixels;
        int ylabel_date = fig->orig[1] + (long_tick+2) + fig->fonts[ticklabel_f].size;
        // tick label date
        gdImageStringFT(fig->img, NULL,\
                            GetCouleur(fig->img,\
                                fig->fonts[ticklabel_f].color),\
                            fig->fonts[ticklabel_f].path,\
                            fig->fonts[ticklabel_f].size,\
                            0.,\
                            xlabel_date, ylabel_date, tickdate);

        int xlabel_hour = fig->padX[0]/2 -10 + i*itv_pixels + fig->fonts[ticklabel_f].size/2;
        int ylabel_hour = fig->orig[1] + 2*((long_tick) + fig->fonts[ticklabel_f].size);
        // tick label heure
        gdImageStringFT(fig->img, NULL,\
                            GetCouleur(fig->img,\
                                fig->fonts[ticklabel_f].color),\
                            fig->fonts[ticklabel_f].path,\
                            fig->fonts[ticklabel_f].size,\
                            0.,xlabel_hour, ylabel_hour, tickhour);
                                
        // printf("%d\n", i);
    }

    //Avoid memory leaks
    gdFontCacheShutdown();

}


/* --------------------------------------------------------------------------- */
void Make_fyticks_ygrid(Figure *fig, char wTicks)
{
    if (fig->fmax_Y == 0)
    {
        printf("Erreur : pas de ticks pour un max a 0.");
        exit(EXIT_FAILURE);
    }

    int itv=0;
    if (fig->fmax_Y <= 10) {
        itv = 1;
    } else if (fig->fmax_Y <= 20) {
        itv = 2;
    } else if (fig->fmax_Y <= 50) {
        itv = 5;
    } else if (fig->fmax_Y <= 100) {
        itv = 10;
    }
    
    // printf("Itv       = %d \n", itv);
    // printf("Itv minor = %d \n", itv_minor);
    int nb_ticks = fig->fmax_Y / itv;

    // printf("Nb ticks  = %d \n", nb_ticks);

    int h_canvas = fig->img->sy - fig->padY[0] - fig->padY[1];
    // On pense a enlever la marge Y pour calculer l'ecart entre tick maj
    // Pos de maxY repere figure = h_canvas - fig->margin[1]
    int h_max = h_canvas - fig->margin[1];
    // printf("h max     = %d \n", h_max);

    // int ecart_major = ( h_canvas - fig->margin[1] ) / (nb_interv)  ; 

    int itv_pixels = (itv*h_max) / fig->fmax_Y;
    // printf("Itv px     = %d \n", itv_pixels);

    // Style tick
    int w_tick = 2;
    int w_linegrid = 0.5;
    LineStyle style_tick;
    Init_linestyle(&style_tick, '-', fig->color_axes, w_tick, ' ', ' ');
    int long_tick = 0;
    LineStyle style_linegrid;
    Init_linestyle(&style_linegrid, '-', gris_grid, w_linegrid, ' ', ' ');

    // Plot ticks
    char tickVal[12];

    for (int i = 1; i <= nb_ticks; i++) {
        // gridline
        ImageLineEpaisseur(fig->img,\
                        fig->orig[0],\
                            fig->orig[1] - i*(itv_pixels+0.5),\
                        (fig->img->sx-1) - fig->padX[1],\
                            fig->orig[1] - i*(itv_pixels+0.5),\
                        &style_linegrid);        

        // ticks
        if (wTicks == 'y') {
            long_tick = 9;
            // tick : facteur 0.5 ajouté pour gerer les nombres pairs/impais de px
            ImageLineEpaisseur(fig->img,\
                            fig->orig[0]-long_tick-2,\
                                fig->orig[1] - i*(itv_pixels+0.5),\
                            fig->orig[0]-2,\
                                fig->orig[1] - i*(itv_pixels+0.5),\
                            &style_tick);
        }

        // Fabrication des tick labels
        sprintf(tickVal, "%d", i*itv);

        int posX_ticklab = fig->orig[0]-(long_tick+2) \
                            - strlen(tickVal) * fig->fonts[ticklabel_f].size;
        int posY_ticklab = fig->orig[1] - i*itv_pixels \
                            + fig->fonts[ticklabel_f].size / 2;

        // printf("Tick %d : %s\n", i, tickVal);
        // // printf("Tick posX : %d \n", posX_ticklab);
        // // printf("Tick posY : %d \n", posY_ticklab);
        // printf("%s \n",fig->fonts[ticklabel_f].path);
        // printf("%d \n",fig->fonts[ticklabel_f].size);

        gdImageStringFT(fig->img, NULL,\
                            GetCouleur(fig->img,\
                                    style_tick.color),\
                            fig->fonts[ticklabel_f].path,\
                            fig->fonts[ticklabel_f].size,\
                                0., posX_ticklab, posY_ticklab, tickVal);

    }

    //Avoid memory leaks
    gdFontCacheShutdown();
}

/* --------------------------------------------------------------------------- */
void Make_yticks_ygrid(Figure *fig, char wTicks)
{
    if (fig->max_Y == 0)
    {
        printf("Erreur : pas de ticks pour un max a 0.");
        exit(EXIT_FAILURE);
    }

    // int nb_ticks = Min_int(fig->max_Y, 10);
    // printf("Nb ticks  = %d \n", nb_ticks);

    // int itv = fig->max_Y / nb_ticks;
    // printf("Itv       = %d \n", itv);

    int itv=0, itv_minor=0;
    if (fig->max_Y <= 10) {
        itv = 1;
        itv_minor = 0;
    } else if (fig->max_Y <= 20)
    {
        itv = 2;
        itv_minor = 1;
    } else if (fig->max_Y <= 50)
    {
        itv = 5;
        itv_minor = 1;
    } else if (fig->max_Y <= 100)
    {
        itv = 10;
        itv_minor = 2;
    }
    
    // printf("Itv       = %d \n", itv);
    // printf("Itv minor = %d \n", itv_minor);
    int nb_ticks = fig->max_Y / itv;

    int nb_ticks_min = 0;    
    if (itv_minor != 0) {
        nb_ticks_min= (fig->max_Y / itv_minor) / nb_ticks;
        // printf("Nb ticks min = %d \n", nb_ticks_min);
    }    
    // printf("Nb ticks  = %d \n", nb_ticks);

    int h_canvas = fig->img->sy - fig->padY[0] - fig->padY[1];
    // On pense a enlever la marge Y pour calculer l'ecart entre tick maj
    // Pos de maxY repere figure = h_canvas - fig->margin[1]
    int h_max = h_canvas - fig->margin[1];
    // printf("h max     = %d \n", h_max);

    // int ecart_major = ( h_canvas - fig->margin[1] ) / (nb_interv)  ; 

    int itv_pixels = (itv*h_max) / fig->max_Y;
    // printf("Itv px     = %d \n", itv_pixels);

    // Style tick
    int w_tick = 2;
    int w_linegrid = 0.5;
    LineStyle style_tick;
    Init_linestyle(&style_tick, '-', fig->color_axes, w_tick, ' ', ' ');
    int long_tick = 0;
    int long_tick_min = 0;
    LineStyle style_linegrid;
    Init_linestyle(&style_linegrid, '-', gris_grid, w_linegrid, ' ', ' ');

    // Plot ticks
    char tickVal[12];

    for (int i = 1; i <= nb_ticks; i++)
    {
        sprintf(tickVal, "%d", i*itv);


        // gridline
        ImageLineEpaisseur(fig->img,\
                        fig->orig[0],\
                            fig->orig[1] - i*(itv_pixels+0.5),\
                        (fig->img->sx-1) - fig->padX[1],\
                            fig->orig[1] - i*(itv_pixels+0.5),\
                        &style_linegrid);        

        if (wTicks == 'y') {
            long_tick = 9;
            long_tick_min = 5;
            // tick : facteur 0.5 ajouté pour gerer les nombres pairs/impais de px
            ImageLineEpaisseur(fig->img,\
                            fig->orig[0]-long_tick-2,\
                                fig->orig[1] - i*(itv_pixels+0.5),\
                            fig->orig[0]-2,\
                                fig->orig[1] - i*(itv_pixels+0.5),\
                            &style_tick);

            if (itv_minor != 0) {

                for (int j = 1; j < nb_ticks_min; j++)
                {
                    int itv_ytickmin = fig->orig[1] - (i-1)*itv_pixels - j*(itv_pixels/(nb_ticks_min));
                    
                    ImageLineEpaisseur(fig->img,\
                                    fig->orig[0]-long_tick_min-2,\
                                        itv_ytickmin,\
                                    fig->orig[0]-2,\
                                        itv_ytickmin,\
                                    &style_tick);            
                }
            }
        }

        int posX_ticklab = fig->orig[0]-(long_tick+2) \
                            - strlen(tickVal) * fig->fonts[ticklabel_f].size;
        int posY_ticklab = fig->orig[1] - i*itv_pixels \
                            + fig->fonts[ticklabel_f].size / 2;

        gdImageStringFT(fig->img, NULL,\
                            GetCouleur(fig->img,\
                                    style_tick.color),\
                            fig->fonts[ticklabel_f].path,\
                            fig->fonts[ticklabel_f].size,\
                                0., posX_ticklab, posY_ticklab, tickVal);

    }

    //Avoid memory leaks
    gdFontCacheShutdown();
}

#endif /* PLOTTER_H */