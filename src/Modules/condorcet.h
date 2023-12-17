/// \file condorcet.h
/// \brief Fichier de déclaration de la méthode de scrutin condorcet.
/// \author Equipe-2 : Salim Moussaoui, Maxime Cherraf, Quentin Kouzmitch, Nicolas Giry
/// \date 15 decembre 2023

#ifndef CONDORCET_H
#define CONDORCET_H

#include "utils_sd.h"

void trierGraphe(ListeArcs * graphe);

void condorcet(const char * fichier, int methodeParadoxe);

#endif // CONDORCET_SCHULZES_H