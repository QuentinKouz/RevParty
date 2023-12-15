/// \file structures.h
/// \brief Fichier de déclaration des structures utilisées dans le programme.
/// \author Equipe-2 : Salim Moussaoui, Maxime Cherraf, Quentin Kouzmitch, Nicolas Giry
/// \date 28 octobre 2023

#ifndef UTILS_SD_H
#define UTILS_SD_H

#include <stdbool.h>

/// \def LONGUEUR_JUGEMENT
#define LONGUEUR_JUGEMENT 24

/// \def LONGUEUR_RESULTAT_JUGEMENT
#define LONGUEUR_RESULTAT_JUGEMENT 25

/// \def LONGUEUR_CONDORCET
#define LONGUEUR_CONDORCET 38

/// \def LONGUEUR_RESULTAT_CONDORCET
#define LONGUEUR_RESULTAT_CONDORCET 39

/// \def MAX_LINE_LENGTH
/// \brief Longueur maximale d'une ligne dans un fichier.
#define MAX_LINE_LENGTH 1024

/// \def MAX_VOTANTS
/// \brief Nombre maximal de votants.
#define MAX_VOTANTS 100

/// \def MAX_CANDIDATS
/// \brief Nombre maximal de candidats.
#define MAX_CANDIDATS 10

/// \def MAX_VOTES
/// \brief Nombre maximal de votes.
#define MAX_VOTES 100

/// \def LIGNES
/// \brief Nombre maximal de lignes.
#define LIGNES 100

/// \def COLONNES
/// \brief Nombre maximal de colonnes.
#define COLONNES 100

/// \struct votant
/// \brief Structure représentant un votant.
/// Un votant est défini par son numéro étudiant et une clé.
typedef struct {
    int num_etu;
    char key[30];
} votant;

/// \struct vote
/// \brief Structure représentant un vote.
///
/// Un vote est défini par sa réponse, la date du vote, le cours associé, un hachage,
/// et le détail des votes par index de burger.
typedef struct {
    int reponse;
    char date[20];
    char cours[50];
    char hashed[64];
    int votes[10]; // votes [index burger]
} vote;



/************************************----------Structures de données----------*****************************************/

/// @struct Candidat
/// @brief Structure représentant un candidat dans le vote.
/// @var Candidat nom
/// Nom du candidat.
/// @var Candidat indice
/// Indice du candidat
/// @var Candidat score
/// Score total du candidat.
typedef struct {
    int indice;
    char nom[50];
    int score;
} Candidat;

typedef struct {
    int indice;
    char nom[50];
    int score[6];
    float pourcentage[6];
    int mediane;
} Candidat_mention;

// Structure pour stocker la matrice de chaînes de caractères dynamique
typedef struct {
    char ***vote;  // Tableau de pointeurs vers des chaînes de caractères
    int ligne;     // Nombre de lignes
    int colonne;   // Nombre de colonnes
} t_mat_char_star_dyn;

typedef struct {
    int **donnees;
    int lignes;
    int colonnes;
} MatriceDuel;

typedef struct Arc {
    int sommetDepart;
    int sommetArrivee;
    int poids;
    struct Arc *suivant;
} Arc;

typedef struct ListeArcs{
    Arc *debut;
    unsigned taille;
    struct ListeArcs * suivant;
    int poids;
} ListeArcs;

typedef struct {
    ListeArcs * debut;
    ListeArcs * courant;
    int taille;
} ListeDeListeArcs;

/*************************************************Méthodes**********************************************************/

const char* obtenirNomCandidat(int indice);

void initMatrice(t_mat_char_star_dyn *matrice, int ligne, int colonne);

MatriceDuel creerMatriceDuel(int lignes, int colonnes);

void remplirMatriceDuel(const t_mat_char_star_dyn *matriceVote, MatriceDuel *matriceDuels);

ListeArcs creerListeArcs();

ListeDeListeArcs creerListeDeListeArcs();

bool estDansGraphe(ListeArcs* liste, int sommet);

void ajouterListeArc (ListeDeListeArcs* listeDeListe, ListeArcs* liste);

void ajouterArc(ListeArcs *liste, const Arc *arc);

void retirerArc(ListeArcs* liste, Arc *arc);

void ajouterArcSommet(ListeArcs* graphe, ListeArcs* arcsAVerifier, int sommet);

void ajouterArcDepuisMatrice(ListeArcs *liste, int sommetDepart, int sommetArrivee, MatriceDuel * matriceDuels);

void creerListeArcsDepuisMatrice(MatriceDuel matriceDuels, int nbCandidats, ListeArcs * liste);

void freeMatrice(t_mat_char_star_dyn *matrice);

void libererMatriceDuel(MatriceDuel *matrice);

void libererListeArcs(ListeArcs *liste);

void afficherMatriceDuelAvecNoms(const MatriceDuel *matrice, int nbCandidats);

void afficherListeArcs(const ListeArcs *liste);

void afficherListeDeListeArcs (ListeDeListeArcs * liste);

int weight_at(ListeDeListeArcs* liste, int ind);


#endif // UTILS_SD_H
