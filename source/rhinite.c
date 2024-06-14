#include <stdio.h>
#include <stdlib.h>

#include "../packages/menuing/menuing.h"
#include "../packages/mt19937ar/mt19937ar.h"

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

void deplacement_alea(personne_t * population, int num_personnes, int taille_grille)
{
    for (int i = 0; i < num_personnes; i++) {
        int direction = genrand_int32() % 4;
        switch (direction) {
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

void contamination(personne_t * population, int num_personnes, int taille_grille, double * proba_contamination)
{
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
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}

int main() 
{
    // Initialisation du générateur de pseudo-aléatoire
    unsigned long init[4]={0x123, 0x234, 0x345, 0x456}, length=4;
    init_by_array(init, length);

    // Déclarations variables état programme
    personne_t *  population;
    int           taille_grille;
    int           num_personnes;
    int           num_infect_init;

    int           duree_incubation; // en jours
    int           duree_contagion; // en jours
    int           duree_imunitee; // en jours

    double        proba_contamination[12] = {0, 0, 0, 0.6, 0.8, 0.7, 0.6, 0.5, 0.4, 0.3, 0.2, 0.1};
    

    // Initialisation variables état programme
    taille_grille = 50;
    num_personnes = 10;
    num_infect_init = 1;

    duree_incubation = 2;
    duree_contagion = 9;
    duree_imunitee = 20;
   
    population = init_population(num_personnes, taille_grille);
    init_contamination(population, num_infect_init, num_personnes);


    // Cycle de 40 jours
    for (int j = 1; j < 40; j++) 
    {  
        // Afficher les individus pour apprécier l'évolution de la maladie
        printf("Jour %d :\n", j);
        for (int i = 0; i < num_personnes; i++) {
            printf("Personne %d: (%d, %d) => État : %d (j=%d)\n", i, population[i].x, population[i].y, population[i].etat, population[i].jour_infection);
        }
        printf("\n");

        // Cycle de journée (24h)
        for (int h = 1; h <= 24; h++)
        {   
            contamination(population, num_personnes, taille_grille, proba_contamination);

            // Horaires d'activité [6h ; 22h]
            if (h >= 6 && h <= 22)
            {
                deplacement_alea(population, num_personnes, taille_grille);
            }
            
        }

        evolution_journaliere_maladie(population, num_personnes, duree_incubation, duree_contagion, duree_imunitee);
    }

    return 0;
}
