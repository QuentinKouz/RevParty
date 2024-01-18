# RevParty
Projet avancé sur les élections
Groupe 2: Nicolas Giry, Quentin Kouzmitch, Salim Moussaoui, Maxime Cherraf

Ce programme propose l'execution de diverses méthodes de scrutin (uninominale à un et deux tours, Condorcet + méthode des paires / méthodes du minimax / méthode de Schulzes, jugement majoritaire).
Pour tester ces méthodes nous avons 3 fichiers csv à disposition (VoteCondorcet.csv pour les méthodes de Condorcet, testCondorcetSansVainqueur.csv pour les méthodes de condorcet en provoquant un paradoxe, voteJugement.csv pour la méthode du jugement majoritaire, chaque fichier pouvant être utilisé pour toutes les méthodes).

Pour executer le programme :

../Executables/main -m all -o log.txt

Cela va tester toutes les méthodes et afficher les résultats.

![Screenshot 2024-01-18 110627](https://github.com/QuentinKouz/RevParty/assets/114723956/28c9e6df-c07a-4b53-9fe4-544012854dd9)

Si vous souhaitez accéder aux calculs intermédiaires, ils seront alors disponibles dans le fichier log.txt grâce à la balise -o 

![Screenshot 2024-01-18 110705](https://github.com/QuentinKouz/RevParty/assets/114723956/89a641e4-c9ae-4071-bd3a-a98c543db2bc)
![Screenshot 2024-01-18 110734](https://github.com/QuentinKouz/RevParty/assets/114723956/fcf93dd8-15b3-4ff3-b46f-32843bb38d6f)
![Screenshot 2024-01-18 110803](https://github.com/QuentinKouz/RevParty/assets/114723956/8f6ffb09-a299-48b2-b4bf-996a0ec33557)

Pour plus de modularité, vous pouvez tester une méthode en particulier avec un fichier au choix :

../Executables/main -i chemin_fichier.csv -o log.txt -m uni1/uni2/jm

ou 

../Executables/main -d chemin_fichier.csv -o log.txt -m cp/cm/cs

uni1, uni2, jm, cp, cm, cs indiquent respectivement que vous souhaitez utiliser la méthode uninominale à un tour, à deux tours, jugement majoritaire, Condorcet paires, Condorcet minimax ou Condorcet Schulzes.

Les balises -i et -d indiquent si la méthode à utiliser, nécessitera des matrices de duels ou non (-d indique qu'elle en aura besoin).
La balise -o est facultative, et permet d'inscrire les calculs dans un fichier de log.
La balise -m précède la méthode de bulletin que vous souhaitez utiliser.

![Screenshot 2024-01-18 111215](https://github.com/QuentinKouz/RevParty/assets/114723956/d4dd5066-83e1-4c24-b9a0-73468a22ce43)

![Screenshot 2024-01-18 111146](https://github.com/QuentinKouz/RevParty/assets/114723956/1ca8b8f4-f987-45aa-b595-2afb01ae5284)

De plus, si vous avez participé aux votes, vous pouvez verifier vos votes de manière totalement anonymisé grâce au Sha256 et au programme verify_my_vote.

Pour exécuter le programme verify_my_vote :

make verify_my_vote

Puis : 

./verify_my_vote <num_etudiant> <clé secrète>

Il faudra ensuite renseigner le nom en majuscule et le prénom avec la première lettre en majuscule.

![Screenshot 2024-01-18 111411](https://github.com/QuentinKouz/RevParty/assets/114723956/839c9422-69b4-4dcc-add5-15c7e2ebb37d)
