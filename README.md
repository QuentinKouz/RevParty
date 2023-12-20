# RevParty
Projet avancé sur les élections
Groupe 2: Nicolas Giry, Quentin Kouzmitch, Salim Moussaoui, Maxime Cherraf

Ce programme propose l'execution de diverses méthodes de scrutin (uninominale à un et deux tours, Condorcet + méthode des paires / méthodes du minimax / méthode de Schulzes, jugement majoritaire).
Pour tester ces méthodes nous avons 3 fichiers csv à disposition (VoteCondorcet.csv pour les méthodes de Condorcet, testCondorcetSansVainqueur.csv pour les méthodes de condorcet en provoquant un paradoxe, voteJugement.csv pour la méthode du jugement majoritaire, chaque fichier pouvant être utilisé pour toutes les méthodes).

Pour executer le programme :

../Executables/main -m all -o log.txt

Cela va tester toutes les méthodes et afficher les résultats.
Si vous souhaitez accéder aux calculs intermédiaires, ils seront alors disponibles dans le fichier log.txt grâce à la balise -o 

Pour plus de modularité, vous pouvez tester une méthode en particulier avec un fichier au choix :

../Executables/main -i chemin_fichier.csv -o log.txt -m uni1/uni2/jm

ou 

../Executables/main -d chemin_fichier.csv -o log.txt -m cp/cm/cs

uni1, uni2, jm, cp, cm, cs indiquent respectivement que vous souhaitez utiliser la méthode uninominale à un tour, à deux tours, jugement majoritaire, Condorcet paires, Condorcet minimax ou Condorcet Schulzes.

Les balises -i et -d indiquent si la méthode à utiliser, nécessitera des matrices de duels ou non (-d indique qu'elle en aura besoin).
La balise -o est facultative, et permet d'inscrire les calculs dans un fichier de log.
La balise -m précède la méthode de bulletin que vous souhaitez utiliser.

De plus, si vous avez participé aux votes, vous pouvez verifier vos votes de manière totalement anonymisé grâce au Sha256 et au programme verify_my_vote.

Pour exécuter le programme verify_my_vote :

make verify_my_vote

Puis : 

./verify_my_vote <num_etudiant> <clé secrète>

Il faudra ensuite renseigner le nom en majuscule et le prénom avec la première lettre en majuscule.
