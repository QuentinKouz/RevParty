/// \file condorcet.c
/// \brief Programme d'implémentation calculant le vainqueur de condorcet s'il existe
/// \author Equipe-2 : Salim Moussaoui, Maxime Cherraf, Quentin Kouzmitch, Nicolas Giry
/// \date 5 decembre 2023

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "utils_sd.h"
#include "lecture_csv.h"

bool estDansEgalite(Candidat_mention recherche, Candidat_mention * liste_candidats, int taille_liste) {
    for (int i=0; i<taille_liste; i++) {
        if (liste_candidats[i].indice == recherche.indice) {
            return true;
        }
    }
    return false;
}

float calculer_pourcentage_mention_inf(Candidat_mention candidat) {
    float somme = 0;
    for (int i=candidat.mediane+1; i<6; i++) {
        somme += candidat.pourcentage[i];
    }
    return somme;
}

Candidat_mention chercher_vainqueur_jugement(Candidat_mention * candidats, int nb_candidats) {
    Candidat_mention vainqueur = candidats[0];
    Candidat_mention egalite[nb_candidats];
    int indice_egalite = 0;
    int mediane_min = candidats[0].mediane;
    for (int i=1; i<nb_candidats; i++) {
        if (candidats[i].mediane < mediane_min) {
            vainqueur = candidats[i];
            mediane_min = candidats[i].mediane;
        } else if (candidats[i].mediane == mediane_min) {
            if (!estDansEgalite(vainqueur, egalite, indice_egalite)) {
                egalite[indice_egalite] = vainqueur;
                indice_egalite ++;
            }
            egalite[indice_egalite] = candidats[i];
            indice_egalite ++;
        }
    }

    if (estDansEgalite(vainqueur, egalite, indice_egalite)) {
        float pourcentage_min = calculer_pourcentage_mention_inf(vainqueur);
        for (int i=1; i<indice_egalite; i++) {
            if (egalite[i].mediane == mediane_min) {
                int p = calculer_pourcentage_mention_inf(egalite[i]);
                if (p < pourcentage_min) {
                    vainqueur = egalite[i];
                    pourcentage_min = p;
                }
            }
        }
    }
    return vainqueur;
}

int calculerMediane(Candidat_mention candidat, int nb_electeurs) {
    int somme = 0;
    int i=0;
    do {
        somme += candidat.score[i];
        i++;
    } while (somme<nb_electeurs/2);
    return i-1;
}

void jugement_majoritaire(char * nom_fichier) {
    int nb_electeurs, nb_candidats;
    compter_lignes_colonnes_csv(nom_fichier, &nb_electeurs, &nb_candidats);

    Candidat_mention candidats[nb_candidats];

    lecture_csv_jugement(nom_fichier, &candidats, nb_candidats, nb_electeurs);

    // remplir les pourcentages
    for (int j=0; j<nb_candidats; j++){
        for (int i=0; i<6; i++) {
            candidats[j].pourcentage[i] = 100*(candidats[j].score[i] / nb_electeurs);
        }
        candidats[j].mediane = calculerMediane(candidats[j], nb_electeurs);
    }

    Candidat_mention vainqueur = chercher_vainqueur_jugement(candidats, nb_candidats);

    printf("Mode de scrutin : jugement majoritaire, %d candidats, %d votants\n", nb_candidats, nb_electeurs);
    printf("\tVainqueur = %s mention : %d (%.2f%%)\n", vainqueur.nom, vainqueur.mediane, vainqueur.pourcentage[vainqueur.mediane]);
}