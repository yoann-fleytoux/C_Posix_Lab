/************************************************************************************************************************/
/* mon_formater : formate le périphérique de mémoire de masse, simulé dans un fichier, pour y installer un SF.		*/
/*															*/
/* C. Collet 2013 ® Université Paul Sabatier, Toulouse									*/
/************************************************************************************************************************/

#include "mon_SGF.h"

#define _NB_MAX_INOEUDS_	10
#define _TAILLE_BLOC_		64
#define _NB_MAX_BLOCS_		64


/* création du périphérique de mémoire de masse, simulé dans un fichier  */
int create_device( char * pgname)
{
    int mon_device;	/* nom du périphérique de mémoire de masse, simulé dans un fichier  */

    mon_device = open (DEVICE_NAME, O_RDWR | O_CREAT | O_EXCL, S_IRUSR | S_IWUSR);
    if ( mon_device == -1 )
    {
    	if (errno == EEXIST)	/* si le fichier existe déjà demande confirmation à l'utilisateur */
    	{
    	    char reponse;	/* réponse à la question fermée - Oui ou Non - ci-dessous */

    	    printf("%s: le périphérique \"%s\" contient peut-être des données qui vont être supprimées.\n", pgname, DEVICE_NAME);
    	    printf("%s: êtes-vous sûr de vouloir le formater ? [oN] :", pgname);
    	    reponse = (char)getchar();
    	    while(getchar() != '\n');			/* passe la suite de la ligne */

    	    if ( reponse != 'o' && reponse != 'O' )
    	    {
        		printf("%s: abandon\n", pgname);
        		exit (EXIT_SUCCESS);
    	    }

    	    mon_device = open (DEVICE_NAME, O_RDWR | O_TRUNC, S_IRUSR | S_IWUSR);
    	}
    	if ( mon_device == -1 )
    	{
    	    fprintf(stderr,"%s: ", pgname);
    	    perror (DEVICE_NAME);
    	    exit (EXIT_DEVICE);
    	}
    }
    return mon_device;
}


void init_constantes_superbloc ( superbloc_t *mon_superbloc )
{	/* Valeurs contantes par défaut, tant qu'elles ne sont pas initialisées en arguments du main */

    assert( mon_superbloc != NULL );		/* DEBUG*/

    mon_superbloc->nb_max_inoeuds = _NB_MAX_INOEUDS_;	/* nombre max de fichiers (inoeuds) que peut contenir le SF */
    mon_superbloc->taille_du_SF   = _NB_MAX_BLOCS_;	/* taille en nombre de blocs du SF */
    mon_superbloc->taille_bloc    = _TAILLE_BLOC_;	/* taille d'un bloc du SF */

    							/* numéro du premier bloc libre, liste chainée de blocs libres */
    mon_superbloc->premier_bloc_libre = 2 + (mon_superbloc->nb_max_inoeuds * sizeof(inoeud_t) -1)/mon_superbloc->taille_bloc;
}


void init_constantes_table_inoeuds ( SGF_t *mon_SGF)	/* initialise la tables des inoeuds avec des valeurs contantes */
{
    inoeud_t	inoeud_vide;				/* inoeud vide pour initialisation de la table des inoeuds */
    int i;

    assert( mon_SGF != NULL && mon_SGF->superbloc != NULL && mon_SGF->table_inoeuds != NULL );		/* DEBUG*/

    inoeud_vide.type_de_fichier = INOEUD_LIBRE;		/* initialise du inoeud vide avec le type inoeud libre et des numéros de bloc vide */
    inoeud_vide.taille = 0;				/* initialise du inoeud vide avec la taille 0 octets */
    for (i=0; i<NB_LIENS_DIRECTS; i++)
	inoeud_vide.liens_directs_blocs[i] = NULL_BLOC;

    for (i=0; i<mon_SGF->superbloc->nb_max_inoeuds; i++)/* initialise les inoeuds avec le inoeud_vide */
	mon_SGF->table_inoeuds[i] = inoeud_vide;
}


/* remplit le périphérique avec le superbloc, la table des inoeuds et des blocs libres chaînés */
void fill_device(char * pgname, SGF_t *mon_SGF)
{
	/* allocation (par écriture) des blocs sur le périphérique de mémoire de masse */
    int i;
    int erreur;
    char *bloc;			/* bloc à allouer dynamiquement en fonction de la taille d'un bloc */

    assert( mon_SGF != NULL && mon_SGF->superbloc != NULL && mon_SGF->table_inoeuds != NULL );		/* DEBUG*/

    if ( mon_SGF->superbloc->taille_bloc < sizeof(superbloc_t) )
    {
	fprintf(stderr,"%s: taille d'un bloc %d o < %ld o (taille min)\n", pgname, mon_SGF->superbloc->taille_bloc, (long int)sizeof(superbloc_t));
	exit (EXIT_TAILLE_BLOC);
    }

							/* écrit le superbloc en premier, dans le bloc 0 */
    if ((erreur = ecrire_superbloc (mon_SGF)) > 0)
    {
	affiche_erreur( erreur, pgname);
	exit (erreur);
    }
							/* écrit la table des inoeuds dans les blocs suivants */
    if ((erreur = ecrire_table_inoeuds (mon_SGF)) > 0)
    {
	affiche_erreur( erreur, pgname);
	exit (erreur);
    }

    bloc = malloc(mon_SGF->superbloc->taille_bloc);
    if (bloc == NULL)
    {
	fprintf(stderr,"%s: pb allocation mémoire\n", pgname);
	exit (EXIT_MEM_ALLOC);
    }

							/* avance jusqu'au premier bloc libre */
    lseek(mon_SGF->device_num, mon_SGF->superbloc->premier_bloc_libre * mon_SGF->superbloc->taille_bloc, SEEK_SET);
    memset (bloc, 0, mon_SGF->superbloc->taille_bloc);
    for (i=mon_SGF->superbloc->premier_bloc_libre; i<mon_SGF->superbloc->taille_du_SF-1; i++)
    {
	*(int *)bloc = i+1;				/* chainage des blocs libre, numéro du bloc libre suivant */
	if (write(mon_SGF->device_num, bloc, mon_SGF->superbloc->taille_bloc) < mon_SGF->superbloc->taille_bloc)
	{
	    fprintf(stderr,"%s: ", pgname);
	    perror (DEVICE_NAME);
	    exit (EXIT_WRITE_PB);
	}
    }
    *(int *)bloc = NULL_BLOC;				/* chainage des blocs libres, fin de chaine */
    if (write(mon_SGF->device_num, bloc, mon_SGF->superbloc->taille_bloc) < mon_SGF->superbloc->taille_bloc)
    {
	fprintf(stderr,"%s: ", pgname);
	perror (DEVICE_NAME);
	exit (EXIT_WRITE_PB);
    }
}


void ajoute_inoeud_racine(char * pgname, SGF_t *mon_SGF)
{
    int		num_inoeud_racine;	/* inoeud racine du répertoire racine à créer en premier */
    dir_element_t element_racine[2];	/* élément contenu par le répertoire racine */
    int erreur;

    assert( mon_SGF != NULL && mon_SGF->superbloc != NULL && mon_SGF->table_inoeuds != NULL );		/* DEBUG*/

							/* créé le premier inoeud avec le répertoire racine "/" */
    num_inoeud_racine = creer_inoeud ( INOEUD_REPERTOIRE, mon_SGF);
    if (num_inoeud_racine <0) 
    {
	fprintf(stderr,"%s: SF plein, plus d'inoeud disponible !\n",pgname );
	exit (EXIT_FS_FULL);
    }

    element_racine[0].elt_inoeud = num_inoeud_racine;		/* le répertoire racine, pointe vers lui-même avec '.' */
    strcpy(element_racine[0].elt_name, ".");
    element_racine[1].elt_inoeud = NO_INOEUD;			/* et c'est tout, la liste s'arrête là */

    erreur=ecrire_donnees_dans_inoeud ( (char *) element_racine, sizeof(element_racine), num_inoeud_racine, mon_SGF);
    if ( erreur )
    {
	affiche_erreur( erreur, pgname);
	exit (erreur);
    }
}


int main(int argc, char * argv[])
{
    SGF_t	mon_SGF;		/* ma structure de SGF */
    superbloc_t	mon_superbloc;		/* ma structure de Superbloc */


    if ( argc > 1 )
    {
	fprintf(stderr,"Usage: %s\n", argv[0]);
	exit (EXIT_PARAM);
    }

/* initialisation des structures du SF  */

    mon_SGF.device_num = create_device(argv[0]);	/* numéro du périphérique de mémoire de masse géré */
    if (mon_SGF.device_num < 0)				/* normalement l'erreur est déjà gérée dans create_device */
	exit (EXIT_DEVICE);

    init_constantes_superbloc (&mon_superbloc);		/* initialise le superbloc avec des valeurs contantes par défaut */

    mon_SGF.superbloc = &mon_superbloc;			/* pointeur vers la structure du superbloc en mémoire */

    mon_SGF.table_inoeuds = malloc(mon_SGF.superbloc->nb_max_inoeuds * sizeof(inoeud_t));
    if (mon_SGF.table_inoeuds == NULL)			/* pointeur vers la table des inoeuds */
    {
	fprintf(stderr,"%s: pb allocation mémoire\n", argv[0]);
	exit (EXIT_MEM_ALLOC);
    }

    init_constantes_table_inoeuds (&mon_SGF);		/* initialise la tables des inoeuds avec des valeurs contantes */

    fill_device(argv[0], &mon_SGF);			/* allocation (par écriture) des blocs sur le périphérique de mémoire de masse */

    ajoute_inoeud_racine(argv[0], &mon_SGF);

    close(mon_SGF.device_num);

    printf("%s: nb inoeuds: %d, nb blocs %d,", argv[0], mon_SGF.superbloc->nb_max_inoeuds, mon_SGF.superbloc->taille_du_SF );
    printf(" taille d'un bloc %d o (taille min: %ld o)\n", mon_SGF.superbloc->taille_bloc, (long int)sizeof(superbloc_t));
    printf("%s: terminé \n", argv[0]);

    exit (0);
}
