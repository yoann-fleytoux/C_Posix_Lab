/************************************************************************************************************************/
/* mon_mkdir : créé un nouveau répertoire.										*/
/*															*/
/* C. Collet 2013 ® Université Paul Sabatier, Toulouse									*/
/************************************************************************************************************************/

#include "mon_SGF.h"


int ajouter_repertoire( int inoeud_rep_sup, SGF_t *mon_SGF, char * pgname)
{
    int	num_inoeud;		/* inoeud du répertoire à créer */
    dir_element_t liste_rep[3];	/* élément contenu par le répertoire */

							/* créé le inoeud du nouveau répertoire */
    num_inoeud = creer_inoeud ( INOEUD_REPERTOIRE, mon_SGF);
    if (num_inoeud <0) 
    {
	fprintf(stderr,"%s: SF plein, plus d'inoeud disponible !\n",pgname );
	exit (EXIT_FS_FULL);
    }

    liste_rep[0].elt_inoeud = num_inoeud;		/* le répertoire pointe vers lui-même avec '.' */
    strcpy(liste_rep[0].elt_name, ".");
    liste_rep[1].elt_inoeud = inoeud_rep_sup;		/* le répertoire pointe vers son supérieur avec '..' */
    strcpy(liste_rep[1].elt_name, "..");
    liste_rep[2].elt_inoeud = NO_INOEUD;		/* et c'est tout, la liste s'arrête là */

    if (ecrire_donnees_dans_inoeud ( (char *) liste_rep, sizeof(liste_rep), num_inoeud, mon_SGF) != 0)
    {
	fprintf(stderr,"%s: SF plein, plus de bloc disponible !\n", pgname);
	exit (EXIT_FS_FULL);
    }
    return num_inoeud;
}


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
	fprintf(stderr,"%s: impossible de creer \"%s\" \n", argv[0], argv[1]);
	exit (EXIT_PARAM);
    }
    if (strlen(nom_rep) >= TAILLE_NOM_FICHIER)
    {
	fprintf(stderr,"%s: nom trop long \"%s\" \n", argv[0], nom_rep);
	exit (EXIT_PARAM);
    }
    if (strcmp(nom_rep,".")==0 || strcmp(nom_rep,"..")==0)
    {
	fprintf(stderr,"%s: nom \"%s\" reserve au SF \n", argv[0], nom_rep);
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

    if (inoeud_element_dans_repertoire(nom_rep, inoeud_rep_sup, mon_SGF) != NO_INOEUD)
    {
	fprintf(stderr,"%s: \"%s\" existe deja !\n", argv[0], argv[1]);
	exit (EXIT_PARAM);
    }

    inoeud_rep = ajouter_repertoire( inoeud_rep_sup, mon_SGF, argv[0]);

    erreur = lier_element_au_repertoire( nom_rep, inoeud_rep, inoeud_rep_sup, mon_SGF);
    if (erreur)
	affiche_erreur( erreur, argv[0]);

    mon_SGF=fermer_SGF(mon_SGF);

    exit (erreur);
}
