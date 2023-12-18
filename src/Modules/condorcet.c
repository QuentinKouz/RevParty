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
#include "condorcet_paires.h"
#include "condorcet_minimax.h"
#include <stdbool.h>


/// @fn void trierGraphe(ListeArcs *graphe)
/// @brief Trie les arcs du graphe en fonction de leur poids.
/// @param[in,out] graphe Pointeur vers la liste des arcs du graphe.
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

/// @fn int calculerNbArcsMax(int nb_candidats)
/// @brief Calcule le nombre maximal d'arcs possibles pour un nombre donné de candidats.
/// @param[in] nb_candidats Nombre de candidats.
/// @return Nombre d'arcs.
int calculerNbArcsMax(int nb_candidats){
    int nb_arcs = 0;
    for (int i=nb_candidats-1; i>0; i--) {
        nb_arcs += i;
    }
    return nb_arcs;
}

/// @fn bool estDansTab(int *tab, int sommet, int taille)
/// @brief Vérifie si un sommet est présent dans un tableau.
/// @param[in] tab Tableau d'entiers.
/// @param[in] sommet Sommet à vérifier.
/// @param[in] taille Taille du tableau.
/// @return Vrai si le sommet est dans le tableau, faux sinon.
bool estDansTab(int * tab, int sommet, int taille){
    for (int i=0; i<taille; i++){
        if (tab[i]==sommet)
            return true;
    }
    return false;
}

/// @fn void retirer(int *tab, int sommet, int taille)
/// @brief Retire un sommet du tableau.
/// @param[in,out] tab Tableau d'entiers.
/// @param[in] sommet Sommet à retirer.
/// @param[in] taille Taille du tableau.
void retirer(int * tab, int sommet, int taille) {
    bool trouve = false;
    for (int i=0; i<taille-1; i++) {
        if (tab[i] == sommet) {
            trouve = true;
        }
        if (trouve) {
            tab[i] = tab[i+1];
        }
    }
}

/// @fn int trouverVainqueurCondorcet(ListeArcs grapheCondorcet)
/// @brief Trouve le vainqueur de Condorcet dans un graphe donné.
/// @param[in] grapheCondorcet Graphe des duels entre candidats.
/// @return Index du vainqueur de Condorcet, -1 s'il n'y en a pas.
int trouverVainqueurCondorcet(ListeArcs grapheCondorcet) {
    int candidats[MAX_CANDIDATS] ;
    Arc* e = grapheCondorcet.debut;
    int i=0;
    while (e!=NULL){
        if(!estDansTab(candidats, e->sommetDepart, i)){
            candidats[i] = e->sommetDepart;
            i++;
        }
        e = e->suivant;
    }
    e = grapheCondorcet.debut;
    while (e!=NULL){
        if(estDansTab(candidats, e->sommetArrivee, i)){
            retirer(candidats, e->sommetArrivee, i);
            i--;
        }
        e = e->suivant;
    }
    if (i==1){
        // il y a un vainqueur de condorcet :
        return candidats[0];
    }
    // pas de vainqueur de condorcet...
    return -1;
}

/// @fn void obtenirGraphe(const char* fichier, ListeArcs *graphe,,const char* log)
/// @brief Obtient le graphe à partir d'un fichier CSV.
/// @param[in] fichier Chemin vers le fichier CSV.
/// @param[in] log fichier log
void obtenirGraphe(const char* fichier, ListeArcs * graphe,const char* log) {
    // obtenir liste arcs et matrice duels
    t_mat_char_star_dyn mat;
    int l = 0;
    int c = 0;
    compter_lignes_colonnes_csv(fichier, &l, &c);
    initMatrice(&mat, l, c);
    lecture_csv_score_condorcet(fichier, &mat, LIGNES, COLONNES);
    MatriceDuel matriceDuels = creerMatriceDuel(MAX_CANDIDATS, MAX_CANDIDATS);
    remplirMatriceDuel(&mat, &matriceDuels);
    char * noms[MAX_CANDIDATS];
    lire_noms_candidats_csv(fichier, &noms, MAX_CANDIDATS);
    afficherMatriceDuelAvecNoms(&matriceDuels, MAX_CANDIDATS,log);
    creerListeArcsDepuisMatrice(matriceDuels, MAX_CANDIDATS, graphe);
    afficherListeArcs (graphe, log);
}

/// @fn void condorcet(const char *fichier, int methodeParadoxe,,const char* log)
/// @brief Exécute l'algorithme de Condorcet en utilisant une méthode spécifique en cas de paradoxe.
/// @param[in] fichier Chemin vers le fichier CSV.
/// @param[in] methodeParadoxe Méthode à utiliser en cas de paradoxe (0 pour paires, 1 pour Schulzes, 2 pour Minimax).
/// @param[in] log fichier log
void condorcet(const char * fichier, int methodeParadoxe,const char* log) {
    ListeArcs grapheCondorcet = creerListeArcs();
    obtenirGraphe(fichier, &grapheCondorcet,log);
    char * noms[MAX_CANDIDATS];
    lire_noms_candidats_csv(fichier, &noms, MAX_CANDIDATS);
    int vainqueur = trouverVainqueurCondorcet(grapheCondorcet);
    printf("Mode de scrutin : Condorcet\n");
    if (vainqueur != -1) {
        printf("\tLe vainqueur de condorcet est : %s\n", noms[vainqueur]);
        return;
    }
    printf("PARADOXE DE CONDORCET\n");
    ListeArcs grapheCondorcetParadoxe = creerListeArcs();

    switch (methodeParadoxe) {
        case 0: // paire
            printf("Résolution du paradoxe par la méthode des Paires\n");
            obtenirGraphePaire(fichier, &grapheCondorcetParadoxe);
            int vainqueur = trouverVainqueurCondorcet(grapheCondorcetParadoxe);
            printf("\tLe candidats %s est vainqueur de Condorcet paires !\n", noms[vainqueur]);
            break;
        case 1: // schulzes
            obtenirGraphe(fichier, &grapheCondorcetParadoxe,log);
            schulzes(&grapheCondorcet, noms);
            break;
        case 2: //minimax
              obtenirGraphe(fichier, &grapheCondorcetParadoxe,log);
              minimax(&grapheCondorcet, noms);
            break;
    }
}
