/// \file condorcet.c
/// \brief Programme d'implémentation calculant le vainqueur de condorcet s'il existe
/// \author Equipe-2 : Salim Moussaoui, Maxime Cherraf, Quentin Kouzmitch, Nicolas Giry
/// \date 5 decembre 2023

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utils_sd.h"
#include "lecture_csv.h"
#include "condorcet_schulzes.h"
#include <stdbool.h>


void trierGraphe(ListeArcs * graphe) {
    ListeArcs graphe_trie = creerListeArcs();
    int t = graphe->taille;
    for (int i=0; i<t; i++) {
        int poidsMax = graphe->debut->poids;
        Arc *arcVainqueur = graphe->debut;
        Arc *p = graphe->debut;
        while (p != NULL) {
            if (p->poids>poidsMax) {
                poidsMax = p->poids;
                arcVainqueur = p;
            }
            p = p->suivant;
        }
        ajouterArc(&graphe_trie, arcVainqueur);
        retirerArc(graphe, arcVainqueur);
    }
    graphe->debut = graphe_trie.debut;
}

bool creeCycle(ListeArcs *grapheCondorcet, Arc* arc) {
    // renvoie true si lors du parcours du graphe depuis le sommet destination de l'arc, on tombe sur le sommet source de l'arc
    int sommetDest = arc->sommetArrivee;
    int sommetDepart= arc->sommetDepart;

    if (estDansGraphe(grapheCondorcet, sommetDest)) {
        ListeArcs arcsAVerifier = creerListeArcs();
        ajouterArcSommet(grapheCondorcet, &arcsAVerifier, sommetDest);
        Arc* e = arcsAVerifier.debut;
        while (e!=NULL) {
            if (e->sommetArrivee == sommetDepart)
                // il y a un cycle
                return true;
            ajouterArcSommet(grapheCondorcet, &arcsAVerifier, e->sommetArrivee);
            e = e->suivant;
        }
    }
    return false;
}

void ajouterArcsSansCycle(ListeArcs *grapheCondorcet,ListeArcs* listeArcsTries) {
    Arc* arc = listeArcsTries->debut;
    while (arc!=NULL) {
        if(!creeCycle(grapheCondorcet, arc)) {
            ajouterArc(grapheCondorcet, arc);
        }
        arc = arc->suivant;
    }
}

void obtenirGraphePaire(const char* fichier, ListeArcs * graphe) {
    // obtenir liste arcs et matrice duels
    t_mat_char_star_dyn mat;
    int l = 0;
    int c = 0;
    compter_lignes_colonnes_csv(fichier, &l, &c);
    initMatrice(&mat, l, c);
    lecture_csv_score_condorcet(fichier, &mat, LIGNES, COLONNES);
    MatriceDuel matriceDuels = creerMatriceDuel(MAX_CANDIDATS, MAX_CANDIDATS);
    remplirMatriceDuel(&mat, &matriceDuels);
    creerListeArcsDepuisMatrice(matriceDuels, MAX_CANDIDATS, graphe);

    // trier graphe
    trierGraphe(graphe);

    // ajout des arcs un par un sans cycle
    ListeArcs grapheCondorcet = creerListeArcs();
    ajouterArcsSansCycle(&grapheCondorcet, graphe);

    graphe = &grapheCondorcet;
}