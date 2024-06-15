#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "../packages/mt19937ar/mt19937ar.h"
#include "../packages/affichage/affichage.h"

#include "./rhinite.h"


// Fonction pour mélanger un tableau
void melanger_positions(int ** positions, int n) 
{
    for (int i = n - 1; i > 0; i--) {
        int j = genrand_int32() % (i + 1);
        int temp_x = positions[i][0];
        int temp_y = positions[i][1];
        positions[i][0] = positions[j][0];
        positions[i][1] = positions[j][1];
        positions[j][0] = temp_x;
        positions[j][1] = temp_y;
    }
}

// Fonction pour initialiser la population avec des coordonnées uniques
personne_t * init_population(int num_personnes, int taille_grille) 
{
    if (num_personnes > taille_grille * taille_grille)
    {
        perror("Grille trop petite pour contenir le nombre de personnes");
        exit(1);
    }


    personne_t *  population;
    int **        positions;
    int           index = 0;

    // Allocation population
    population = malloc(num_personnes * sizeof(personne_t));
    if (population == NULL){
        perror("Erreur allocation population");
        exit(1);
    }

    // Allocation positions
    positions = malloc((taille_grille * taille_grille) * sizeof(int *));
    if (positions == NULL){
        perror("Erreur allocation positions");
        exit(1);
    }

    // Générer toutes les positions possibles sur la grille
    for (int x = 0; x < taille_grille; x++) {
        for (int y = 0; y < taille_grille; y++) 
        {
            // Allocation positions[index]
            positions[index] = malloc(2 * sizeof(int));
            if (positions[index] == NULL) {
                fprintf(stderr,"Erreur allocation positions[%d]",index);
                exit(1);
            }

            positions[index][0] = x;
            positions[index][1] = y;
            index++;
        }
    }

    // Mélanger les positions
    melanger_positions(positions, taille_grille * taille_grille);

    // Assigner les positions mélangées à la population
    for (int i = 0; i < num_personnes; i++) {
        population[i].x = positions[i][0];
        population[i].y = positions[i][1];
        population[i].etat = SAIN;
        population[i].jour_infection = 0;
    }

    // Libération positions
    for (int i = 0 ; i < taille_grille * taille_grille ; i++) {
        free(positions[i]);
    }
    free(positions);

    return population;
}


// Fonction pour vérifier si une position est occupée
bool position_occupee(personne_t * population, int num_personnes, int x, int y, int taille_grille) 
{
    for (int i = 0; i < num_personnes; i++) 
    {
        if (population[i].x == x && population[i].y == y) {
            return true;
        }
    }
    return false;
}

void deplacement_alea(personne_t * population, int num_personnes, int taille_grille) {
    for (int i = 0; i < num_personnes; i++) {
        // Liste des directions possibles
        bool directions_possibles[4] = {true, true, true, true}; // Droite, Gauche, Haut, Bas
        int nb_directions_possibles = 4;

        // Vérifier les directions possibles
        int nouvelle_x, nouvelle_y;

        // Droite
        nouvelle_x = (population[i].x + 1) % taille_grille;
        nouvelle_y = population[i].y;
        if (position_occupee(population, num_personnes, nouvelle_x, nouvelle_y, taille_grille)) 
        {
            directions_possibles[0] = false;
            nb_directions_possibles--;
        }

        // Gauche
        nouvelle_x = (population[i].x - 1 + taille_grille) % taille_grille;
        nouvelle_y = population[i].y;
        if (position_occupee(population, num_personnes, nouvelle_x, nouvelle_y, taille_grille)) 
        {
            directions_possibles[1] = false;
            nb_directions_possibles--;
        }

        // Haut
        nouvelle_x = population[i].x;
        nouvelle_y = (population[i].y + 1) % taille_grille;
        if (position_occupee(population, num_personnes, nouvelle_x, nouvelle_y, taille_grille)) 
        {
            directions_possibles[2] = false;
            nb_directions_possibles--;
        }

        // Bas
        nouvelle_x = population[i].x;
        nouvelle_y = (population[i].y - 1 + taille_grille) % taille_grille;
        if (position_occupee(population, num_personnes, nouvelle_x, nouvelle_y, taille_grille)) 
        {
            directions_possibles[3] = false;
            nb_directions_possibles--;
        }

        // S'il n'y a pas de directions possibles, ne pas se déplacer
        if (nb_directions_possibles == 0) 
        {
            continue;
        }

        // Sélectionner une direction parmi les possibles
        int direction_choisie;
        do {
            direction_choisie = genrand_int32() % 4;
        } while (!directions_possibles[direction_choisie]);

    
        // Effectuer le déplacement dans la direction choisie
        switch (direction_choisie) {
            case 0: population[i].x = (population[i].x + 1) % taille_grille; break; // Droite
            case 1: population[i].x = (population[i].x - 1 + taille_grille) % taille_grille; break; // Gauche
            case 2: population[i].y = (population[i].y + 1) % taille_grille; break; // Haut
            case 3: population[i].y = (population[i].y - 1 + taille_grille) % taille_grille; break; // Bas
        }
    }
}

void init_contamination(personne_t * population, int nb_initial_contamines, int num_personnes) 
{
    for (int i = 0; i < nb_initial_contamines; i++) {
        int index = genrand_int32() % num_personnes;
        population[index].etat = INCUBANT;
        population[index].jour_infection = 1;
    }
}

void evolution_journaliere_maladie(personne_t * population, int num_personnes, int duree_incubation, int duree_contagion, int duree_imunitee)
{
    for (int i = 0; i < num_personnes; i++)
    {
        // Incrémentation jours infections
        if (population[i].etat != SAIN)
        {
            population[i].jour_infection++;
        }


        if (population[i].etat == INCUBANT && population[i].jour_infection > duree_incubation) 
        {
            population[i].etat = CONTAGIEUX;
        } 
        else if (population[i].etat == CONTAGIEUX && population[i].jour_infection > (duree_incubation + duree_contagion)) 
        {
            population[i].etat = GUERI;
        } 
        else if (population[i].etat == GUERI && population[i].jour_infection > (duree_incubation + duree_contagion + duree_imunitee))
        {
            population[i].etat = SAIN;
            population[i].jour_infection = 0;
        }  
    }
}

int contamination(personne_t * population, int num_personnes, int taille_grille, double * proba_contamination)
{
    int nb_contamination_heure = 0;

    // Pour chaque personne de la population...
    for (int i = 0; i < num_personnes; i++)
    {
        // ... étant contagieuse.
        if (population[i].etat == CONTAGIEUX) 
        {
            // Vérifier les voisins dans un voisinage de Moore d'ordre 2
            for (int dx = -2; dx <= 2; dx++) {
                for (int dy = -2; dy <= 2; dy++) {
                    if (dx != 0 || dy != 0) 
                    {
                        int nx = (population[i].x + dx + taille_grille) % taille_grille;
                        int ny = (population[i].y + dy + taille_grille) % taille_grille;

                        for (int j = 0; j < num_personnes; j++) 
                        {
                            // Pour chaque voisin sain
                            if (population[j].x == nx && population[j].y == ny && population[j].etat == SAIN) 
                            {
                                // Jet aléatoire en fonction du taux d'infectiosité de la personne contagieuse (i)
                                if (genrand_real1() <= proba_contamination[population[i].jour_infection]) 
                                {
                                    population[j].etat = INCUBANT;
                                    population[j].jour_infection = 1;
                                    nb_contamination_heure++;
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    return nb_contamination_heure;
}

int * launch_sim(int nb_jours, int taille_grille, int num_personnes, int duree_incubation, int duree_contagion, int duree_imunitee, double * proba_contamination, int num_infect_init)
{
    personne_t * population = init_population(num_personnes, taille_grille);
    init_contamination(population, num_infect_init, num_personnes);

    // Statistiques sur l'évolution de la contagion
    int * nb_contamination = malloc(nb_jours * sizeof(int));
    if (nb_contamination == NULL){
        perror("Erreur allocation nb_contamination");
        exit(1);
    }

    for (int j = 0; j < nb_jours; j++) 
    {  
        int nb_contamination_jour = 0;

        // Cycle de journée (24h)
        for (int h = 1; h <= 24; h++)
        {   
            nb_contamination_jour += contamination(population, num_personnes, taille_grille, proba_contamination);

            // Horaires d'activité [6h ; 22h]
            if (h >= 6 && h <= 22)
            {
                deplacement_alea(population, num_personnes, taille_grille);
            }
            
        }

        nb_contamination[j] = nb_contamination_jour;

        evolution_journaliere_maladie(population, num_personnes, duree_incubation, duree_contagion, duree_imunitee);
    }

    free(population);
    return nb_contamination;
}
