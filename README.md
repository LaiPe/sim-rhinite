# sim-rhinite
**sim-rhinite** est un projet de simulation de maladie infectieuse type rhinite.
Il est composé de deux programmes :

 - **stats_sim** : Il permet de produire des statistiques sur l'évolution de la contamination de cette maladie en fonction du nombre d'infectés initiaux.
- **idle** : Annexe permettant de visualiser le comportement de la population simulée.

## Compilation et exécution
Le code source du programme est fourni avec un constructeur (`makefile`) qui se chargera de la compilation des fichiers sources en exécutable. 

Veillez à ce que `gcc` soit installé sur votre machine, puis placez une instance Bash dans le répertoire racine du programme et exécutez y la commande :
```
$ make
```
Ensuite, vous pouvez exécuter l'un des deux programmes avec :
```
$ ./<nom_executable>
```

