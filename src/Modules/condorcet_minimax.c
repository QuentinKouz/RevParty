/// \file condorcet_minimax.c
/// \brief Programme d'implémentation calculant le vainqueur de condorcet minimax
/// \author Equipe-2 : Salim Moussaoui, Maxime Cherraf, Quentin Kouzmitch, Nicolas Giry
/// \date 15 decembre 2023

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "utils_sd.h"

// Fonction pour trouver le vainqueur selon la méthode Minimax
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

void minimax(ListeArcs *graphe, int nbCandidats) {
    int vainqueur = trouverVainqueurMinimax(graphe, nbCandidats);
    if (vainqueur == -1) {
        printf("ERREUR: Pas de vainqueur de minimax\n");
        exit(10);
    }
    printf("Le candidat n°%d est le vainqueur de minimax !\n", vainqueur);
}


/*#######################TEST#############################*/
// Fonction pour ajouter un arc de test au graphe
void ajouterArcTest(ListeArcs *graphe, int depart, int arrivee, int poids) {
    Arc *nouvelArc = (Arc *)malloc(sizeof(Arc));
    nouvelArc->sommetDepart = depart;
    nouvelArc->sommetArrivee = arrivee;
    nouvelArc->poids = poids;
    nouvelArc->suivant = graphe->debut;
    graphe->debut = nouvelArc;
}


/*int main(void) {
    ListeArcs graphe = creerListeArcs();

    // Ajouter des arcs de test
    ajouterArcTest(&graphe, 0, 1, 10);  // Candidat 0 bat Candidat 1 avec un score de 10
    ajouterArcTest(&graphe, 1, 2, 20);  // Candidat 1 bat Candidat 2 avec un score de 20
    ajouterArcTest(&graphe, 2, 0, 5);   // Candidat 2 bat Candidat 0 avec un score de 5
    
    /*
    Donc     
    Candidat 0 :
        Bat Candidat 1 avec un score de 10.
        Perd contre Candidat 2 avec un score de 5.
        Le pire score est 5 (contre Candidat 2).

    Candidat 1 :
        Bat Candidat 2 avec un score de 20.
        Perd contre Candidat 0 avec un score de 10.
        Le pire score est 10 (contre Candidat 0).

    Candidat 2 :
        Bat Candidat 0 avec un score de 5.
        Perd contre Candidat 1 avec un score de 20.
        Le pire score est 20 (contre Candidat 1).
        
    Dans la méthode Minimax, le vainqueur est celui avec le plus petit "pire score". Dans ce cas, c'est le Candidat 0 avec un score de 5.
    
    minimax(&graphe, 3);  // On suppose qu'il y a 3 candidats

    libererListeArcs(&graphe);

    return 0;
}*/
