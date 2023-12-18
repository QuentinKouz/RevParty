/// \file condorcet_schulzes.c
/// \brief Programme d'implémentation calculant le vainqueur de condorcet schulzes
/// \author Equipe-2 : Salim Moussaoui, Maxime Cherraf, Quentin Kouzmitch, Nicolas Giry
/// \date 15 decembre 2023

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utils_sd.h"
#include "condorcet.h"
#include <stdbool.h>

// schulzes **************************************

/// @fn bool equals(Arc *a1, Arc *a2)
/// @brief Compare deux arcs pour vérifier s'ils sont égaux.
/// @param[in] a1 Pointeur vers le premier arc.
/// @param[in] a2 Pointeur vers le second arc.
/// @return Vrai si les arcs sont égaux, faux sinon.
bool equals(Arc * a1, Arc * a2) {
    return (a1->sommetArrivee) == (a2->sommetArrivee) && (a1->sommetDepart) == (a2->sommetDepart);
}

/// @fn bool estDedans(Arc *arc, ListeArcs *listeArcs)
/// @brief Vérifie si un arc est présent dans une liste d'arcs.
/// @param[in] arc Pointeur vers l'arc à vérifier.
/// @param[in] listeArcs Pointeur vers la liste d'arcs.
/// @return Vrai si l'arc est dans la liste, faux sinon.
bool estDedans(Arc * arc, ListeArcs * listeArcs) {
    Arc * e = listeArcs->debut;
    while(e!=NULL) {
        if(equals(e, arc)) {
            return true;
        }
        e = e->suivant;
    }
    return false;
}

/// @fn int min(ListeArcs *listeArcs)
/// @brief Trouve le poids minimum parmi les arcs d'une liste.
/// @param[in] listeArcs Pointeur vers la liste d'arcs.
/// @return Poids minimum trouvé dans la liste.
int min(ListeArcs * listeArcs) {
    Arc * e = listeArcs->debut;
    int minimum = e->poids;
    while (e!=NULL) {
        if (e->poids < minimum){
            minimum = e->poids;
        }
        e = e->suivant;
    }
    return minimum;
}

/// @fn bool estDansListeSommet(int sommet, int *listeSommets, int taille)
/// @brief Vérifie si un sommet est présent dans une liste de sommets.
/// @param[in] sommet Sommet à vérifier.
/// @param[in] listeSommets Tableau des sommets.
/// @param[in] taille Taille du tableau des sommets.
/// @return Vrai si le sommet est dans la liste, faux sinon.
bool estDansListeSommet(int sommet, int * listeSommets, int taille) {
    for (int i=0; i<taille; i++) {
        if (listeSommets[i] == sommet) {
            return true;
        }
    }
    return false;
}

/// @fn ListeArcs *trouverCheminsAetB(ListeArcs *graphe, int A, int B, int *vus, int nbVus, ListeArcs *chemin, int *poids)
/// @brief Trouve les chemins entre deux sommets A et B dans un graphe.
/// @param[in] graphe Pointeur vers la liste des arcs du graphe.
/// @param[in] A Sommet de départ.
/// @param[in] B Sommet d'arrivée.
/// @param[in] vus Tableau des sommets déjà visités.
/// @param[in] nbVus Nombre de sommets déjà visités.
/// @param[in] chemin Pointeur vers la liste des arcs représentant le chemin actuel.
/// @param[in] poids Pointeur vers le poids du chemin.
/// @return Pointeur vers la liste des arcs représentant le chemin trouvé.
ListeArcs * trouverCheminsAetB(ListeArcs * graphe, int A, int B, int * vus, int nbVus, ListeArcs * chemin, int * poids) {
    ListeArcs tmp = creerListeArcs();
    tmp.debut = chemin->debut;
    
    if (A==B) {
        chemin->poids = min(chemin);
        if (chemin->poids > *poids){
            *poids = chemin->poids;
        }
        return chemin;
    } 

    Arc * courant = graphe->debut;
    while (courant != NULL) {
        if (courant->sommetDepart == A) {
            if (!estDansListeSommet(courant->sommetArrivee, vus, nbVus)) {
                chemin->debut = tmp.debut;
                vus[nbVus] = A;
                nbVus ++;
                // faire une copie de l'arc courant
                Arc* arc = malloc(sizeof(arc));
                memcpy(arc, courant, sizeof(Arc));
                arc->suivant = NULL;
                ajouterArc(chemin, arc);
                trouverCheminsAetB(graphe, courant->sommetArrivee, B, vus, nbVus, chemin, poids);
                chemin->debut = tmp.debut;
                retirerArc(chemin, arc);
            }
        }
        courant = courant->suivant;
    }
    return NULL;
}

/// @fn int trouverCheminFortAetB(ListeArcs *graphe, int A, int B)
/// @brief Trouve le chemin le plus fort entre deux sommets A et B.
/// @param[in] graphe Pointeur vers la liste des arcs du graphe.
/// @param[in] A Sommet de départ.
/// @param[in] B Sommet d'arrivée.
/// @return Poids du chemin le plus fort.
int trouverCheminFortAetB(ListeArcs *graphe, int A, int B) {
    int vus[100];
    int nbVus = 0;
    ListeArcs tmp = creerListeArcs();
    int poids = 0;
    trouverCheminsAetB(graphe, A, B, vus, nbVus, &tmp, &poids);
    return poids;
}

/// @fn void trouverCheminsForts(ListeArcs *graphe, int nbCandidats, int *score)
/// @brief Trouve les chemins les plus forts pour chaque paire de candidats dans un graphe.
/// @param[in] graphe Pointeur vers la liste des arcs du graphe.
/// @param[in] nbCandidats Nombre total de candidats.
void trouverCheminsForts(ListeArcs * graphe, int nbCandidats, int * score) {
    int i_score = 0;
    for (int i=0; i<nbCandidats; i++) {
        for (int j=0; j<nbCandidats; j++) {
            if (i!=j) {
                score[i_score] = trouverCheminFortAetB(graphe, i, j);
                i_score++;
            }
        }
    }
}

/// @fn int trouverVainqueurSchulzes(int *score, int nbCandidats)
/// @brief Détermine le vainqueur selon la méthode de Schulzes.
/// @param[in] score Tableau des scores des chemins les plus forts.
/// @param[in] nbCandidats Nombre total de candidats.
/// @return Index du vainqueur, -1 s'il n'y en a pas.
int trouverVainqueurSchulzes(int * score, int nbCandidats) {
    // listeCheminsForts est rangé dans l'ordre (ex: 0>1, 0>2, 1>0, 1>2, 2>0, 2>1) -> nous profitons de cette propriété pour faciliter le calcul du vainqueur
    int tab_vainqueur[nbCandidats];
    for (int i=0; i<nbCandidats; i++) {
        tab_vainqueur[i] = 0;
        for (int j=0; j<nbCandidats-1; j++) {
            int decalage;
            if (j<i) {
                decalage = j*(nbCandidats-1) + (i-1);
            } else {
                decalage = (nbCandidats-1) * (j+1) + i;  
            }
            if (score[(nbCandidats-1)*i+j] > score[decalage]) {
                tab_vainqueur[i] += 1;
            }
        }
        if (tab_vainqueur[i] == nbCandidats-1) {
            return i;
        }
    }
    return -1;

}

/// @fn void schulzes(ListeArcs *graphe, char *candidats[])
/// @brief Applique la méthode de Schulzes pour trouver le vainqueur de Condorcet.
/// @param[in] graphe Pointeur vers la liste des arcs du graphe.
/// @param[in] candidats Tableau des noms des candidats.
void schulzes(ListeArcs * graphe, char * candidats[]) {
    int score[MAX_CANDIDATS*MAX_CANDIDATS-MAX_CANDIDATS];
    trouverCheminsForts(graphe, MAX_CANDIDATS, score);
    int v = trouverVainqueurSchulzes(score, MAX_CANDIDATS);
    if (v==-1) {
        printf("ERREUR: Pas de vainqueur de schulzes\n");
        return;
    }
    printf("Résolution du paradoxe par la méthode de Sculzes\n\tLe candidat %s est le vainqueur de schulzes !\n", candidats[v]);
}
