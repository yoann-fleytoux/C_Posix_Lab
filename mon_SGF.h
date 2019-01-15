/************************************************************************************************************************/
/* mon_SGF.h : structures de données et API des fonctions de gestion d'un SGF : Système de Gestion de Fichiers.		*/
/*															*/
/* C. Collet 2013 ® Université Paul Sabatier, Toulouse									*/
/************************************************************************************************************************/


/*	!	NE RIEN MODIFIER DANS CE FICHIER	!	*/


#define _POSIX_C_SOURCE 1	/* Pour respecter la norme POSIX.1 */
#include <stdio.h>		/* Pour printf et perror */
#include <unistd.h>		/* Pour lseek, close, read, write... */
#include <stdlib.h>		/* Pour exit , EXIT_SUCCESS et EXIT_FAILURE */
#include <sys/types.h>		/* types implémentés différemment selon la version d'Unix */
#include <sys/stat.h>		/* constantes symboliques pour les droits d'accès */
#include <fcntl.h>		/* constantes symboliques pour les différents types d'ouverture */
#include <errno.h>		/* messages d'erreur associés aux valeurs de la variable errno */
#include <string.h>		/* Pour memcpy, memset... */
#include <libgen.h>		/* Pour dirname et basename */
#include <assert.h>		/* A N'UTILISER QUE POUR LA MISE AU POINT */


/* Valeurs par défauts du SGF */

#define DEVICE_NAME		".device_name"	/* nom du périphérique de mémoire de masse, simulé dans un fichier */
#define NB_LIENS_DIRECTS	10		/* nombre de blocs utilisables pour un fichier */
#define TAILLE_NOM_FICHIER	20		/* taille max d'un nom de fichier dans ce SF */


/* Erreurs renvoyées par exit ou return */

#define EXIT_PARAM		1		/* paramètres incorrects */
#define EXIT_DEVICE		2		/* pb d'acces au fichier qui sert de mémoire de masse */
#define EXIT_TAILLE_BLOC	3		/* la taille d'un bloc n'est pas >= taille du superbloc */
#define EXIT_MEM_ALLOC		4		/* pb de malloc */
#define EXIT_DEVICE_SIZE	5		/* accès à un bloc inexistant ou taille d'écriture dans un bloc > taille d'un bloc */
#define EXIT_WRITE_PB		6		/* pb d'écriture sur le disque (après un write) */
#define EXIT_READ_PB		7		/* pb de lecture sur le disque (après un read)*/
#define EXIT_FS_FULL		8		/* le SGF est plein */
#define EXIT_FS_CORRUPTED	9		/* incohérence du SGF */


/* Constantes de manipulation des inoeuds */

#define NO_INOEUD		-1		/* pas de inoeud libre ou inoeud pas trouvé ou fin de liste d'éléments d'un répertoire */

#define INOEUD_LIBRE		0		/* inoeud libre :  pas encore utilisé */
#define INOEUD_FICHIER		1		/* inoeud de type fichier ordinaire */
#define INOEUD_REPERTOIRE	2		/* inoeud de type répertoire */


/* Constantes de manipulation des blocs */

#define NULL_BLOC		-1		/* valeur de fin de liste chaînée de blocs libres */



/*	!	NE RIEN MODIFIER DANS CE FICHIER	!	*/



/********************************************************************************/
/* dir_element_t : type de structure de données d'un élément de la liste des	*/
/*  éléments d'un répertoire.							*/
/********************************************************************************/
typedef struct
{
  int elt_inoeud;			/* inoeud du fichier ordinaire ou sous-répertoire de ce répertoire */
  char elt_name [TAILLE_NOM_FICHIER] ;	/* nom de l'élément */
} dir_element_t ;		/* ma structure d'élément d'un répertoire */


/********************************************************************************/
/* inoeud_t : type de structure de données d'un inoeud.				*/
/********************************************************************************/
typedef struct
{
  int type_de_fichier;				/* fichier ordinaire ou répertoire */
  int taille;					/* taille en octets du fichier ordinaire ou répertoire */
  int liens_directs_blocs [ NB_LIENS_DIRECTS ];	/* liens directs vers des blocs de données sur le périphérique */
} inoeud_t ;			/* ma structure de inoeud */


/********************************************************************************/
/* superbloc_t : type de structure de données du superbloc.			*/
/********************************************************************************/
typedef struct
{
  int nb_max_inoeuds;			/* nombre max de fichiers (inoeuds) que peut contenir le SF */
  int taille_du_SF;			/* taille en nombre de blocs du SF */
  int taille_bloc;			/* taille d'un bloc du SF */
  int premier_bloc_libre ;		/* numéro du premier bloc libre, liste chainée de blocs libres */
} superbloc_t ;			/* ma structure de Superbloc */


/********************************************************************************/
/* SGF_t : type de structure de données du Système de Gestion de Fichiers.	*/
/********************************************************************************/
typedef struct
{
  int device_num;			/* numéro du périphérique de mémoire de masse géré */
  superbloc_t * superbloc;		/* pointeur vers la structure du superbloc en mémoire */
  inoeud_t * table_inoeuds ;		/* pointeur vers la table des inoeuds */
} SGF_t ;			/* ma structure de SGF */


extern	void affiche_erreur(int num_erreur, char * pgname);

extern	int ecrire_superbloc ( SGF_t *mon_SGF);

extern	int lire_superbloc ( SGF_t *mon_SGF);

extern	int ecrire_table_inoeuds ( SGF_t *mon_SGF);

extern	int lire_table_inoeuds ( SGF_t *mon_SGF);

extern	int lire_SGF ( SGF_t *mon_SGF);

extern	SGF_t * ouvrir_SGF ();

extern	SGF_t * fermer_SGF ( SGF_t *mon_SGF);

extern	int ecrire_donnees_dans_bloc ( char * data, int size, int num_bloc, SGF_t *mon_SGF);

extern	int ecrire_bloc ( char * bloc, int num_bloc, SGF_t *mon_SGF);

extern	int lire_donnees_dans_bloc ( char * data, int size, int num_bloc, SGF_t *mon_SGF);

extern	int lire_bloc ( char * bloc, int num_bloc, SGF_t *mon_SGF);

extern	int inoeud_libre ( SGF_t *mon_SGF);

extern	int bloc_libre_suivant ( int num_bloc, SGF_t *mon_SGF);

extern	int liberer_blocs_du_inoeud ( int inoeud, int premier_bloc, SGF_t *mon_SGF);

extern	int allouer_n_blocs_dans_inoeud ( int nb_blocs_a_allouer, int inoeud, SGF_t *mon_SGF);

extern	int ecrire_donnees_dans_inoeud ( char * data, int data_size, int inoeud, SGF_t *mon_SGF);

extern	int lire_donnees_dans_inoeud ( char * data, int data_size, int inoeud, SGF_t *mon_SGF);

extern	int creer_inoeud ( int type_inoeud, SGF_t *mon_SGF);

extern	int liberer_inoeud ( int inoeud, SGF_t *mon_SGF);

extern	dir_element_t *  lire_liste_rep_dans_inoeud ( int inoeud_rep, SGF_t *mon_SGF);

extern	int delier_element_du_repertoire(char *nom_elt, int inoeud_elt, int inoeud_rep, SGF_t *mon_SGF);

extern	int lier_element_au_repertoire(char *nom_elt, int inoeud_elt, int inoeud_rep, SGF_t *mon_SGF);

extern	char * nom_suivant (char * designation);

extern	int inoeud_element_dans_repertoire(char * nom_element, int inoeud_rep, SGF_t *mon_SGF);

extern	int inoeud_designation_element(char * designation, int inoeud_org, SGF_t *mon_SGF);

