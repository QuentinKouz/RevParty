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

// Fonction pour obtenir le nom du candidat à partir de son indice
const char* obtenirNomCandidat(int indice) {
    for (int i = 0; i < MAX_CANDIDATS; ++i) {
        if (candidats[i].indice == indice) {
            return candidats[i].nom;
        }
    }
    return NULL; // Indice non trouvé
}


/************************************----------TABLEAU DES DONNEES DE VOTE----------*****************************************/
// Initialisation matrice
void initMatrice(t_mat_char_star_dyn *matrice, int ligne, int colonne) {
    matrice->ligne = ligne;
    matrice->colonne = colonne;

    // Allouer mémoire pour les pointeurs de lignes
    matrice->vote = (char ***)malloc(ligne * sizeof(char **));

    // Allouer mémoire pour chaque ligne
    for (int i = 0; i < ligne; i++)
        matrice->vote[i] = (char **)malloc(colonne * sizeof(char *));
}


// Libérer la mémoire allouée pour la matrice
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


void libererMatriceDuel(MatriceDuel *matrice) {
    for (int i = 0; i < matrice->lignes; ++i) {
        free(matrice->donnees[i]);
    }
    free(matrice->donnees);
}


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




void afficherMatriceDuelAvecNoms(const MatriceDuel *matrice, int nbCandidats) {
    // Trouver la longueur maximale des noms
    int longueurMaxNom = 0;
    for (int i = 0; i < nbCandidats; ++i) {
        int longueurNom = strlen(candidats[i].nom);
        if (longueurNom > longueurMaxNom) {
            longueurMaxNom = longueurNom;
        }
    }

    // Afficher les noms des candidats en haut de la matrice
    printf("%-*s |", longueurMaxNom, "");

    for (int i = 0; i < nbCandidats; ++i) {
        printf(" %-*s |", longueurMaxNom, candidats[i].nom);
    }
    printf("\n");

    // Afficher une ligne de séparation
    for (int i = 0; i < nbCandidats + 1; ++i) {
        for (int j = 0; j < longueurMaxNom + 3; ++j) {
            printf("-");
        }
    }
    printf("\n");

    // Afficher la matrice avec les noms des candidats sur le côté
    for (int i = 0; i < matrice->lignes; ++i) {
        printf("%-*s |", longueurMaxNom, candidats[i].nom);

        for (int j = 0; j < matrice->colonnes; ++j) {
            printf(" %-*d |", longueurMaxNom, matrice->donnees[i][j]);
        }
        printf("\n");

        // Afficher une ligne de séparation après chaque ligne de données
        for (int k = 0; k < nbCandidats + 1; ++k) {
            for (int l = 0; l < longueurMaxNom + 3; ++l) {
                printf("-");
            }
        }
        printf("\n");
    }
}


/************************************----------GRAPHE----------*****************************************/

ListeArcs creerListeArcs() {
    ListeArcs liste;
    liste.debut = NULL;
    liste.taille = 0;
    return liste;
}

ListeDeListeArcs creerListeDeListeArcs() {
    ListeDeListeArcs liste;
    liste.debut = NULL;
    liste.courant = NULL;
    liste.taille = 0;
    return liste;
}

bool estDansGraphe(ListeArcs* liste, int sommet) {
    Arc* e = liste->debut;
    while(e != NULL) {
        if (e->sommetDepart == sommet)
            return true;
        e = e->suivant;
    }
    return false;
}

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

void ajouterArcSommet(ListeArcs* graphe, ListeArcs* arcsAVerifier, int sommet) {
    Arc* e = graphe->debut;
    while(e != NULL) {
        if (e->sommetDepart == sommet) {
            ajouterArc(arcsAVerifier, e);
        }
        e = e->suivant;
    }
}

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

void afficherListeArcs(const ListeArcs *liste) {
    Arc *courant = liste->debut;
    int i=0;
    while (courant != NULL) {
        //printf("(%s -> %s, poids = %d)\n", candidats[courant->sommetDepart].nom, candidats[courant->sommetArrivee].nom, courant->poids);
        printf("%d: (%d -> %d), poids = %d\n", i, courant->sommetDepart, courant->sommetArrivee, courant->poids);
        fflush(stdout);
        courant = courant->suivant;
        i++;
    }
}

void afficherArc(const Arc *arc) {
    printf("(%d -> %d), poids = %d\n", arc->sommetDepart, arc->sommetArrivee, arc->poids);
    fflush(stdout);
}

void afficherListeDeListeArcs (ListeDeListeArcs * liste) {
    ListeArcs * e = liste->debut;
    printf("Affichage:\n");
    while (e!=NULL) {
        afficherListeArcs(e);
        printf("\tPoids: %d\n\n",e->poids);
        e = e->suivant;
    }
    fflush(stdout);
}

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

void afficher_score_jugement(const Candidat_mention *candidats, int nb_candidats) {
    for (int i = 0; i < nb_candidats; i++) {
        printf("Nom: %s\n", candidats[i].nom);
        printf("Scores: ");
        for (int j = 0; j < 6; j++) {
            printf("%d ", candidats[i].score[j]);
        }
        printf("\n\n");
    }
}
