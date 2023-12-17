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

bool equals(Arc * a1, Arc * a2) {
    return (a1->sommetArrivee) == (a2->sommetArrivee) && (a1->sommetDepart) == (a2->sommetDepart);
}

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

bool estDansListeSommet(int sommet, int * listeSommets, int taille) {
    for (int i=0; i<taille; i++) {
        if (listeSommets[i] == sommet) {
            return true;
        }
    }
    return false;
}

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

int trouverCheminFortAetB(ListeArcs *graphe, int A, int B) {
    int vus[100];
    int nbVus = 0;
    ListeArcs tmp = creerListeArcs();
    int poids = 0;
    trouverCheminsAetB(graphe, A, B, vus, nbVus, &tmp, &poids);
    return poids;
}

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

// methode à appeler pour faire schulzes
void schulzes(ListeArcs * graphe, char * candidats[]) {
    // todo: recuperer dans le fichier csv
    int score[MAX_CANDIDATS*MAX_CANDIDATS-MAX_CANDIDATS];
    trouverCheminsForts(graphe, MAX_CANDIDATS, score);
    int v = trouverVainqueurSchulzes(score, MAX_CANDIDATS);
    if (v==-1) {
        printf("ERREUR: Pas de vainqueur de schulzes\n");
        return;
    }
    printf("Résolution du paradoxe par la méthode de Sculzes\n\tLe candidat %s est le vainqueur de schulzes !\n", candidats[v]);
}