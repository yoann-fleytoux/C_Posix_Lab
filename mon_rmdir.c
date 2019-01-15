/************************************************************************************************************************/
/* mon_rmdir : efface un répertoire.											*/
/*															*/
/* C. Collet 2013 ® Université Paul Sabatier, Toulouse									*/
/************************************************************************************************************************/

#include "mon_SGF.h"


int main(int argc, char * argv[])
{
    SGF_t * mon_SGF;		/* ma structure de SGF */
    int inoeud_rep, inoeud_rep_sup;
    char * nom_rep, *designation;
    int erreur = 0;

    if ( argc != 2 )
    {
	fprintf(stderr,"Usage: %s dir\n", argv[0]);
	exit (EXIT_PARAM);
    }
    if (argv[1][0]!='/')
    {
	fprintf(stderr,"%s: erreur de designation \"%s\" doit commencer par '/'\n", argv[0], argv[1]);
	exit (EXIT_PARAM);
    }

    designation=malloc(strlen(argv[1])+1);	/* copie le chemin pour modifier la chaine */
    strcpy(designation, argv[1]);
    nom_rep=basename(argv[1]);

    if (nom_rep[0] == '/')
    {
	fprintf(stderr,"%s: impossible de detruire \"%s\" \n", argv[0], argv[1]);
	exit (EXIT_PARAM);
    }

    mon_SGF = ouvrir_SGF();
    if (mon_SGF == NULL )
    {
	fprintf(stderr,"%s: ", argv[0]);
	perror (DEVICE_NAME);
	exit (EXIT_DEVICE);
    }

    inoeud_rep_sup=inoeud_designation_element(dirname(designation), 0, mon_SGF);

    if (inoeud_rep_sup == NO_INOEUD)
    {
	fprintf(stderr,"%s: chemin \"%s\" inexistant !\n", argv[0], argv[1]);
	exit (EXIT_PARAM);
    }

    if ((inoeud_rep = inoeud_element_dans_repertoire(nom_rep, inoeud_rep_sup, mon_SGF)) == NO_INOEUD)
    {
	fprintf(stderr,"%s: \"%s\" n'existe pas !\n", argv[0], argv[1]);
	exit (EXIT_PARAM);
    }

    if (mon_SGF->table_inoeuds[inoeud_rep].taille > sizeof(dir_element_t)*3)
    {
	fprintf(stderr,"%s: le repertoire \"%s\" n'est pas vide !\n", argv[0], argv[1]);
	exit (EXIT_PARAM);
    }

    erreur = delier_element_du_repertoire( nom_rep, inoeud_rep, inoeud_rep_sup, mon_SGF);
    if (erreur)
    {
	affiche_erreur( erreur, argv[0]);
	exit (erreur);
    }

    erreur = liberer_inoeud (inoeud_rep, mon_SGF);
    if (erreur)
	affiche_erreur( erreur, argv[0]);

    mon_SGF=fermer_SGF(mon_SGF);

    exit (erreur);
}
