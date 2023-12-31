/// \file lecture_csv.c
/// \brief Programme de lecture csv
/// \author Equipe-2 : Salim Moussaoui, Maxime Cherraf, Quentin Kouzmitch, Nicolas Giry
/// \date 5 decembre 2023

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lecture_csv.h"
#include "utils_sd.h"

/************************************----------Verify My Vote----------*****************************************/

/// @fn void lecture_csv_votant(const char *nom_fichier, votant *votants[MAX_VOTANTS])
/// @brief Lit les données des votants à partir d'un fichier CSV.
/// @param[in] nom_fichier Chemin vers le fichier CSV.
/// @param[in] votants Tableau de pointeurs vers les structures de votants.
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

/// @fn void lecture_csv_vote(const char *nom_fichier, vote *votes[MAX_VOTES])
/// @brief Lit les données des votes à partir d'un fichier CSV.
/// @param[in] nom_fichier Chemin vers le fichier CSV.
/// @param[in] votes Tableau de pointeurs vers les structures de votes.
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

/// @fn void lecture_csv_verifyMyVote(char* nom_fichier, votant* votants[MAX_VOTANTS], vote* votes[MAX_VOTES])
/// @brief Lit les données des votants ou des votes.
/// @param[in] nom_fichier Chemin vers le fichier CSV.
/// @param[in] votants Tableau de pointeurs vers les structures de votants.
/// @param[in] votes Tableau de pointeurs vers les structures de votes.
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

/// @fn void compter_lignes_colonnes_csv(const char *nom_fichier, int *lignes, int *colonnes)
/// @brief Compte les lignes et les colonnes d'un fichier CSV.
/// @param[in] nom_fichier Chemin vers le fichier CSV.
/// @param[in] lignes Pointeur vers le nombre total de lignes.
/// @param[in] colonnes Pointeur vers le nombre total de colonnes.
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
    (*colonnes) = (*colonnes) / (*lignes) - 4;
    (*lignes)--;
    fclose(fichier);
}

/// @fn void lecture_csv_score_condorcet(const char *nom_fichier, t_mat_char_star_dyn *matrice, int ligne, int colonne)
/// @brief Lit les scores de Condorcet à partir d'un fichier CSV dans une matrice dynamique.
/// @param[in] nom_fichier Chemin vers le fichier CSV.
/// @param[in] matrice Pointeur vers la matrice dynamique à remplir.
/// @param[in] ligne Nombre de lignes de la matrice.
/// @param[in] colonne Nombre de colonnes de la matrice.
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

/// @fn void lire_noms_candidats_csv(const char *nom_fichier, char *noms[], int nb_max)
/// @brief Lit les noms des candidats à partir d'un fichier CSV.
/// @param[in] nom_fichier Chemin vers le fichier CSV.
/// @param[in] noms Tableau de pointeurs vers les chaînes de caractères des noms.
/// @param[in] nb_max Nombre maximum de candidats à lire.
void lire_noms_candidats_csv(const char *nom_fichier, char *noms[], int nb_max) {
    FILE *fichier = fopen(nom_fichier, "r, ccs=UTF-8");
    
    if (fichier == NULL) {
        perror("Erreur lors de l'ouverture du fichier");
        exit(EXIT_FAILURE);
    }

    char ligne[1024];
    fgets(ligne, sizeof(ligne), fichier);
    char * token = strtok(ligne, ",");
    int i=0;

    //ignore les 4 premières colonnes
    for (int j=0; j<4; j++) {
        token = strtok(NULL, ",");
    }

    while (token != NULL) {
        if (i<nb_max) {
            char *pos_espace = strchr(token, ' ');//arg int
             // Si un espace est trouvé, avancer la position du token
            // pour pointer juste après l'espace
            if (pos_espace != NULL) 
                token = pos_espace + 3;
            noms[i] = malloc(strlen(token) + 1);
            strcpy(noms[i], token);
            i++;
        }
        token = strtok(NULL, ",");
    }

    fclose(fichier);
}

/// @fn void lecture_csv_jugement(const char *nom_fichier, Candidat_mention **candidats, int nb_candidats, int nb_electeurs)
/// @brief Lit un fichier CSV et remplit un tableau de structures Candidat_mention.
/// @param[in] nom_fichier Nom du fichier CSV à lire.
/// @param[in] candidats Pointeur vers un tableau de structures Candidat_mention à remplir.
/// @param[in] nb_candidats Nombre de candidats.
/// @param[in] nb_electeurs Nombre d'électeurs.
void lecture_csv_jugement(const char *nom_fichier, Candidat_mention **candidats, int nb_candidats, int nb_electeurs) {
    FILE *fichier = fopen(nom_fichier, "r, ccs=UTF-8");
    if (fichier == NULL) {
        perror("Erreur lors de l'ouverture du fichier");
        exit(EXIT_FAILURE);
    }

    char ligne[1024];
    // on ignore la première ligne
    if (fgets(ligne, sizeof(ligne), fichier) == NULL) {
        fprintf(stderr, "Le fichier est vide.\n");
        fclose(fichier);
        exit(EXIT_FAILURE);
    }

    *candidats = (Candidat_mention *)malloc(nb_candidats * sizeof(Candidat_mention));
    if (*candidats == NULL) {
        perror("Erreur d'allocation de mémoire");
        fclose(fichier);
        exit(EXIT_FAILURE);
    }

    char *noms_burgers[nb_candidats];
    lire_noms_candidats_csv(nom_fichier, &noms_burgers, nb_candidats);
    
    for (int i = 0; i < nb_candidats; i++) {
        // Copiez le nom dans le champ 'nom' de la structure
        strcpy((*candidats)[i].nom, noms_burgers[i]);
    }
    while (fgets(ligne, sizeof(ligne), fichier)) {
        char *valeurVote = strtok(ligne, ",");
        for (int i = 0; i < 3; i++) {
            valeurVote = strtok(NULL, ",");
        }
        for (int i = 0; i < nb_candidats; i++) {
            valeurVote = strtok(NULL, ",");
            int vote = atoi(valeurVote);
            
            if (vote != -1) {
                (*candidats)[i].score[vote-1]++;
            }
        }
    }
}


/// @fn void lecture_csv_jugement_conversion_mention(const char *nom_fichier, Candidat_mention **candidats, int nb_candidats, int nb_electeurs)
/// @brief Lit un fichier CSV et fait la conversion des mentions pour remplir un tableau de structures Candidat_mention.
/// @param[in] nom_fichier Nom du fichier CSV à lire.
/// @param[in] candidats Pointeur vers un tableau de structures Candidat_mention à remplir.
/// @param[in] nb_candidats Nombre de candidats.
/// @param[in] nb_electeurs Nombre d'électeurs.
void lecture_csv_jugement_conversion_mention(const char *nom_fichier, Candidat_mention **candidats, int nb_candidats, int nb_electeurs) {
    FILE *fichier = fopen(nom_fichier, "r, ccs=UTF-8");
    if (fichier == NULL) {
        perror("Erreur lors de l'ouverture du fichier");
        exit(EXIT_FAILURE);
    }

    char ligne[1024];
    // on ignore la première ligne
    if (fgets(ligne, sizeof(ligne), fichier) == NULL) {
        fprintf(stderr, "Le fichier est vide.\n");
        fclose(fichier);
        exit(EXIT_FAILURE);
    }

    *candidats = (Candidat_mention *)malloc(nb_candidats * sizeof(Candidat_mention));
    if (*candidats == NULL) {
        perror("Erreur d'allocation de mémoire");
        fclose(fichier);
        exit(EXIT_FAILURE);
    }

    char *noms_burgers[nb_candidats];
    lire_noms_candidats_csv(nom_fichier, &noms_burgers, nb_candidats);
    
    for (int i = 0; i < nb_candidats; i++) {
        // Copiez le nom dans le champ 'nom' de la structure
        strcpy((*candidats)[i].nom, noms_burgers[i]);
    }
    while (fgets(ligne, sizeof(ligne), fichier)) {
        char *valeurVote = strtok(ligne, ",");
        for (int i = 0; i < 3; i++) {
            valeurVote = strtok(NULL, ",");
        }
        for (int i = 0; i < nb_candidats; i++) {
            valeurVote = strtok(NULL, ",");
            int vote = atoi(valeurVote);

            // conversion vote en mention : 1:TB 2-3:B  4-5:AB 6-7:P 8-9:M 10:AF
            switch (vote) {
                case 2:
                case 3:
                    vote = 2;
                    break;
                case 4:
                case 5:
                    vote = 3;
                    break;
                case 6:
                case 7:
                    vote = 4;
                    break;
                case 8:
                case 9:
                    vote = 5;
                    break;
                case (10):
                    vote = 6;
                    break;
            }
            
            if (vote != -1) {
                (*candidats)[i].score[vote-1]++;
            }
        }
    }
}
