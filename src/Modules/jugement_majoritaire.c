/// \file jugement_majoritaire.c
/// \brief Programme d'implémentation calculant le vainqueur avec la méthode Jugement majoritaire
/// \author Equipe-2 : Salim Moussaoui, Maxime Cherraf, Quentin Kouzmitch, Nicolas Giry
/// \date 5 decembre 2023

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "utils_sd.h"
#include "lecture_csv.h"

/// @fn bool estDansEgalite(Candidat_mention recherche, Candidat_mention *liste_candidats, int taille_liste)
/// @brief Vérifie si un candidat est présent dans une liste de candidats mentionnés pour égalité.
/// @param[in] recherche Candidat à rechercher.
/// @param[in] liste_candidats Liste des candidats mentionnés.
/// @param[in] taille_liste Taille de la liste.
/// @return Vrai si le candidat est dans la liste, faux sinon.
bool estDansEgalite(Candidat_mention recherche, Candidat_mention * liste_candidats, int taille_liste) {
    for (int i=0; i<taille_liste; i++) {
        if (liste_candidats[i].indice == recherche.indice) {
            return true;
        }
    }
    return false;
}

/// @fn float calculer_pourcentage_mention_inf(Candidat_mention candidat)
/// @brief Calcule le pourcentage des mentions inférieures à la médiane pour un candidat.
/// @param[in] candidat Candidat dont on calcule le pourcentage.
/// @return Pourcentage des mentions inférieures à la médiane.
float calculer_pourcentage_mention_inf(Candidat_mention candidat) {
    float somme = 0;
    for (int i=candidat.mediane+1; i<6; i++) {
        somme += candidat.pourcentage[i];
    }
    return somme;
}

/// @fn Candidat_mention chercher_vainqueur_jugement(Candidat_mention *candidats, int nb_candidats)
/// @brief Trouve le vainqueur du jugement majoritaire parmi une liste de candidats.
/// @param[in] candidats Liste des candidats.
/// @param[in] nb_candidats Nombre de candidats.
/// @return Candidat_mention représentant le vainqueur.
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

/// @fn int calculerMediane(Candidat_mention candidat, int nb_electeurs)
/// @brief Calcule la médiane des scores pour un candidat.
/// @param[in] candidat Candidat dont on calcule la médiane.
/// @param[in] nb_electeurs Nombre total d'électeurs.
/// @return Médiane des scores.
int calculerMediane(Candidat_mention candidat, int nb_electeurs) {
    int somme = 0;
    int i=0;
    do {
        somme += candidat.score[i];
        i++;
    } while (somme<nb_electeurs/2);
    return i-1;
}

/// @fn void jugement_majoritaire(char *nom_fichier)
/// @brief Exécute le jugement majoritaire pour déterminer le vainqueur d'une élection.
/// @param[in] nom_fichier Chemin vers le fichier contenant les données de l'élection.
/// @param[in] log fichier log
void jugement_majoritaire(char * nom_fichier,const char * log) {
    int nb_electeurs = 0;
    int nb_candidats = 0;
    compter_lignes_colonnes_csv(nom_fichier, &nb_electeurs, &nb_candidats);
    Candidat_mention * candidats;
    
    lecture_csv_jugement(nom_fichier, &candidats, nb_candidats, nb_electeurs);
    afficher_score_jugement(candidats, nb_candidats,log);

    // remplir les pourcentages
    fprintf(log,"LES POURCENTAGES POUR CHAQUE CANDIDAT\n\n");
    fprintf(log, "%-25s | %-10s\n", "Noms", "Pourcentages");

    for (int j=0; j<nb_candidats; j++){
        for (int i=0; i<6; i++) {
            candidats[j].pourcentage[i] = 100.0*(candidats[j].score[i] /(float) nb_electeurs);
            fprintf(log,"nom : %-25s\t\tpourcentage : %.2f%%\n",candidats[j].nom,candidats[j].pourcentage[i]);
        }
        
        
        candidats[j].mediane = calculerMediane(candidats[j], nb_electeurs);
    }
    
    fprintf(log,"MEDIANE POUR CHAQUE CANDIDAT :\n\n");
    for (int k=0; k<nb_candidats; k++){
            fprintf(log," %s : %d\n",candidats[k].nom,candidats[k].mediane);
        } 

    Candidat_mention vainqueur = chercher_vainqueur_jugement(candidats, nb_candidats);

    printf("Mode de scrutin : jugement majoritaire, %d candidats, %d votants\n", nb_candidats, nb_electeurs);
    printf("\tVainqueur = %s mention : %d (%.2f%%)\n", vainqueur.nom, vainqueur.mediane, vainqueur.pourcentage[vainqueur.mediane]);
    free(candidats);
}
