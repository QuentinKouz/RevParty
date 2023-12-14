/// \file lecture_csv.h
/// \brief Fichier de déclaration des structures utilisées dans le programme.
/// \author Equipe-2 : Salim Moussaoui, Maxime Cherraf, Quentin Kouzmitch, Nicolas Giry
/// \date 28 octobre 2023

#ifndef LECTURE_CSV_H
#define LECTURE_CSV_H

#include "utils_sd.h"

void lecture_csv_verifyMyVote(char* nom_fichier, votant* votants[MAX_VOTANTS], vote* votes[MAX_VOTES]); // changer le nom le fichier verify_my_vote

void lecture_csv_score_condorcet(const char *nom_fichier, t_mat_char_star_dyn *matrice, int ligne, int colonne);



#endif // LECTURE_CSV_H
