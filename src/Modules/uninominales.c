/// \file uninominales.c
/// \brief Programme d'implémentation de la méthode uninominal à un tour ou deux tours selon le paramètre.
/// \author Equipe-2 : Salim Moussaoui, Maxime Cherraf, Quentin Kouzmitch, Nicolas Giry
/// \date 9 novembre 2023

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utils_sd.h"
#include "lecture_csv.h"

#define MAX_CANDIDATS 10
#define MAX_VOTANTS 100
#define TAILLE_MAX_LIGNE 1024




/// @fn void ignorerEnTetes(FILE *fichier)
/// @brief Ignore la première ligne (entêtes) dans un fichier CSV.
/// @param[in] fichier Pointeur vers le fichier CSV ouvert.
void ignorerEnTetes(FILE *fichier) {
    char buffer[TAILLE_MAX_LIGNE];
    fgets(buffer, TAILLE_MAX_LIGNE, fichier);
}

/// @fn void removeCandidat(Candidat candidat, Candidat candidats[], int * nbCandidats)
/// @brief enleve candidat du tableau candidats et met à jour nbCandidats.
/// @param[in] candidat candidat à enlever.
/// @param[in] candidats Tableau des candidats.
/// @param[in] nbCandidats Nombre de candidats dans le tableau..
void removeCandidat(Candidat candidat, Candidat candidats[], int *nbCandidats) {
    int trouve = 0;
    for (int i=0; i<(*nbCandidats)-1; i++) {
        if (strcmp(candidat.nom, candidats[i].nom)==0) {
            trouve = 1;
        }
        if (trouve==1) {
            candidats[i] = candidats[i+1];
        }
    }
    if (trouve==1)
        (*nbCandidats) --;
}


/// @fn Candidat trouverVainqueur(Candidat candidats[], int nbCandidats, unsigned * indiceVainqueur)
/// @brief Trouve le candidat avec le score le plus élevé, et enseigne son indice dans indiceVainqueur
/// @param[in] candidats Tableau des candidats.
/// @param[in] nbCandidats Nombre de candidats dans le tableau.
/// @param[in] indiceVainqueur reference à affecter avec l'indice du vainqueur.
/// @return Le candidat gagnant.
Candidat trouverVainqueur(Candidat candidats[], int nbCandidats, unsigned * indiceVainqueur) {
    Candidat vainqueur = candidats[0];
    (* indiceVainqueur) = 0;
    for (int i = 1; i < nbCandidats; i++) {
        if (candidats[i].score > vainqueur.score) {
            vainqueur = candidats[i];
            (* indiceVainqueur) = i;
        }
    }
    return vainqueur;
}

/// @fn void uninominales(const char *nomFichier)
/// @brief Détermine le vainqueur basé sur les données du fichier spécifié.
/// @param[in] nomFichier Nom du fichier contenant les résultats du vote.
/// @param[in] log fichier log 
void uninominales(const char *nomFichier, int nbTours,const char * log) {

    if (nbTours !=1 && nbTours !=2) {
        fprintf(stderr,"Erreur paramètres de fonction\n");
        exit(1);
    }

    char cheminComplet[100];
    sprintf(cheminComplet, "%s", nomFichier);

    FILE *fichier = fopen(cheminComplet, "r");
    if (!fichier) {
        printf("Erreur d'ouverture du fichier %s\n", cheminComplet);
        exit(1);
    }

    ignorerEnTetes(fichier);

    int nbCandidats = MAX_CANDIDATS;

    char * noms[MAX_CANDIDATS];
    lire_noms_candidats_csv(cheminComplet, &noms, MAX_CANDIDATS);

    Candidat candidats[MAX_CANDIDATS];
    
    for (int i=0; i<MAX_CANDIDATS; i++) {
        strcpy(candidats[i].nom, noms[i]);
        candidats[i].score = 0; 
    }

    int votesVotants[MAX_VOTANTS][MAX_CANDIDATS];

    int nbVotants = 0;
    char buffer[TAILLE_MAX_LIGNE];

    while (fgets(buffer, TAILLE_MAX_LIGNE, fichier)) {
        char *valeurVote = strtok(buffer, ",");
        for (int i = 0; i < 4; i++) { // On ignore les 4 premières colonnes des fichiers CSV
            valeurVote = strtok(NULL, ",");
        }

        for (int i = 0; i < MAX_CANDIDATS; i++) {
            if (valeurVote != NULL) {
                int vote = atoi(valeurVote);
                votesVotants[nbVotants][i] = vote;
                if (vote == 1) {
                    candidats[i].score += 1;
                }
                valeurVote = strtok(NULL, ",");
            }
        }
        nbVotants++;
    }

    fclose(fichier);

    Candidat vainqueurs [2];
    unsigned indiceVainqueurs [2];
    int nbVotes = 0;

    // certains électeurs ont voté plusieurs fois "1" pour un candidat, pour ne pas avoir à choisir le candidat voté nous les comptons tous, bien que cela donne plus de voix à certaines personnes.
    // le nombre total de votes diffère donc du nombre de votants, et nous en avons besoin pour calculer le score de chaque candidat en %
    fprintf(log, "SCORES CANDIDATS UNINOMINALE\n\n");
    fprintf(log, "%-25s | %-10s\n", "Nom", "Score");
    for (int i=0; i<MAX_CANDIDATS; i++) {
        nbVotes += candidats[i].score;
        fprintf(log, "%-25s | %-10d\n", candidats[i].nom, candidats[i].score);
    }

    vainqueurs[0] = trouverVainqueur(candidats, nbCandidats, &indiceVainqueurs[0]);
    removeCandidat(vainqueurs[0], candidats, &nbCandidats);
    vainqueurs[1] = trouverVainqueur(candidats, nbCandidats, &indiceVainqueurs[1]);

    // comme nous enlevons le premier candidats, si le deuxième se trouve après, il sera décalé de -1, nous rectifions donc ce décalage
    if (indiceVainqueurs[1]>=indiceVainqueurs[0]) {
        indiceVainqueurs[1] ++;
    }

    float scores[2];
    scores[0] = ((float)vainqueurs[0].score / (float)(nbVotes)) * 100;
    scores[1] = ((float)vainqueurs[1].score / (float)(nbVotes)) * 100;

    if (nbTours == 1) {
        printf("Mode de scrutin : uninominal à 1 tour, %d candidats, %d votants\n", MAX_CANDIDATS, nbVotants-1);
        printf("\tVainqueur = %s (%.2f%%)\n", vainqueurs[0].nom, scores[0]);
        return;
    } 

    printf("Mode de scrutin : uninominal à 2 tours, %d candidats, %d votants\n", MAX_CANDIDATS, nbVotants-1);

    printf("\tPremier tour : vainqueurs = %s (%.2f%%), %s (%.2f%%)\n", vainqueurs[0].nom, scores[0], vainqueurs[1].nom, scores[1]);

    /************************************************----------Deuxième tour----------*************************************************/

    vainqueurs[0].score = 0;
    vainqueurs[1].score = 0;
    int voteC1;
    int voteC2;

    for (int i=0; i<nbVotants; i++) {
        voteC1 = votesVotants[i][indiceVainqueurs[0]];
        voteC2 = votesVotants[i][indiceVainqueurs[1]];
        if (voteC1==voteC2) {
            vainqueurs[0].score ++;
            vainqueurs[1].score ++;
        } else if( voteC1 < voteC2 ) {
            if (voteC1 != -1) {
                vainqueurs[0].score ++;
            } else {
                vainqueurs[1].score ++;
            }
        } else {
            if (voteC2 != -1) {
                vainqueurs[1].score ++;
            } else {
                vainqueurs[0].score ++;
            }
        }
    }

    Candidat vainqueur;

    if (vainqueurs[0].score==vainqueurs[1].score) {
        fprintf(stderr, "Les deux candidats sont à égalité ! Un nouveau vote est nécessaire pour déterminer un vainqueur.\n");
        return;
    } else if (vainqueurs[0].score >vainqueurs[1].score) {
        vainqueur = vainqueurs[0];
    } else {
        vainqueur = vainqueurs[1];
    }

    printf("\tDeuxième tour : vainqueur = %s (%.2f%%)\n", vainqueur.nom, (float) ((float) vainqueur.score/((float) vainqueurs[0].score + vainqueurs[1].score))*100);
}
