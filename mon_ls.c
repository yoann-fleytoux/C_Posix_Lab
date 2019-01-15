/************************************************************************************************************************/
/* mon_ls : affiche le contenu du répertoire passé en paramètre. Le chemin doit obligatoirement être absolu.		*/
/*															*/
/* C. Collet 2013 ® Université Paul Sabatier, Toulouse									*/
/************************************************************************************************************************/

#include "mon_SGF.h"

int main(int argc, char * argv[])
{
    SGF_t * mon_SGF;		/* ma structure de SGF */
    int inoeud_rep;
    dir_element_t * liste_rep = NULL;
    int i;

    if ( argc != 2 )
    {
	fprintf(stderr,"Usage: %s dir\n", argv[0]);
	exit (EXIT_PARAM);
    }
    if (argv[1][0]!='/')
    {
	fprintf(stderr,"%s: erreur de designation \"%s\" doit commencer par '/' (chemin absolu).\n", argv[0], argv[1]);
	exit (EXIT_PARAM);
    }

    mon_SGF = ouvrir_SGF();
    if (mon_SGF == NULL )
    {
	fprintf(stderr,"%s: ", argv[0]);
	perror (DEVICE_NAME);
	exit (EXIT_DEVICE);
    }

    inoeud_rep=inoeud_designation_element(argv[1], 0, mon_SGF);

    if (inoeud_rep == NO_INOEUD)
    {
	fprintf(stderr,"%s: chemin \"%s\" inexistant !\n", argv[0], argv[1]);
	exit (EXIT_PARAM);
    }

    if ((liste_rep =lire_liste_rep_dans_inoeud ( inoeud_rep, mon_SGF)) == NULL)
    {
	affiche_erreur( EXIT_DEVICE , argv[0]);
	exit (EXIT_DEVICE);
    }

    printf("%3d '%s' :\n", inoeud_rep, argv[1]);
    i=0;
    while( liste_rep[i].elt_inoeud != NO_INOEUD)
    {
	printf(" %3d %s\n", liste_rep[i].elt_inoeud, liste_rep[i].elt_name);
	i++;
    }

    mon_SGF=fermer_SGF(mon_SGF);

    exit (0);
}
