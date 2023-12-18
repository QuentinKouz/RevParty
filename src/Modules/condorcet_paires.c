/// \file condorcet_paires.c
/// \brief Programme d'implémentation calculant le vainqueur de condorcet paire
/// \author Equipe-2 : Salim Moussaoui, Maxime Cherraf, Quentin Kouzmitch, Nicolas Giry
/// \date 15 decembre 2023

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utils_sd.h"
#include "lecture_csv.h"
#include "condorcet_schulzes.h"
#include "condorcet.h"
#include <stdbool.h>

/// @fn bool creeCycle(ListeArcs *grapheCondorcet, Arc* arc)
/// @brief Vérifie si l'ajout d'un arc crée un cycle dans le graphe.
/// @param[in] grapheCondorcet Pointeur vers le graphe de Condorcet.
/// @param[in] arc Pointeur vers l'arc à vérifier.
/// @return Vrai si l'ajout de l'arc crée un cycle, faux sinon.
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

/// @fn void ajouterArcsSansCycle(ListeArcs *grapheCondorcet, ListeArcs* listeArcsTries)
/// @brief Ajoute des arcs à un graphe de Condorcet en évitant de créer des cycles.
/// @param[in,out] grapheCondorcet Pointeur vers le graphe de Condorcet.
/// @param[in] listeArcsTries Pointeur vers la liste des arcs triés.
void ajouterArcsSansCycle(ListeArcs *grapheCondorcet,ListeArcs* listeArcsTries) {
    Arc* arc = listeArcsTries->debut;
    while (arc!=NULL) {
        if(!creeCycle(grapheCondorcet, arc)) {
            ajouterArc(grapheCondorcet, arc);
        }
        arc = arc->suivant;
    }
}

/// @fn void obtenirGraphePaire(const char* fichier, ListeArcs *graphe)
/// @brief Construit le graphe de Condorcet en utilisant la méthode des paires à partir d'un fichier CSV.
/// @param[in] fichier Chemin vers le fichier CSV.
/// @param[in] graphe Pointeur vers le graphe de Condorcet à construire.
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
    ListeArcs grapheCondorcetPaire = creerListeArcs();
    ajouterArcsSansCycle(&grapheCondorcetPaire, graphe);

    graphe->debut = grapheCondorcetPaire.debut;
}
