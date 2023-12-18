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

//TODO : recuperer les noms depuis le fichiers csv 
//char* noms[] = {"Burger Black Pepper", "Burger Sud-Ouest", "Thai Burger", "Veggie Burger", "Fire Cracker", "Roma", "Crispy" "Cheese Burger", "Burger Surprise", "Country"};

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

/// @fn void obtenirGraphe(const char* fichier, ListeArcs *graphe)
/// @brief Obtient le graphe à partir d'un fichier CSV.
/// @param[in] fichier Chemin vers le fichier CSV.
void obtenirGraphe(const char* fichier, ListeArcs * graphe) {
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
}

/// @fn void condorcet(const char *fichier, int methodeParadoxe)
/// @brief Exécute l'algorithme de Condorcet en utilisant une méthode spécifique en cas de paradoxe.
/// @param[in] fichier Chemin vers le fichier CSV.
/// @param[in] methodeParadoxe Méthode à utiliser en cas de paradoxe (0 pour paires, 1 pour Schulzes, 2 pour Minimax).
void condorcet(const char * fichier, int methodeParadoxe) {
    ListeArcs grapheCondorcet = creerListeArcs();
    obtenirGraphe(fichier, &grapheCondorcet);
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
            obtenirGraphe(fichier, &grapheCondorcetParadoxe);
            schulzes(&grapheCondorcet, noms);
            break;
        case 2: //minimax
              obtenirGraphe(fichier, &grapheCondorcetParadoxe);
              minimax(&grapheCondorcet, noms);
            break;
    }
}

/*
int main(int argc, const char* argv[]) {
    if (argc != 2) {
        printf("Usage: %s <fichier_condorcet>\n", argv[0]);
        exit(2);
    }   
    condorcet(argv[1], 0);
}*/