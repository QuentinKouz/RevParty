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
#include <stdbool.h>

//TODO : recuperer les noms depuis le fichiers csv 
char* noms[] = {"Burger Black Pepper", "Burger Sud-Ouest", "Thai Burger", "Veggie Burger", "Fire Cracker", "Roma", "Crispy" "Cheese Burger", "Burger Surprise", "Country"};

int calculerNbArcsMax(int nb_candidats){
    int nb_arcs = 0;
    for (int i=nb_candidats-1; i>0; i--) {
        nb_arcs += i;
    }
    return nb_arcs;
}

bool estDansTab(int * tab, int sommet, int taille){
    for (int i=0; i<taille; i++){
        if (tab[i]==sommet)
            return true;
    }
    return false;
}

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

void condorcet(const char * fichier, int methodeParadoxe) {
    ListeArcs grapheCondorcet = creerListeArcs();
    obtenirGraphe(fichier, &grapheCondorcet);

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
            printf("Résolution du paradoxe par la méthode des paires\n");
            obtenirGraphePaire(fichier, &grapheCondorcetParadoxe);
            int vainqueur = trouverVainqueurCondorcet(grapheCondorcet);
            printf("\tLe vainqueur de condorcet est : %s\n", noms[vainqueur]);
            break;
        case 1: // schulzes
            obtenirGraphe(fichier, &grapheCondorcetParadoxe);
            schulzes(&grapheCondorcet);
            break;
        case 2: //minimax
            // todo Salim
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