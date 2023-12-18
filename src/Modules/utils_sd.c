/// \file utils_sd.c
/// \brief Fichier des allocations dynamiques des tableaux dans le programme.
/// \author Equipe-2 : Salim Moussaoui, Maxime Cherraf, Quentin Kouzmitch, Nicolas Giry
/// \date 11 decembre 2023

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
//#include "structure_graphe.h"
#include "utils_sd.h"

/************************************----------CANDIDATS----------*****************************************/

Candidat candidats[MAX_CANDIDATS] = {
    {0, "BBP"}, {1, "BSO"}, {2, "Thai"}, {3, "Veggie"}, {4, "FireC"},
    {5, "Roma"}, {6, "Crispy"}, {7, "Cheese"}, {8, "BS"}, {9, "Country"},
};

/// @fn const char* obtenirNomCandidat(int indice)
/// @brief Obtient le nom du candidat à partir de son indice.
/// @param[in] indice Indice du candidat.
/// @return Nom du candidat correspondant.
const char* obtenirNomCandidat(int indice) {
    for (int i = 0; i < MAX_CANDIDATS; ++i) {
        if (candidats[i].indice == indice) {
            return candidats[i].nom;
        }
    }
    return NULL; // Indice non trouvé
}


/************************************----------TABLEAU DES DONNEES DE VOTE----------*****************************************/

/// @fn void initMatrice(t_mat_char_star_dyn *matrice, int ligne, int colonne)
/// @brief Initialise une matrice dynamique.
/// @param[in] matrice Pointeur vers la matrice à initialiser.
/// @param[in] ligne Nombre de lignes de la matrice.
/// @param[in] colonne Nombre de colonnes de la matrice.
void initMatrice(t_mat_char_star_dyn *matrice, int ligne, int colonne) {
    matrice->ligne = ligne;
    matrice->colonne = colonne;

    // Allouer mémoire pour les pointeurs de lignes
    matrice->vote = (char ***)malloc(ligne * sizeof(char **));

    // Allouer mémoire pour chaque ligne
    for (int i = 0; i < ligne; i++)
        matrice->vote[i] = (char **)malloc(colonne * sizeof(char *));
}


/// @fn void freeMatrice(t_mat_char_star_dyn *matrice)
/// @brief Libère la mémoire allouée pour une matrice dynamique.
/// @param[in] matrice Pointeur vers la matrice à libérer.
void freeMatrice(t_mat_char_star_dyn *matrice) {
    // Libérer la mémoire pour chaque ligne
    for (int i = 0; i < matrice->ligne; ++i) {
        for (int j = 0; j < matrice->colonne; ++j) {
            free(matrice->vote[i][j]);
        }
        free(matrice->vote[i]);
    }
    // Libérer la mémoire pour les pointeurs de lignes
    free(matrice->vote);
}


/************************************----------MATRICE DES DUELS----------*****************************************/

/// @fn MatriceDuel creerMatriceDuel(int lignes, int colonnes)
/// @brief Crée une matrice de duels pour les candidats.
/// @param[in] lignes Nombre de lignes de la matrice.
/// @param[in] colonnes Nombre de colonnes de la matrice.
/// @return La matrice de duels créée.
MatriceDuel creerMatriceDuel(int lignes, int colonnes) {
    MatriceDuel matrice;
    matrice.lignes = lignes;
    matrice.colonnes = colonnes;

    matrice.donnees = (int **)malloc(lignes * sizeof(int *));
    if (matrice.donnees == NULL) {
        fprintf(stderr, "Erreur d'allocation de mémoire pour les lignes\n");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < lignes; ++i) {
        matrice.donnees[i] = (int *)malloc(colonnes * sizeof(int));
        if (matrice.donnees[i] == NULL) {
            fprintf(stderr, "Erreur d'allocation de mémoire pour les colonnes\n");
            exit(EXIT_FAILURE);
        }
    }

    for (int i = 0; i < lignes; ++i) {
        for (int j = 0; j < colonnes; ++j) {
            matrice.donnees[i][j] = 0;
        }
    }

    return matrice;
}

/// @fn void libererMatriceDuel(MatriceDuel *matrice)
/// @brief Libère la mémoire allouée pour une matrice de duels.
/// @param[in] matrice Pointeur vers la matrice de duels à libérer.
void libererMatriceDuel(MatriceDuel *matrice) {
    for (int i = 0; i < matrice->lignes; ++i) {
        free(matrice->donnees[i]);
    }
    free(matrice->donnees);
}

/// @fn void remplirMatriceDuel(const t_mat_char_star_dyn *matriceVote, MatriceDuel *matriceDuels)
/// @brief Remplit une matrice de duels à partir des votes.
/// @param[in] matriceVote Matrice contenant les votes.
/// @param[in] matriceDuels Matrice de duels à remplir.
void remplirMatriceDuel(const t_mat_char_star_dyn *matriceVote, MatriceDuel *matriceDuels) {
    // Tableau pour suivre les paires déjà comparées
    int dejaCompare[MAX_CANDIDATS][MAX_CANDIDATS] = {0};

    for (int i = 1; i < matriceVote->ligne; ++i) {
        // Réinitialiser le tableau dejaCompare à chaque nouveau vote
        memset(dejaCompare, 0, sizeof(dejaCompare));

        for (int j = 4; j <= 13; ++j) {
            for (int k = j + 1; k <= 13; ++k) {
                int classement_burger1 = atoi(matriceVote->vote[i][j]);
                int classement_burger2 = atoi(matriceVote->vote[i][k]);

                if (classement_burger1 != -1 && classement_burger2 != -1 &&
                    !dejaCompare[j - 4][k - 4] && !dejaCompare[k - 4][j - 4]) {

                    // Les deux burgers sont préférés (classement plus petit) ou non préférés (classement égal)
                    if (classement_burger1 < classement_burger2) {
                        matriceDuels->donnees[j - 4][k - 4]++;
                    }
                    else if (classement_burger1 > classement_burger2) {
                        matriceDuels->donnees[k - 4][j - 4]++;
                    }

                    // Marquer cette paire comme déjà comparée
                    dejaCompare[j - 4][k - 4] = 1;
                    dejaCompare[k - 4][j - 4] = 1;
                }
            }
        }
    }
}

/// @fn void afficherMatriceDuelAvecNoms(const MatriceDuel *matrice, int nbCandidats)
/// @brief Affiche une matrice de duels avec les noms des candidats.
/// @param[in] matrice Matrice de duels à afficher.
/// @param[in] nbCandidats Nombre de candidats.
/// @param[in] log fichier log
void afficherMatriceDuelAvecNoms(const MatriceDuel *matrice, int nbCandidats,const char * log) {
    fprintf(log,"MATRICE DUELS : \n\n");
    // Trouver la longueur maximale des noms
    int longueurMaxNom = 0;
    for (int i = 0; i < nbCandidats; ++i) {
        int longueurNom = strlen(candidats[i].nom);
        if (longueurNom > longueurMaxNom) {
            longueurMaxNom = longueurNom;
        }
    }

    // Afficher les noms des candidats en haut de la matrice
    fprintf(log,"%-*s |", longueurMaxNom, "");

    for (int i = 0; i < nbCandidats; ++i) {
        fprintf(log," %-*s |", longueurMaxNom, candidats[i].nom);
    }
    fprintf(log,"\n");

    // Afficher une ligne de séparation
    for (int i = 0; i < nbCandidats + 1; ++i) {
        for (int j = 0; j < longueurMaxNom + 3; ++j) {
            fprintf(log,"-");
        }
    }
    fprintf(log,"\n");

    // Afficher la matrice avec les noms des candidats sur le côté
    for (int i = 0; i < matrice->lignes; ++i) {
        fprintf(log,"%-*s |", longueurMaxNom, candidats[i].nom);

        for (int j = 0; j < matrice->colonnes; ++j) {
            fprintf(log," %-*d |", longueurMaxNom, matrice->donnees[i][j]);
        }
        fprintf(log,"\n");

        // Afficher une ligne de séparation après chaque ligne de données
        for (int k = 0; k < nbCandidats + 1; ++k) {
            for (int l = 0; l < longueurMaxNom + 3; ++l) {
                fprintf(log,"-");
            }
        }
        fprintf(log,"\n");
    }
}


/************************************----------GRAPHE----------*****************************************/

/// @fn ListeArcs creerListeArcs()
/// @brief Crée une liste d'arcs pour un graphe.
/// @return Une liste d'arcs vide.
ListeArcs creerListeArcs() {
    ListeArcs liste;
    liste.debut = NULL;
    liste.taille = 0;
    return liste;
}

/// @fn ListeDeListeArcs creerListeDeListeArcs()
/// @brief Crée une liste de listes d'arcs pour un graphe.
/// @return Une liste de listes d'arcs vide.
ListeDeListeArcs creerListeDeListeArcs() {
    ListeDeListeArcs liste;
    liste.debut = NULL;
    liste.courant = NULL;
    liste.taille = 0;
    return liste;
}

/// @fn bool estDansGraphe(ListeArcs* liste, int sommet)
/// @brief Vérifie si un sommet est présent dans un graphe.
/// @param[in] liste Pointeur vers la liste des arcs représentant le graphe.
/// @param[in] sommet L'indice du sommet à rechercher dans le graphe.
/// @return Vrai si le sommet est présent, faux sinon.
bool estDansGraphe(ListeArcs* liste, int sommet) {
    Arc* e = liste->debut;
    while(e != NULL) {
        if (e->sommetDepart == sommet)
            return true;
        e = e->suivant;
    }
    return false;
}

/// @fn void ajouterListeArc(ListeDeListeArcs* listeDeListe, ListeArcs* liste)
/// @brief Ajoute une liste d'arcs à une liste de listes d'arcs.
/// @param[in] listeDeListe Pointeur vers la liste de listes d'arcs.
/// @param[in] liste Pointeur vers la liste d'arcs à ajouter.
void ajouterListeArc (ListeDeListeArcs* listeDeListe, ListeArcs* liste) {
    if (listeDeListe->debut == NULL) {
        listeDeListe->debut = liste;
        listeDeListe->courant = liste;
    } else {
        listeDeListe->courant->suivant = liste;
        listeDeListe->courant = listeDeListe->courant->suivant;
        listeDeListe->courant->suivant = NULL;
    }
    listeDeListe->taille ++;
}

/// @fn void ajouterArc(ListeArcs *liste, const Arc *arc)
/// @brief Ajoute un arc à une liste d'arcs.
/// @param[in] liste Pointeur vers la liste d'arcs.
/// @param[in] arc Pointeur vers l'arc à ajouter.
void ajouterArc(ListeArcs *liste, const Arc *arc){
    Arc* copie = malloc(sizeof(Arc));
    copie->sommetDepart = arc->sommetDepart;
    copie->sommetArrivee = arc->sommetArrivee;
    copie->poids = arc->poids;
    copie->suivant = NULL;
    (liste->taille)++;
    if (liste->debut == NULL) {
        liste->debut = copie;
    } else {
        Arc *p = liste->debut;
        while (p->suivant != NULL){
            p = p->suivant;
        }
        p->suivant = copie;
    }
}

/// @fn void retirerArc(ListeArcs* liste, Arc *arc)
/// @brief Retire un arc spécifique d'une liste d'arcs.
/// @param[in] liste Pointeur vers la liste d'arcs.
/// @param[in] arc Pointeur vers l'arc à retirer.
void retirerArc(ListeArcs* liste, Arc *arc) {
    if (liste == NULL || liste->debut == NULL) {
        //printf("La liste est vide ou n'existe pas.\n");
        return;
    }

    Arc *p = liste->debut;
    Arc *prev = NULL;

    if ((p->sommetArrivee == arc->sommetArrivee)&& (p->sommetDepart == arc->sommetDepart)) { // Vérifier si l'arc à retirer est en tête de liste
        liste->debut = p->suivant;
        free(p);
        (liste->taille)--;
        return;
    }
    while ((p != NULL) && ((p->sommetArrivee != arc->sommetArrivee)|| (p->sommetDepart != arc->sommetDepart))) {
        prev = p;
        p = p->suivant;
    }

    if (p == NULL) {
        //printf("L'arc à enlever n'est pas dans la liste\n");
        return;
    }
    prev->suivant = p->suivant;
    free(p);
    (liste->taille)--;
}

/// @fn void ajouterArcSommet(ListeArcs* graphe, ListeArcs* arcsAVerifier, int sommet)
/// @brief Ajoute à une liste d'arcs tous les arcs d'un graphe qui partent d'un sommet spécifique.
/// @param[in] graphe Pointeur vers le graphe source.
/// @param[in] arcsAVerifier Pointeur vers la liste d'arcs où ajouter les nouveaux arcs.
/// @param[in] sommet Indice du sommet de départ des arcs à ajouter.
void ajouterArcSommet(ListeArcs* graphe, ListeArcs* arcsAVerifier, int sommet) {
    Arc* e = graphe->debut;
    while(e != NULL) {
        if (e->sommetDepart == sommet) {
            ajouterArc(arcsAVerifier, e);
        }
        e = e->suivant;
    }
}

/// @fn void ajouterArcDepuisMatrice(ListeArcs *liste, int sommetDepart, int sommetArrivee, MatriceDuel *matriceDuels)
/// @brief Crée et ajoute un nouvel arc à une liste d'arcs à partir des données d'une matrice de duels.
/// @param[in] liste Pointeur vers la liste d'arcs.
/// @param[in] sommetDepart Indice du sommet de départ de l'arc.
/// @param[in] sommetArrivee Indice du sommet d'arrivée de l'arc.
/// @param[in] matriceDuels Pointeur vers la matrice de duels contenant les poids des arcs.
void ajouterArcDepuisMatrice(ListeArcs *liste, int sommetDepart, int sommetArrivee, MatriceDuel * matriceDuels) {
    Arc *nouvelArc = (Arc *)malloc(sizeof(Arc));
    if (nouvelArc == NULL) {
        fprintf(stderr, "Erreur d'allocation de mémoire pour un arc\n");
        exit(EXIT_FAILURE);
    }

    nouvelArc->sommetDepart = sommetDepart;
    nouvelArc->sommetArrivee = sommetArrivee;
    nouvelArc->poids = matriceDuels->donnees[sommetDepart][sommetArrivee] - matriceDuels->donnees[sommetArrivee][sommetDepart];
    nouvelArc->suivant = liste->debut;

    liste->debut = nouvelArc;
    (liste->taille)++;
}

/// @fn void creerListeArcsDepuisMatrice(MatriceDuel matriceDuels, int nbCandidats, ListeArcs *liste)
/// @brief Crée une liste d'arcs à partir d'une matrice de duels.
/// @param[in] matriceDuels Matrice contenant les scores des duels entre candidats.
/// @param[in] nbCandidats Nombre de candidats.
/// @param[in] liste Liste d'arcs à remplir.
void creerListeArcsDepuisMatrice(MatriceDuel matriceDuels, int nbCandidats, ListeArcs * liste) {
    //ListeArcs liste = creerListeArcs();

    int dejaVu[MAX_CANDIDATS][MAX_CANDIDATS]={0};

    for (int i = 0; i < nbCandidats; ++i) {
        for (int j = 0; j < nbCandidats; ++j) {
            if(!dejaVu[i][j] && !dejaVu[j][i]){
                if (matriceDuels.donnees[i][j] > matriceDuels.donnees[j][i] && i!=j)
                    ajouterArcDepuisMatrice(liste, i, j, &matriceDuels);



                else if (matriceDuels.donnees[i][j] < matriceDuels.donnees[j][i] && i!=j)
                    ajouterArcDepuisMatrice(liste, j, i, &matriceDuels);

                dejaVu[i][j]=1;
                dejaVu[j][i]=1;
            }
        }
    }
}

/// @fn void libererListeArcs(ListeArcs *liste)
/// @brief Libère la mémoire allouée pour une liste d'arcs.
/// @param[in] liste Liste d'arcs à libérer.
void libererListeArcs(ListeArcs *liste) {
    Arc *courant = liste->debut;
    while (courant != NULL) {
        Arc *suivant = courant->suivant;
        free(courant);
        courant = suivant;
    }
    liste->debut = NULL;
    liste->taille = 0;
}

/// @fn void afficherListeArcs(const ListeArcs *liste)
/// @brief Affiche les arcs d'une liste.
/// @param[in] liste Liste d'arcs à afficher.
/// @param[in] log fichier log 
void afficherListeArcs(const ListeArcs *liste,const char * log) {
    Arc *courant = liste->debut;
    int i=0;
    fprintf(log,"LISTE DES ARCS:\n\n");
    while (courant != NULL) {
        //printf("(%s -> %s, poids = %d)\n", candidats[courant->sommetDepart].nom, candidats[courant->sommetArrivee].nom, courant->poids);
        fprintf(log,"%d: (%d -> %d), poids = %d\n", i, courant->sommetDepart, courant->sommetArrivee, courant->poids);
        fflush(stdout);
        courant = courant->suivant;
        i++;
    }
}


/// @fn void afficherArc(const Arc *arc)
/// @brief Affiche un arc.
/// @param[in] arc Arc à afficher.
void afficherArc(const Arc *arc) {
    printf("(%d -> %d), poids = %d\n", arc->sommetDepart, arc->sommetArrivee, arc->poids);
    fflush(stdout);
}

/// @fn void afficherListeDeListeArcs(ListeDeListeArcs *liste)
/// @brief Affiche les listes d'arcs d'une liste de listes d'arcs.
/// @param[in] liste Liste de listes d'arcs à afficher.
/// @param[in] log fichier log 
void afficherListeDeListeArcs (ListeDeListeArcs * liste, const char * log) {
    ListeArcs * e = liste->debut;
    fprintf(log,"Affichage:\n");
    while (e!=NULL) {
        afficherListeArcs(e,log);
        printf(log,"\tPoids: %d\n\n",e->poids);
        e = e->suivant;
    }
    fflush(stdout);
}

/// @fn int weight_at(ListeDeListeArcs *liste, int ind)
/// @brief Retourne le poids à un indice spécifique dans une liste de listes d'arcs.
/// @param[in] liste Liste de listes d'arcs.
/// @param[in] ind Indice à consulter.
/// @return Poids à l'indice spécifié, ou -1 si l'indice est trop grand.
int weight_at(ListeDeListeArcs* liste, int ind) {
    if (ind<liste->taille) {
        ListeArcs * courant = liste->debut;
        for (int i=0; i<ind; i++) {
            courant=courant->suivant;
        }
        return courant->poids;
    }
    printf("indice trop grand\n");
    return -1;
}

/// @fn void afficher_score_jugement(const Candidat_mention *candidats, int nb_candidats)
/// @brief Affiche les scores de jugement pour chaque candidat.
/// @param[in] candidats Tableau de candidats avec leurs scores.
/// @param[in] nb_candidats Nombre de candidats dans le tableau.
/// @param[in] log fichier log

void afficher_score_jugement(const Candidat_mention *candidats, int nb_candidats,const char * log) {
    fprintf(log,"SCORE JUGEMENT MAJORITAIRE POUR CHAQUE CANDIDAT\n\n");
    for (int i = 0; i < nb_candidats; i++) {
        fprintf(log,"Nom: %s\n", candidats[i].nom);
        fprintf(log,"Scores: ");
        for (int j = 0; j < 6; j++) {
            fprintf(log,"%d ", candidats[i].score[j]);
        }
        fprintf(log,"\n\n");
    }
}
