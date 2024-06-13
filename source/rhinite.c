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

int main() 
{
    // Initialisation du générateur de pseudo-aléatoire
    unsigned long init[4]={0x123, 0x234, 0x345, 0x456}, length=4;
    init_by_array(init, length);

    // Déclarations variables état programme
    personne_t *  population;
    int           num_personnes = 10;
    int           taille_grille = 50;

    population = init_population(num_personnes, taille_grille);

    for (int j = 0; j < 3; j++) 
    {  
        // Afficher les coordonnées des individus pour vérifier l'unicité
        for (int i = 0; i < num_personnes; i++) {
            printf("Personne %d: (%d, %d)\n", i, population[i].x, population[i].y);
        }
        printf("\n");

        deplacement_alea(population, num_personnes, taille_grille);
    }

    return 0;
}
