/************************************************************************************************************************/
/* mon_affiche_SGF : affiche le SGF du périphérique de mémoire de masse, simulé dans un fichier.			*/
/*															*/
/* C. Collet 2013 ® Université Paul Sabatier, Toulouse									*/
/************************************************************************************************************************/

#include "mon_SGF.h"


void affiche_usage ( char * pgname)
{
    fprintf(stderr,"Usage: %s [-b n]\n -b n : affiche le contenu du bloc n\n", pgname);
    exit (EXIT_PARAM);
}


void affiche_superbloc ( superbloc_t * mon_superbloc)
{
    assert( mon_superbloc != NULL );		/* DEBUG*/

    printf("Superbloc\n");
    printf("\033[01;31m");	/* couleur : rouge */
    printf(" nb_max_inoeuds    :%3d\n", mon_superbloc->nb_max_inoeuds);
    printf(" taille_du_SF      :%3d\n", mon_superbloc->taille_du_SF);
    printf(" taille_bloc       :%3d\n", mon_superbloc->taille_bloc);
    printf(" premier_bloc_libre:%3d\n", mon_superbloc->premier_bloc_libre);
    printf("\033[0m");		/* couleur : blanc */

}


void affiche_inoeud ( inoeud_t * mon_inoeud)
{
    int i;
    assert( mon_inoeud != NULL );		/* DEBUG*/

    switch (mon_inoeud->type_de_fichier)
    {
	case INOEUD_LIBRE :
	    printf("\033[01;35m");	/* couleur : mauve */
	    break;
	case INOEUD_FICHIER :
	    printf("\033[01;32m");	/* couleur : vert */
	    break;
	case INOEUD_REPERTOIRE :
	    printf("\033[01;34m");	/* couleur : bleu */
	    break;
    }
    printf("  type  :%3d", mon_inoeud->type_de_fichier);
    printf("  taille:%3d o", mon_inoeud->taille);
    printf(",  blocs:");
    for (i=0; i<NB_LIENS_DIRECTS; i++)
	printf("%3d,", mon_inoeud->liens_directs_blocs[i]);
    printf("\n");
    printf("\033[0m");			/* couleur : blanc */

}


void affiche_table_inoeuds ( SGF_t *mon_SGF)
{
    int i;

    assert( mon_SGF != NULL && mon_SGF->superbloc != NULL && mon_SGF->table_inoeuds != NULL );		/* DEBUG*/

    printf("Table des inoeuds\n");
    for (i=0; i < mon_SGF->superbloc->nb_max_inoeuds; i++)
    {
	printf("%3d", i);
	affiche_inoeud(&(mon_SGF->table_inoeuds[i]));
    }

}


void affiche_liste_blocs_libres ( SGF_t *mon_SGF)
{
    int num_bloc;

    assert( mon_SGF != NULL && mon_SGF->superbloc != NULL );		/* DEBUG*/

    printf("Liste des blocs libres :\n ");
    num_bloc = mon_SGF->superbloc->premier_bloc_libre;		/* premier bloc libre */
    while ( num_bloc != NULL_BLOC )
    {
	printf("%3d ", num_bloc);
	num_bloc = bloc_libre_suivant( num_bloc, mon_SGF);
    }
    printf("\n");

}


void affiche_SGF ( SGF_t *mon_SGF)
{
    assert( mon_SGF != NULL && mon_SGF->superbloc != NULL );		/* DEBUG*/

    printf("\033[01;36m");		/* couleur : magenta */
    printf("**********************************\n*");
    printf("\033[0;33m");		/* couleur : marron */
    printf(" Systeme de Gestion de Fichiers ");
    printf("\033[01;36m");		/* couleur : magenta */
    printf("*\n**********************************\n");
    printf("\033[0m");		/* couleur : blanc */
    affiche_superbloc ( mon_SGF->superbloc);
    affiche_table_inoeuds ( mon_SGF);
    affiche_liste_blocs_libres ( mon_SGF);
}


void affiche_bloc( int num_bloc, SGF_t *mon_SGF)
{
    char *bloc;			/* bloc à allouer dynamiquement en fonction de la taille d'un bloc */
    int i,j;

    assert( mon_SGF != NULL && mon_SGF->superbloc != NULL );		/* DEBUG*/

    bloc = malloc(mon_SGF->superbloc->taille_bloc);
    if (bloc == NULL)
    {
	fprintf(stderr,"affiche_bloc: pb allocation mémoire\n");
	return ;
    }
    if (lire_bloc ( bloc, num_bloc, mon_SGF) != 0)
    {
	fprintf(stderr,"affiche_bloc: pb lecture bloc %d\n", num_bloc);
	free(bloc);
	return ;
    }
    printf("Bloc %3d :", num_bloc);
    printf("\033[01;36m");		/* couleur : magenta */
    for (i=0; i < mon_SGF->superbloc->taille_bloc; i+=16)
    {
	printf("\n%3d : ", i);
	for (j=0; j < 16; j++)
	    printf("%3d ", bloc[i+j]);
    }
    printf("\n");
    printf("\033[0m");			/* couleur : blanc */

    free(bloc);
}



int main(int argc, char * argv[])
{
    SGF_t * mon_SGF;		/* ma structure de SGF */
    int	num_bloc = NULL_BLOC;	/* numero du bloc à afficher */
    int i;

    if ( argc > 3 )
	affiche_usage( argv[0]);

    i = 1;
    while (i < argc )
    {
	if (argv[i][0] == '-' && argv[i][1] == 'b')
	{
	    if (argv[i][2] >= '0' && argv[i][2] <= '9')
		num_bloc = atoi(argv[i]+2);
	    else if (i+1 < argc && argv[i+1][0] >= '0' && argv[i+1][0] <= '9')
	    {
		i++;
		num_bloc = atoi(argv[i]);
	    }
	    else
		affiche_usage( argv[0]); 
	}
	else
	    affiche_usage( argv[0]);  

	i++;
    }


    mon_SGF = ouvrir_SGF();
    if (mon_SGF == NULL )
    {
	fprintf(stderr,"%s: ", argv[0]);
	perror (DEVICE_NAME);
	exit (EXIT_DEVICE);
    }

    if (num_bloc == NULL_BLOC)
	affiche_SGF( mon_SGF);
    else
	affiche_bloc( num_bloc, mon_SGF);

    mon_SGF=fermer_SGF(mon_SGF);

    exit (0);
}
