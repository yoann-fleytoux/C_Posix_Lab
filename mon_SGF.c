/************************************************************************************************************************/
/* mon_SGF.c : fonctions de gestion d'un SGF : Syst�me de Gestion de Fichiers.						*/
/*															*/
/* C. Collet 2013 � Universit� Paul Sabatier, Toulouse									*/
/************************************************************************************************************************/

#include "mon_SGF.h"


/********************************************************************************/
/* affiche_erreur : affiche un message d'erreur en fonction de "num_erreur".	*/
/********************************************************************************/

void affiche_erreur(int num_erreur, char * pgname)
{
    switch (num_erreur)
    {
	case EXIT_FS_FULL :
	    fprintf(stderr,"%s: SF plein, plus d'inoeud disponible !\n",pgname );
	    break;
	case EXIT_FS_CORRUPTED :
	    fprintf(stderr,"%s: SF corrompu !\n",pgname );
	    break;
	case EXIT_MEM_ALLOC :
	    fprintf(stderr,"%s: pb allocation memoire !\n",pgname );
	    break;
	case EXIT_DEVICE :
	case EXIT_WRITE_PB :
	case EXIT_READ_PB :
	    fprintf(stderr,"%s: ",pgname );
	    perror (DEVICE_NAME);
	    break;
	case EXIT_DEVICE_SIZE :
	    fprintf(stderr,"%s: PB taille de donn�es incompatible avec le SF !\n",pgname );
	    break;
	default  :
	    fprintf(stderr,"%s: ",pgname );
	    perror (DEVICE_NAME);
	    break;
    }
}


/********************************************************************************/
/* ecrire_superbloc : �crit au d�but (bloc num�ro 0) du fichier ouvert		*/
/*  "mon_SGF->device_num" le contenu de "mon_SGF->superbloc".			*/
/* Renvoie 0 si tout va bien ou un code d'erreur sinon.				*/
/********************************************************************************/

int ecrire_superbloc ( SGF_t *mon_SGF)
{
    assert( mon_SGF != NULL && mon_SGF->superbloc != NULL );		/* DEBUG*/

    lseek (mon_SGF->device_num, 0, SEEK_SET);	/* �crit le superbloc dans le bloc 0 */

    if (write(mon_SGF->device_num, mon_SGF->superbloc, sizeof(superbloc_t)) < sizeof(superbloc_t))
	return EXIT_WRITE_PB;

    return 0;					/* tout va bien */
}


/********************************************************************************/
/* lire_superbloc : copie dans "mon_SGF->superbloc" les donn�es de superbloc	*/
/*  lues au d�but (bloc num�ro 0) du fichier ouvert "mon_SGF->device_num".	*/
/* Renvoie 0 si tout va bien ou un code d'erreur sinon.				*/
/********************************************************************************/

int lire_superbloc ( SGF_t *mon_SGF)
{
/* 	A FAIRE 	*/
    return 0;					/* tout va bien */
}


/********************************************************************************/
/* ecrire_table_inoeuds : �crit � partir du bloc num�ro 1, dans le fichier	*/
/*  ouvert "mon_SGF->device_num" le contenu de "mon_SGF->table_inoeuds".	*/
/* Renvoie 0 si tout va bien ou un code d'erreur sinon.				*/
/********************************************************************************/

int ecrire_table_inoeuds ( SGF_t *mon_SGF)
{
    assert( mon_SGF != NULL && mon_SGF->superbloc != NULL && mon_SGF->table_inoeuds != NULL );		/* DEBUG*/

					/* avance jusqu'au bloc 1 */
    lseek(mon_SGF->device_num, mon_SGF->superbloc->taille_bloc, SEEK_SET);

					/* �crit la table des inoeuds dans les blocs suivants */
    if (write(mon_SGF->device_num, mon_SGF->table_inoeuds, mon_SGF->superbloc->nb_max_inoeuds * sizeof(inoeud_t))
	< mon_SGF->superbloc->nb_max_inoeuds * sizeof(inoeud_t))
	return EXIT_WRITE_PB;

    return 0;				/* tout va bien */
}


/********************************************************************************/
/* lire_table_inoeuds : copie dans "mon_SGF->table_inoeuds" les donn�es de la	*/
/*  table des inoeuds lues � partir du bloc num�ro 1 dans le fichier ouvert	*/
/*  "mon_SGF->device_num".							*/
/* Renvoie 0 si tout va bien ou un code d'erreur sinon.				*/
/********************************************************************************/

int lire_table_inoeuds ( SGF_t *mon_SGF)
{
 /* 	A FAIRE 	*/
   return 0;				/* tout va bien */
}


/********************************************************************************/
/* lire_SGF : alloue en m�moire, si besoin, les structures "mon_SGF->superbloc"	*/
/*  et "mon_SGF->table_inoeuds", et y copie les donn�es correspondantes lues	*/
/*  dans le fichier ouvert "mon_SGF->device_num".				*/
/* Renvoie 0 si tout va bien ou un code d'erreur sinon.				*/
/********************************************************************************/

int lire_SGF ( SGF_t *mon_SGF)
{
/* 	A FAIRE 	*/
    return 0;							/* tout va bien */
}


/********************************************************************************/
/* ouvrir_SGF : alloue en m�moire la structure "mon_SGF" et ouvre en lecture-	*/
/*  �criture le fichier d�j� existant "DEVICE_NAME", dont le num�ro est copi�	*/
/*  dans "mon_SGF->device_num". Puis lit les donn�es des structures du SGF.	*/
/* Renvoie un pointeur vers une structure SGF initialis�e et pr�te � l'emploi	*/
/*  pour d'autres op�rations sur le SGF.					*/
/********************************************************************************/

SGF_t * ouvrir_SGF ()
{
/* 	A FAIRE 	*/
    return NULL;
}


/********************************************************************************/
/* fermer_SGF : ferme le fichier "mon_SGF->device_num" et lib�re de la m�moire	*/
/*  les strutures du SGF.							*/
/* Renvoie NULL									*/
/********************************************************************************/

SGF_t * fermer_SGF ( SGF_t *mon_SGF)
{
    assert( mon_SGF != NULL && mon_SGF->superbloc != NULL && mon_SGF->table_inoeuds != NULL );		/* DEBUG*/

    close(mon_SGF->device_num);
    free(mon_SGF->superbloc);
    free(mon_SGF->table_inoeuds);
    free(mon_SGF);
    return NULL;
}



/********************************************************************************/
/* ecrire_donnees_dans_bloc : �crit dans le bloc "num_bloc", du fichier ouvert	*/
/*  "mon_SGF->device_num" le contenu de "data" de "size" octets.		*/
/* !  "size" doit �tre <= taille d'un bloc					*/
/* Renvoie 0 si tout va bien ou un code d'erreur sinon.				*/
/********************************************************************************/

int ecrire_donnees_dans_bloc ( char * data, int size, int num_bloc, SGF_t *mon_SGF)
{
    assert( mon_SGF != NULL && mon_SGF->superbloc != NULL && data != NULL );		/* DEBUG*/

    if ( num_bloc == NULL_BLOC || num_bloc >= mon_SGF->superbloc->taille_du_SF || size > mon_SGF->superbloc->taille_bloc)
	return EXIT_DEVICE_SIZE;

					/* avance jusqu'au bloc num_bloc */
    lseek (mon_SGF->device_num, num_bloc * mon_SGF->superbloc->taille_bloc, SEEK_SET);
    if (write (mon_SGF->device_num, data, size) < size)
	return EXIT_WRITE_PB;

    return 0;				/* tout va bien */
}


/********************************************************************************/
/* ecrire_bloc : �crit dans le bloc "num_bloc", du fichier ouvert		*/
/*  "mon_SGF->device_num" tout le contenu de "bloc".				*/
/* Renvoie 0 si tout va bien ou un code d'erreur sinon.				*/
/********************************************************************************/

int ecrire_bloc ( char * bloc, int num_bloc, SGF_t *mon_SGF)
{
    return ecrire_donnees_dans_bloc( bloc, mon_SGF->superbloc->taille_bloc, num_bloc, mon_SGF);
}


/********************************************************************************/
/* lire_donnees_dans_bloc : copie dans "data" les "size" octets de donn�es lues	*/
/*  dans le bloc "num_bloc", du fichier ouvert "mon_SGF->device_num".		*/
/* !  "size" doit �tre <= taille d'un bloc					*/
/* Renvoie 0 si tout va bien ou un code d'erreur sinon.				*/
/********************************************************************************/

int lire_donnees_dans_bloc ( char * data, int size, int num_bloc, SGF_t *mon_SGF)
{
    assert( mon_SGF != NULL && mon_SGF->superbloc != NULL && data != NULL );		/* DEBUG*/

    if ( num_bloc == NULL_BLOC || num_bloc >= mon_SGF->superbloc->taille_du_SF || size > mon_SGF->superbloc->taille_bloc)
	return EXIT_DEVICE_SIZE;

					/* avance jusqu'au bloc num_bloc */
    lseek (mon_SGF->device_num, num_bloc * mon_SGF->superbloc->taille_bloc, SEEK_SET);
    if (read (mon_SGF->device_num, data, size) < size)
	return EXIT_READ_PB;

    return 0;				/* tout va bien */
}


/********************************************************************************/
/* lire_bloc : copie dans "bloc" les donn�es lues dans tout le bloc "num_bloc"	*/
/*  du fichier ouvert "mon_SGF->device_num".					*/
/* Renvoie 0 si tout va bien ou un code d'erreur sinon.				*/
/********************************************************************************/

int lire_bloc ( char * bloc, int num_bloc, SGF_t *mon_SGF)
{
    return lire_donnees_dans_bloc( bloc, mon_SGF->superbloc->taille_bloc, num_bloc, mon_SGF);
}


/********************************************************************************/
/* inoeud_libre : cherche dans la table des inoeuds - "mon_SGF->table_inoeuds"	*/
/*  le num�ro du premier inoeud libre.						*/
/* Renvoie le num�ro du premier inoeud libre ou "NO_INOEUD" s'il n'y en a pas.	*/
/********************************************************************************/

int inoeud_libre ( SGF_t *mon_SGF)
{
/* 	A FAIRE 	*/
    return NO_INOEUD;		/* pas de inoeud libre */
}


/********************************************************************************/
/* bloc_libre_suivant : lit dans le bloc libre "num_bloc", le num�ro du bloc	*/
/*  libre suivant.								*/
/* !  "num_bloc" doit �tre le num�ro d'un bloc libre. 				*/
/* Renvoie le num�ro du bloc libre suivant ou "NULL_BLOC" s'il n'y en a pas.	*/
/********************************************************************************/

int bloc_libre_suivant ( int num_bloc, SGF_t *mon_SGF)
{
/* 	A FAIRE 	*/
    return NULL_BLOC;
}


/********************************************************************************/
/* liberer_blocs_du_inoeud : lib�re les blocs du inoeud "inoeud" � partir du	*/
/*  num�ro "premier_bloc" jusqu'au dernier utilis� de la table			*/
/*  "liens_directs_blocs" du "inoeud".						*/
/*  Les �l�ments de la table "liens_directs_blocs" lib�r�s prennent la valeur	*/
/*  "NULL_BLOC".								*/
/*  Les blocs lib�r�s sont ajout�s en t�te de la liste cha�n�e de blocs libres,	*/
/*  en �crivant dans chaque bloc lib�r� le num�ro du bloc libre suivant et en	*/
/*  mettant � jour le num�ro du premier bloc libre (t�te de la liste) dans	*/
/*  "mon_SGF->superbloc->premier_bloc_libre".					*/
/*  Le superbloc et la table des inoeuds sont mis � jour sur le SGF (�crits	*/
/*  dans le fichier du SGF).							*/
/* Renvoie 0 si tout va bien ou un code d'erreur sinon.				*/
/********************************************************************************/

int liberer_blocs_du_inoeud ( int inoeud, int premier_bloc, SGF_t *mon_SGF)
{
/* 	A FAIRE 	*/
    return 0;							/* tout va bien */
}


/********************************************************************************/
/* allouer_n_blocs_dans_inoeud : prend dans la liste des blocs libres les	*/
/*  "nb_blocs_a_allouer" premiers blocs libres et les alloue � l'inoeud		*/
/*  "inoeud" en mettant le num�ro des blocs aux emplacements libres de la table	*/
/*  "liens_directs_blocs" du inoeud "inoeud".					*/
/*  Met � jour le num�ro du premier bloc libre (t�te de la liste) dans		*/
/*  "mon_SGF->superbloc->premier_bloc_libre".					*/
/*  Le superbloc et la table des inoeuds sont mis � jour sur le SGF (�crits	*/
/*  dans le fichier du SGF).							*/
/* Renvoie 0 si tout va bien ou un code d'erreur sinon (notamment EXIT_FS_FULL	*/
/*  s'il n'y a plus de blocs libres ou d'emplacements libres dans la table	*/
/*  "liens_directs_blocs").							*/
/********************************************************************************/

int allouer_n_blocs_dans_inoeud ( int nb_blocs_a_allouer, int inoeud, SGF_t *mon_SGF)
{
/* 	A FAIRE 	*/
    return 0;							/* tout va bien */
}


/********************************************************************************/
/* ecrire_donnees_dans_inoeud : �crit dans les blocs du inoeud "inoeud", le	*/
/*  contenu de "data" de "data_size" octets. Si le nombre de blocs allou�s au	*/
/*  inoeud "inoeud" n'est pas suffisant, alloue plus de blocs, et s'il est trop	*/
/*  grand, lib�re les blocs inutiles. �crit les donn�es de "data" bloc par bloc	*/
/*  (�crit "mon_SGF->superbloc->taille_bloc" octets de donn�es � chaque fois)	*/
/*  jusqu'� l'avant dernier bloc. �crit les donn�es restantes et pas plus, dans	*/
/*  le dernier bloc.								*/
/* Renvoie 0 si tout va bien ou un code d'erreur sinon.				*/
/********************************************************************************/

int ecrire_donnees_dans_inoeud ( char * data, int data_size, int inoeud, SGF_t *mon_SGF)
{
/* 	A FAIRE 	*/
    return 0;							/* tout va bien */
}


/********************************************************************************/
/* lire_donnees_dans_inoeud : copie dans "data" les "data_size" octets de	*/
/*  donn�es lues dans les blocs du inoeud "inoeud". Lit les donn�es bloc par	*/
/*  bloc (lit "mon_SGF->superbloc->taille_bloc" octets de donn�es � chaque fois)*/
/*  jusqu'� l'avant dernier bloc. Lit les donn�es restantes et pas plus, dans	*/
/*  le dernier bloc.								*/
/* Renvoie 0 si tout va bien ou un code d'erreur sinon.				*/
/********************************************************************************/

int lire_donnees_dans_inoeud ( char * data, int data_size, int inoeud, SGF_t *mon_SGF)
{
/* 	A FAIRE 	*/
    return 0;							/* tout va bien */
}


/********************************************************************************/
/* creer_inoeud : cr�e avec le premier num�ro de inoeud libre, un inoeud de	*/
/*  type "type_inoeud" et met � jour la table des inoeuds.			*/
/* Renvoie le num�ro du inoeud cr�� si tout va bien ou "NO_INOEUD" sinon.	*/
/********************************************************************************/

int creer_inoeud ( int type_inoeud, SGF_t *mon_SGF)
{
/* 	A FAIRE 	*/
    return NO_INOEUD;
}


/********************************************************************************/
/* liberer_inoeud : lib�re les �ventuels blocs utilis�s par le inoeud "inoeud",	*/
/*  met le type du inoeud � "INOEUD_LIBRE" et sa taille � z�ro. Puis met � jour	*/
/*  la table des inoeuds.							*/
/* Renvoie 0 si tout va bien ou un code d'erreur sinon.				*/
/********************************************************************************/

int liberer_inoeud ( int inoeud, SGF_t *mon_SGF)
{
/* 	A FAIRE 	*/
    return 0;							/* tout va bien */
}


/********************************************************************************/
/* lire_liste_rep_dans_inoeud : alloue en m�moire un tableau d'�l�ments de la	*/
/*  structure "dir_element_t", et y copie les donn�es correspondantes lues	*/
/*  dans les blocs de donn�es de l'inoeud "inoeud_rep" qui doit �tre un		*/
/*  r�pertoire.									*/
/* Renvoie un pointeur vers un tableau de "dir_element_t" contenant la liste	*/
/*  des �l�ments du r�pertoire d'inoeud "inoeud_rep".				*/
/********************************************************************************/

dir_element_t *  lire_liste_rep_dans_inoeud ( int inoeud_rep, SGF_t *mon_SGF)
{
    dir_element_t * liste_rep = NULL;
    assert( mon_SGF != NULL && mon_SGF->table_inoeuds != NULL );		/* DEBUG*/
    assert( mon_SGF->table_inoeuds[inoeud_rep].type_de_fichier == INOEUD_REPERTOIRE && mon_SGF->table_inoeuds[inoeud_rep].taille >0 );	/* DEBUG*/

    liste_rep = malloc( mon_SGF->table_inoeuds[inoeud_rep].taille);
    if (liste_rep == NULL)
	return NULL;

    if (lire_donnees_dans_inoeud ( (char *) liste_rep, mon_SGF->table_inoeuds[inoeud_rep].taille, inoeud_rep, mon_SGF) >0)
    {
	free(liste_rep);
	return NULL;
    }
    return liste_rep;
}


/********************************************************************************/
/* delier_element_du_repertoire : supprime le lien entre l'�l�ment de nom	*/
/*  "nom_elt", de inoeud "inoeud_elt", et le r�pertoire de inoeud "inoeud_rep".	*/
/*  Pour cela recopie le dernier �l�ment de la liste des �l�ments du r�pertoire	*/
/*  � l'indice de l'�l�ment � retirer. Met � jour la liste des �l�ments du	*/
/*  r�pertoire d'inoeud "inoeud_rep".						*/
/* Renvoie 0 si tout va bien ou un code d'erreur sinon.				*/
/********************************************************************************/

int delier_element_du_repertoire(char *nom_elt, int inoeud_elt, int inoeud_rep, SGF_t *mon_SGF)
{
    dir_element_t * liste_rep = NULL;
    int i, num_elt;

    assert( inoeud_elt != inoeud_rep );					/* DEBUG : l'�l�ment n'est pas le r�pertoire lui m�me */

    if ((liste_rep =lire_liste_rep_dans_inoeud ( inoeud_rep, mon_SGF)) == NULL)
	return EXIT_DEVICE;

    assert( inoeud_elt != liste_rep[1].elt_inoeud || inoeud_rep==0 );	/* DEBUG : l'�l�ment n'est pas '..' sauf pour la racine qui n'a pas de '..' */

    i=0;								/* cherche l'�l�ment dans la liste du r�pertoire */
    while( liste_rep[i].elt_inoeud != inoeud_elt && liste_rep[i].elt_inoeud != NO_INOEUD )
	i++;

    if (liste_rep[i].elt_inoeud == NO_INOEUD || strcmp(liste_rep[i].elt_name,nom_elt))
    {
	free(liste_rep);
	fprintf(stderr,"delier_element_du_repertoire: \"%s\" (inoeud %d) pas dans le r�pertoire %d !\n", nom_elt, inoeud_elt, inoeud_rep);
	return EXIT_FS_CORRUPTED;
    }
    num_elt=i;

    while( liste_rep[i].elt_inoeud != NO_INOEUD )			/* va en fin de liste */
	i++;

    if (num_elt != i-1)							/* s'il y a au moins un �l�ment apr�s celui � retirer */
    {									/*  �crase l'�l�ment dans la liste avec le dernier �l�ment de la liste */
	liste_rep[num_elt].elt_inoeud = liste_rep[i-1].elt_inoeud;
	strncpy(liste_rep[num_elt].elt_name, liste_rep[i-1].elt_name, TAILLE_NOM_FICHIER);
	liste_rep[num_elt].elt_name[TAILLE_NOM_FICHIER-1] = '\0';
    }

    liste_rep[i-1].elt_inoeud = NO_INOEUD;				/* raccourcit la liste */

    if (ecrire_donnees_dans_inoeud ( (char *)liste_rep , mon_SGF->table_inoeuds[inoeud_rep].taille - sizeof(dir_element_t), inoeud_rep, mon_SGF) != 0)
    {
	free(liste_rep);
	return (EXIT_FS_FULL);
    }
    free(liste_rep);

    return 0;								/* tout va bien */
}


/********************************************************************************/
/* lier_element_au_repertoire : cr�e un lien entre l'�l�ment de nom "nom_elt",	*/
/*  de inoeud "inoeud_elt", et le r�pertoire de inoeud "inoeud_rep".		*/
/*  Pour cela ajoute l'�l�ment � la fin de la liste des �l�ments du r�pertoire.	*/
/*  Met � jour la liste des �l�ments du r�pertoire d'inoeud "inoeud_rep".	*/
/* Renvoie 0 si tout va bien ou un code d'erreur sinon.				*/
/********************************************************************************/

int lier_element_au_repertoire(char *nom_elt, int inoeud_elt, int inoeud_rep, SGF_t *mon_SGF)
{
    dir_element_t * liste_rep = NULL;
    int i;

    assert( inoeud_elt != inoeud_rep );			/* DEBUG : l'�l�ment n'est pas le r�pertoire lui m�me */

    if ((liste_rep =lire_liste_rep_dans_inoeud ( inoeud_rep, mon_SGF)) == NULL)
	return EXIT_DEVICE;

    i=0;
    while( liste_rep[i].elt_inoeud != NO_INOEUD )	/* va en fin de liste */
    {
 	assert( inoeud_elt != liste_rep[i].elt_inoeud);	/* DEBUG : l'�l�ment n'existe pas d�j� */
	i++;
    }
							/* augmente la taille du tableau liste_rep d'un �lement */
    liste_rep = realloc( liste_rep, mon_SGF->table_inoeuds[inoeud_rep].taille + sizeof(dir_element_t));
    if (liste_rep == NULL)
	return EXIT_MEM_ALLOC;

    liste_rep[i].elt_inoeud = inoeud_elt;		/* ajoute le nouveau inoeud et le nom de l'�l�ment en fin de liste */
    strncpy(liste_rep[i].elt_name, nom_elt, TAILLE_NOM_FICHIER);
    liste_rep[i].elt_name[TAILLE_NOM_FICHIER-1] = '\0';
    liste_rep[i+1].elt_inoeud = NO_INOEUD;		/* met � jour la fin de la liste */

    if (ecrire_donnees_dans_inoeud ( (char *)liste_rep , mon_SGF->table_inoeuds[inoeud_rep].taille + sizeof(dir_element_t), inoeud_rep, mon_SGF) != 0)
    {
	free(liste_rep);
	return (EXIT_FS_FULL);
    }

    free(liste_rep);

    return 0;						/* tout va bien */
}


/********************************************************************************/
/* nom_suivant : avance dans la cha�ne de caract�res "designation" jusqu'�	*/
/*  trouver un caract�re '/' ou fin de cha�ne. Si c'est un '/', le remplace par	*/
/*  fin de cha�ne.								*/
/* !  modifie la contenu de la cha�ne de caract�res "designation" en rempla�ant	*/
/*   le premier '/' trouv� par fin de cha�ne. 					*/
/* Renvoie un pointeur vers le caract�re suivant le premier '/' trouv� ou vers	*/
/*  fin de cha�ne s'il n'en a pas trouv�.					*/
/********************************************************************************/

char * nom_suivant (char * designation)
{
    char * pt_nom_suivant = designation;

    assert( designation != NULL );		/* DEBUG*/

    while (*pt_nom_suivant!='/' && *pt_nom_suivant!='\0')
	pt_nom_suivant++;

    if (*pt_nom_suivant=='/')
    {
	*pt_nom_suivant='\0';
	pt_nom_suivant++;
    }

    return pt_nom_suivant;
}


/********************************************************************************/
/* inoeud_element_dans_repertoire : cherche dans la liste des �l�ments du	*/
/*  r�pertoire d'inoeud "inoeud_rep" un �l�ment dont le nom est "nom_element".	*/
/* Renvoie le num�ro de inoeud de l'�l�ment s'il existe ou "NO_INOEUD" sinon.	*/
/********************************************************************************/

int inoeud_element_dans_repertoire(char * nom_element, int inoeud_rep, SGF_t *mon_SGF)
{
    int i;
    int inoeud_elt;
    dir_element_t * liste_rep = NULL;

    assert( mon_SGF != NULL && mon_SGF->table_inoeuds != NULL && nom_element != NULL );		/* DEBUG*/

    if (mon_SGF->table_inoeuds[inoeud_rep].type_de_fichier != INOEUD_REPERTOIRE)
	return NO_INOEUD;

    if ((liste_rep =lire_liste_rep_dans_inoeud ( inoeud_rep, mon_SGF)) == NULL)
    {
	fprintf(stderr,"lire_donnees_dans_inoeud: %d", inoeud_rep);
	perror (DEVICE_NAME);
	exit (EXIT_DEVICE);
    }

    i=0;					/* cherche l'�l�ment dans la liste des �l�ments du r�pertoire */
    while( liste_rep[i].elt_inoeud != NO_INOEUD && strcmp(liste_rep[i].elt_name,nom_element) != 0 )
	i++;

    inoeud_elt = liste_rep[i].elt_inoeud;
    free(liste_rep);
    return inoeud_elt;
}


/********************************************************************************/
/* inoeud_designation_element : parcours tous les noms du chemin "designation"	*/
/*  s�par�s par le caract�re '/', et pour chaque nom cherche le inoeud de cet	*/
/*  �l�ment dans le r�pertoire de inoeud "inoeud_rep_sup" pour l'utiliser comme	*/
/*  "inoeud_rep_sup" dans la recherche suivante. La recherche se fait en	*/
/*  partant du r�pertoire de inoeud "inoeud_org".				*/
/* Renvoie le num�ro de inoeud du dernier nom du chemin "designation" s'il	*/
/*  existe ou "NO_INOEUD" sinon.						*/
/********************************************************************************/

int inoeud_designation_element(char * designation, int inoeud_org, SGF_t *mon_SGF)
{
    int inoeud_rep_sup;
    char * pt_designation;
    char * pt_nom_suivant;

    assert( mon_SGF != NULL && mon_SGF->table_inoeuds != NULL && designation != NULL );		/* DEBUG*/

    pt_designation=malloc(strlen(designation)+1);	/* copie le chemin pour modifier la chaine */
    strcpy(pt_designation, designation);

    if (pt_designation[0] == '/')			/* si le chemin commence par la racine */
    {
	pt_designation++;
	inoeud_rep_sup=0;				/* le premier inoeud est la racine */
    }
    else
	inoeud_rep_sup=inoeud_org;			/* sinon c'est celui pass� en param�tre */


    while (pt_designation[0] != '\0')			/* parcours toute la cha�ne de caract�res contenant le chemin */
    {
	pt_nom_suivant=nom_suivant(pt_designation);	/* nom suivant dans la cha�ne de caract�res point�e par pt_designation */
	inoeud_rep_sup=inoeud_element_dans_repertoire(pt_designation, inoeud_rep_sup, mon_SGF);
	if (inoeud_rep_sup == NO_INOEUD)
	    return NO_INOEUD;

	pt_designation=pt_nom_suivant;			/* avance dans la cha�ne de caract�res contenant le chemin */
    }
    return inoeud_rep_sup;
}
