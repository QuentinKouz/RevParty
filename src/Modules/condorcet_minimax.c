/// \file condorcet_minimax.c
/// \brief Programme d'implémentation calculant le vainqueur de condorcet minimax
/// \author Equipe-2 : Salim Moussaoui, Maxime Cherraf, Quentin Kouzmitch, Nicolas Giry
/// \date 15 decembre 2023

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "utils_sd.h"

/// @fn int trouverVainqueurMinimax(ListeArcs *graphe, int nbCandidats)
/// @brief Fonction pour trouver le vainqueur selon la méthode Minimax
/// @param[in] graphe Pointeur vers la liste des arcs du graphe.
/// @param[in] nbCandidats Nombre de candidats.
/// @return Index du vainqueur.
int trouverVainqueurMinimax(ListeArcs *graphe, int nbCandidats) {
    int tab_pire_score[nbCandidats];
    for (int i = 0; i < nbCandidats; i++) {
        tab_pire_score[i] = -10000;  // Initialiser avec une valeur minimale possible
        Arc *arc = graphe->debut;
        while(arc != NULL) {
            if (arc->sommetDepart == i || arc->sommetArrivee == i) {
                int score = arc->poids;
                if (score > tab_pire_score[i]) {
                    tab_pire_score[i] = score;  // Chercher le pire score (le plus élevé)
                }
            }
            arc = arc->suivant;
        }
    }
    int vainqueur = -1, meilleur_pire_score = 10000;
    for (int i = 0; i < nbCandidats; i++) {
        if (tab_pire_score[i] < meilleur_pire_score) {
            meilleur_pire_score = tab_pire_score[i];
            vainqueur = i;
        }
    }
    return vainqueur;
}

/// @fn void minimax(ListeArcs *graphe, char *candidats[])
/// @brief Calcule le vainqueur de Condorcet Minimax et l'affiche.
/// @param[in] graphe Pointeur vers la liste des arcs du graphe.
/// @param[in] candidats Tableau des noms des candidats.
void minimax(ListeArcs *graphe, char * candidats[]) {
    int vainqueur = trouverVainqueurMinimax(graphe, MAX_CANDIDATS);
    if (vainqueur == -1) {
        printf("ERREUR: Pas de vainqueur de minimax\n");
        exit(5);
    }
    printf("Résolution du paradoxe par la méthode de Minimax\n\tLe candidat %s est le vainqueur de Condorcet minimax !\n", candidats[vainqueur]);
}







