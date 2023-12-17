/// \file main.c
/// \brief point d'entrée (_start) ou main du programme dans le programme.
/// \author Equipe-2 : Salim Moussaoui, Maxime Cherraf, Quentin Kouzmitch, Nicolas Giry
/// \date 11 decembre 2023

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>
#include "utils_sd.h"
#include "uninominales.h"
#include "condorcet.h"


int main(int argc, char *argv[]) {
    
    // Déclaration des variables pour les options de la ligne de commande
    char *fichierEntree = NULL;
    char *fichierLog = NULL;
    char *methode = NULL;

    // Variables pour le fichier de log
    FILE *logfp = stdout; // Par défaut, l'écran est utilisé comme fichier de log

    // Variable pour vérifier si l'option -d ou -i est présente
    int duelOption = 0;

    // Analyse des options de la ligne de commande
    int opt;
    while ((opt = getopt(argc, argv, "i:d:o:m:")) != -1) {
        switch (opt) {
            case 'i':
                fichierEntree = optarg;
                break;
            case 'd':
                fichierEntree = optarg;
                duelOption = 1;
                break;
            case 'o':
                fichierLog = optarg;
                break;
            case 'm':
                methode = optarg;
                break;
            case '?':
                //variable optopt recupère le caractère erronnée
                fprintf(stderr, "Option inconnue: -%c\n", optopt);
                exit(EXIT_FAILURE);
            default:
                fprintf(stderr, "Usage: %s -i fichier | -d fichier -o fichier_log -m method\n", argv[0]);
                exit(EXIT_FAILURE);
        }
    }

    // Vérification des incompatibilités entre les options 
    if (strcmp(methode,"all")!=0){
        if ((fichierEntree == NULL || methode == NULL || (strcmp(methode, "cm") == 0 || 
           strcmp(methode, "cp") == 0||strcmp(methode, "cs") == 0) && !duelOption) ||                    
           (duelOption && (fichierEntree == NULL || methode == NULL ||  
           (strcmp(methode, "uni1") == 0 || strcmp(methode, "uni2") == 0 || 
           strcmp(methode, "jm") == 0)))) {
            
            fprintf(stderr, "Options incompatibles ou manquantes.\n");
            exit(EXIT_FAILURE);
        }
        
    }

    
    // Vérification de l'existence du fichier d'entrée et de son extension
    if (fichierEntree != NULL) {
        // Vérification de l'extension du fichier sans ouvrir le fichier
        if (strstr(fichierEntree, ".csv") == NULL) {
            fprintf(stderr, "Erreur : Le fichier d'entrée ne possède pas l'extension .csv.\n");
            exit(EXIT_FAILURE);
        }

        FILE *f = fopen(fichierEntree, "r");
        if (f == NULL) {
            perror("Erreur lors de l'ouverture du fichier d'entrée");
            exit(EXIT_FAILURE);
        }

        fclose(f);
    }
    
    
    // Ouverture du fichier de log si spécifié
    if (fichierLog != NULL) {
        logfp = fopen(fichierLog, "w");
        printf("logfp ouvert\n");
        if (logfp == NULL) {
            perror("Erreur lors de l'ouverture du fichier de log");
            exit(EXIT_FAILURE);
        }
    }
    
    
    // Traitement en fonction des options
    if (strcmp(methode, "all") == 0){
        printf("all\n");
    }
    else if (strcmp(methode, "uni1") == 0) {
        // Code pour uni1
        uninominales(fichierEntree, 1);
    } 
    else if (strcmp(methode, "uni2") == 0) {
        uninominales(fichierEntree, 2);
    } 
    else if (strcmp(methode, "cm") == 0) {
        //fprintf(logfp,"je suis dans cm\n");
        condorcet(fichierEntree, 2);
    } 
    else if (strcmp(methode, "cp") == 0) {
        condorcet(fichierEntree, 0);
    } 
    else if (strcmp(methode, "cs") == 0) {
        condorcet(fichierEntree, 1);
    } 
    else if (strcmp(methode, "jm") == 0) {
        // Code pour jm
        jugement_majoritaire(fichierEntree);
    } 
    else {
        fprintf(stderr, "Erreur : Méthode non reconnue.\n");
        exit(EXIT_FAILURE);
    }

    
    // Fermeture du fichier de log si différent de stdout
    if (logfp != stdout) {
        fclose(logfp);
    }

    return 0;
}
