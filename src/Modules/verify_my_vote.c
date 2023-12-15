/// \file verify_my_vote.c
/// \brief Programme de vérification des votes.
/// \author Equipe-2 : Salim Moussaoui, Maxime Cherraf, Quentin Kouzmitch, Nicolas Giry
/// \date 28 octobre 2023

#include "sha256.h"
#include "sha256_utils.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "utils_sd.h"
#include "lecture_csv.h"


/// \def AUCUN
/// \brief Constante représentant l'absence de vote
#define AUCUN 0

/// \def JUGEMENT
/// \brief Constante représentant le vote par jugement.
#define JUGEMENT 1

/// \def CONDORCET
/// \brief Constante représentant le vote par la méthode Condorcet.
#define CONDORCET 2


/// \brief Vérifie si la personne est bien inscrite dans le fichier des électeurs
///
/// Cette fonction vérifie si le numéro étudiant est bien dans le fichier ainsi que la clé
/// Puis vérifie si la clé et le numéro étudiant sont sur la même ligne
/// \param[in] fichier : Le nom du fichier contenant les électeurs.
/// \param[in] longueur : Le nombre de lignes dans le fichier.
/// \param[in] votants : Le tableau des votants.
/// \param[in] numero_etudiant : Le numéro de l'étudiant à vérifier.
/// \param[in] key : La clé de l'étudiant.
/// \param[in] methode : La méthode de vote (JUGEMENT ou CONDORCET).
/// \return Retourne la méthode de vote si le votant est valide, sinon retourne AUCUN.
int verifier_votant(char fichier[], int longueur, votant * votants[], int numero_etudiant, char key[], int methode){
  lecture_csv_verifyMyVote(fichier, votants, NULL);
  for (int ligne = 0; ligne < longueur; ligne++){
    if (((votants[ligne])->num_etu == numero_etudiant)) {
      if (strcmp(key,votants[ligne]->key) == 0){
        return methode;
      }
    }
  }
  return AUCUN;
}


/// \brief Crée un hash à partir du nom, prénom et clé d'un votant.
/// \param[in] nom : Le nom du votant.
/// \param[in] prenom : Le prénom du votant.
/// \param[in] key : La clé du votant.
/// \param[out] hash : Le hash SHA-256 créé.
void creerHash(char *nom, char *prenom, char *key, char *hash){
    // conversion de la clé en binaire
    strcat(nom, " ");
    strcat(nom, prenom);
    strcat(nom, key);
    sha256ofString(nom, hash);
}

/// \brief Fonction principale du programme.
/// \param[in] argc : Le nombre d'arguments.
/// \param[in] argv : Le tableau des arguments.
/// \return 0 si le programme se termine avec succès, sinon un code d'erreur.
int main(int argc, char * argv[]) {
    SHA256_CTX sha;
    int len_data;
    BYTE *data = (char* )malloc(sizeof(strlen(argv[2]) * sizeof(char))); // clé
    char hash[SHA256_BLOCK_SIZE*2 + 1];
    char fichier_votant_jugement[] = "../ResultatsVote/codeJugementNumEtu.csv";
    char fichier_votant_condorcet[] = "../ResultatsVote/codeCondorcetNumEtu.csv";
    votant* votant_jugement[LONGUEUR_JUGEMENT];
    votant* votant_condorcet[LONGUEUR_CONDORCET];
    int type_vote = AUCUN;
    char fichier_resultat_jugement[] = "../ResultatsVote/voteJugement.csv";
    char fichier_resultat_condorcet[] = "../ResultatsVote/VoteCondorcet.csv";
    int nb_lignes_vote;
    char * fichier;


    // Vérification du bon nombre d'arguments (Gestion d'erreur à l'appel)
    if (argc < 3) {
        fprintf(stderr, "Usage: ./verify_my_vote <numéro étudiant> <code secret>\n");
        exit(1);
    }

    int numero_etudiant = atoi(argv[1]);
    char* key = argv[2];
    char nom[100];
    char prenom[100];

    printf("Quel est votre nom (en MAJUSCULE) : ");
    scanf("%s", &nom);
    printf("Quel est votre prenom (MAJUSCULE premiere lettre seulement) : ");
    scanf("%s", &prenom);
    printf("Bonjour %s %s !\n", prenom, nom);

    // allocation mémoire des tableaux votants
    for (int i = 0; i < LONGUEUR_JUGEMENT; i++){
      votant_jugement[i] = malloc(sizeof(votant));
    }
    for (int i = 0; i < LONGUEUR_CONDORCET; i++){
      votant_condorcet[i] = malloc(sizeof(votant));
    }

    // On vérifie si la personne est dans la liste des votants de Jugement
    type_vote += verifier_votant(fichier_votant_jugement,LONGUEUR_JUGEMENT, votant_jugement, numero_etudiant, key, JUGEMENT);
    // On vérifie si la personne est dans la liste des votants de Condorcet
    type_vote += verifier_votant(fichier_votant_condorcet,LONGUEUR_CONDORCET, votant_condorcet, numero_etudiant, key, CONDORCET);

    // Si la personne n'est dans aucune liste de vote on sort
    if (type_vote == AUCUN){
      fprintf(stderr, "%s %s n'est présent dans aucune liste.\n", argv[1], argv[2]);
      exit(1);
    }

    // on crée la structure des votes en fonction du type de vote puis on l'initialise
    switch (type_vote){
      case JUGEMENT:
        nb_lignes_vote = LONGUEUR_RESULTAT_JUGEMENT;
        fichier = fichier_resultat_jugement;
        break;
      case CONDORCET:
        nb_lignes_vote = LONGUEUR_RESULTAT_CONDORCET;
        fichier = fichier_resultat_condorcet;
        break;
      default:
        fprintf(stderr, "Il y a une erreur lors de la vérification\n");
        exit(1);
    }

    // allocation mémoire du tableau votes
    vote* votes[nb_lignes_vote];
    for (int i = 0; i < nb_lignes_vote; i++){
      votes[i] = malloc(sizeof(vote));
    }

    // on remplit le tableau avec les informations du fichier csv correspondant
    lecture_csv_verifyMyVote(fichier, NULL, votes);

    // on crée le hash depuis le nom, le prénom et la clé du votant
    creerHash(nom, prenom, key, hash);

    // On regarde dans les votes à quelle ligne se situe le vote du votant et on sort
    int ligne_resultat_vote =0;
    while (ligne_resultat_vote<nb_lignes_vote && strcmp(votes[ligne_resultat_vote]->hashed, hash) != 0) {
        ligne_resultat_vote++;
    }
    if (strcmp(votes[ligne_resultat_vote]->hashed, hash) != 0){
      fprintf(stderr, "Nous sommes navrés, nous ne trouvons pas votre vote...\nÊtes-vous sûr d'avoir rentré le bon nom et prenom selon le format demandé ?\n");
      exit(1);
    }

    char *burgers[] = {
      "Burger Black Pepper",
      "Burger Sud-Ouest",
      "Thai Burger",
      "Veggie Burger",
      "Fire cracker",
      "Roma",
      "Crispy",
      "Cheese Burger",
      "Burger surprise",
      "Country"
    };

    // Affichage du vote
    if (type_vote == JUGEMENT) {
      printf("Votre vote se situe à la ligne %d du fichier voteJugement.csv.\n", ligne_resultat_vote+1);
      for(int i = 0; i < 10; i++) {
          printf("Vote n°%d : - %s : (%d)\n", i+1, burgers[i], votes[ligne_resultat_vote]->votes[i]);
        }
    }
    else if (type_vote == CONDORCET) {
      printf("Votre vote se situe à la ligne %d du fichier VoteCondorcet.csv.\n", ligne_resultat_vote+1);
      for(int i = 0; i < 10; i++) {
          printf("Vote n°%d : - %s : (%d)\n", i+1, burgers[i], votes[ligne_resultat_vote]->votes[i]);
      }
    }
    else {
      printf("Il y a une erreur lors de la vérification\n");
      exit(3);
    }

    // Libération de la mémoire
    for (int i = 0; i < LONGUEUR_JUGEMENT; i++){
        free(votant_jugement[i]);
    }
    for (int i = 0; i < LONGUEUR_CONDORCET; i++){
        free(votant_condorcet[i]);
    }
    for (int i = 0; i < nb_lignes_vote; i++){
        free(votes[i]);
    }

    return 0;
}
