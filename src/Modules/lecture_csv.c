#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//#include "structures.h"
#include "lecture_csv.h"
#include "utils_sd.h"

/************************************----------Verify My Vote----------*****************************************/
void lecture_csv_votant(const char *nom_fichier, votant *votants[MAX_VOTANTS]) {
    FILE *file = fopen(nom_fichier, "r");

    if (file == NULL) {
        fprintf(stderr, "Erreur lors de l'ouverture du fichier.\n");
        exit(1);
    }

    char line[MAX_LINE_LENGTH];
    int votants_count = 0;

    // on ignore la première ligne
    if (fgets(line, sizeof(line), file) == NULL) {
        fprintf(stderr, "Le fichier est vide.\n");
        fclose(file);
        exit(1);
    }

    int num_etu;
    char key[50];
    while (fgets(line, sizeof(line), file)) {
        if (sscanf(line, "%d %50s", &num_etu, key) == 2) {
          votants[votants_count]->num_etu = num_etu;
          strcpy(votants[votants_count]->key, key);
          votants_count++;
        }
    }
    fclose(file);
}


void lecture_csv_vote(const char * nom_fichier, vote *votes[MAX_VOTES]) {
    FILE *file = fopen(nom_fichier, "r");

    if (file == NULL) {
        fprintf(stderr, "Erreur lors de l'ouverture du fichier.\n");
        exit(1);
    }

    char line[MAX_LINE_LENGTH];
    int nb_votants = 0;

    // on ignore la première ligne
    if (fgets(line, sizeof(line), file) == NULL) {
        fprintf(stderr, "Le fichier est vide.\n");
        fclose(file);
        exit(1);
    }

    while (fgets(line, sizeof(line), file)) {
        char *token = strtok(line, ",");
        int field_count = 0;
        while (token != NULL && field_count < 14) {
            switch (field_count) {
                case 0:
                    votes[nb_votants]->reponse = atoi(token);
                    break;
                case 1:
                    strcpy(votes[nb_votants]->date, token);
                    break;
                case 2:
                    strcpy(votes[nb_votants]->cours, token);
                    break;
                case 3:
                    strcpy(votes[nb_votants]->hashed, token);
                    break;
                default:
                    votes[nb_votants]->votes[field_count-4] = atoi(token);
                    break;
            }
            token = strtok(NULL, ",");
            field_count++;
        }
        nb_votants++;
    }
    fclose(file);
}


void lecture_csv_verifyMyVote(char* nom_fichier, votant* votants[MAX_VOTANTS], vote* votes[MAX_VOTES]) {
    if (votes == NULL && votants != NULL) {
        lecture_csv_votant(nom_fichier, votants);
    } else if (votes != NULL && votants == NULL) {
        lecture_csv_vote(nom_fichier, votes);
    } else {
        fprintf(stderr, "erreur appel fonction\n");
        exit(1);
    }
}

/************************************----------CONDORCET----------*****************************************/
void compter_lignes_colonnes_csv(const char * nom_fichier, int * lignes, int * colonnes) {
    char ligne[1000];
    char *token;

    FILE * fichier = fopen(nom_fichier, "r");

    if (fichier == NULL) {
        printf("Impossible d'ouvrir le fichier.\n");
        return;
    }

    // Compter les lignes et déterminer le nombre de colonnes
    while (fgets(ligne, sizeof(ligne), fichier) != NULL) {
        (*lignes)++;

        // Utilisation de strtok pour compter les virgules (séparateurs de colonnes)
        token = strtok(ligne, ",");
        while (token != NULL) {
            (*colonnes)++;
            token = strtok(NULL, ",");
        }
    }

    // Soustraire 1 pour corriger le comptage de la dernière ligne
    (*colonnes) = (*colonnes) / (*lignes) - 1;
    fclose(fichier);
}

void lecture_csv_score_condorcet(const char *nom_fichier, t_mat_char_star_dyn *matrice, int ligne, int colonne) {
    FILE *fichier = fopen(nom_fichier, "r");

    if (fichier == NULL) {
        perror("Erreur lors de l'ouverture du fichier");
        exit(EXIT_FAILURE);
    }

    char buffer[MAX_LINE_LENGTH];

    // Allouer mémoire pour les pointeurs de lignes
    matrice->vote = (char ***)malloc(ligne * sizeof(char **));

    // Lire les données du fichier et remplir la matrice
    for (int i = 0; i < ligne && fgets(buffer, sizeof(buffer), fichier) != NULL; ++i) {
        // Utiliser strtok pour diviser la ligne en tokens
        char *token = strtok(buffer, ",");
        matrice->vote[i] = (char **)malloc(colonne * sizeof(char *));

        for (int j = 0; j < colonne && token != NULL; ++j) {
            matrice->vote[i][j] = (char *)malloc((strlen(token) + 1) * sizeof(char));
            strcpy(matrice->vote[i][j], token);
            token = strtok(NULL, ",");
        }
    }

    fclose(fichier);
}