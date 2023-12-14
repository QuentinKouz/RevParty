#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "structure_graphe.h"



/*
int main() {

    printf("nombre de possibilites de COMBINAISONS = %d\n",nombrePossibiltePermutation(MAX_CANDIDATS));


    // Initialiser la matrice
    t_mat_char_star_dyn mat;
    initMatrice(&mat, LIGNES, COLONNES);

    // Charger le fichier CSV dans la matrice
    lecture_csv_score_condorcet("ResultatsVote/VoteCondorcet.csv", &mat, LIGNES, COLONNES);

    // Afficher le contenu de la matrice à titre de test
    /*for (int i = 0; i < mat.ligne; ++i) {
        for (int j = 0; j < mat.colonne; ++j) {
            printf("%s ", mat.vote[i][j]);
        }
        printf("\n");
    }

    MatriceDuel matriceDuels = creerMatriceDuel(MAX_CANDIDATS, MAX_CANDIDATS);

    //remplir matrice des duels
    remplirMatriceDuel(&mat, &matriceDuels);

    // Affichage de la matrice des duels avec les noms des candidats
    afficherMatriceDuelAvecNoms(&matriceDuels, MAX_CANDIDATS);
    printf ("les duels se lisent en fonction des lignes ex :\n BBP a gagne 23 fois contre BSO\n et BSO 15 fois contre BBP\n");
    printf("\n");

    // Créer la liste des arcs à partir de la matrice des duels
    ListeArcs listeArcs = creerListeArcsDepuisMatrice(matriceDuels, MAX_CANDIDATS);

    // Afficher la liste des arcs
    afficherListeArcs(&listeArcs,candidats);


    // Libération de la mémoire
    freeMatrice(&mat);
    libererMatriceDuel(&matriceDuels);
    // Libérer la mémoire de la liste des arcs
    libererListeArcs(&listeArcs);

    return 0;
}
*/
