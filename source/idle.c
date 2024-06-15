#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../packages/menuing/menuing.h"
#include "../packages/mt19937ar/mt19937ar.h"
#include "./rhinite.h"

#define BORDURE "#"


// Fonction de comparaison pour qsort
int comparer_personnes(const void *a, const void *b) {
    personne_t *personneA = (personne_t *)a;
    personne_t *personneB = (personne_t *)b;
    
    // Comparer les coordonnées x puis y pour une lecture ligne par ligne
    if (personneA->x != personneB->x) {
        return personneA->x - personneB->x;
    } else {
        return personneA->y - personneB->y;
    }
}

personne_t * tri_croiss_population(personne_t *population, int num_personnes) {
    // Allouer de la mémoire pour une copie du tableau
    personne_t *copie_population = malloc(num_personnes * sizeof(personne_t));
    if (copie_population == NULL) {
        perror("Erreur d'allocation de mémoire");
        exit(EXIT_FAILURE);
    }
    
    // Copier les éléments du tableau original dans la copie
    for (int i = 0; i < num_personnes; i++) {
        copie_population[i] = population[i];
    }
    
    // Trier la copie du tableau en utilisant qsort et la fonction de comparaison
    qsort(copie_population, num_personnes, sizeof(personne_t), comparer_personnes);
    
    return copie_population;
}

void affich_population(int taille_grille, personne_t * population, int num_personnes)
{
    personne_t * population_ordone = tri_croiss_population(population, num_personnes);
    int c = 0; // Curseur de lecture du tableau ordoné

    //Affichage bordure supérieure
    for (int i = 0; i < taille_grille + 2; i++)
    {
        printf(BORDURE);
    }
    printf("\n");


    for (int i = 0; i < taille_grille; i++)
    {
        printf(BORDURE);
        for (int j = 0; j < taille_grille; j++)
        {
            if (i == population_ordone[c].x && j == population_ordone[c].y)
            {
                printf("%d",population_ordone[c].etat);
                c++;
            } else {
                printf(" ");
            }
        }
        printf(BORDURE);
        printf("\n");
    }

    //Affichage bordure inférieure
    for (int i = 0; i < taille_grille + 2; i++)
    {
        printf(BORDURE);
    }
    printf("\n");

    free(population_ordone);
}

int * launch_idle(int nb_jours, int taille_grille, int num_personnes, int duree_incubation, int duree_contagion, int duree_imunitee, double * proba_contamination, int num_infect_init)
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
        clear();

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

            // Affichage population
            printf("Jour %d, heure %d\n\n", j+1, h);
            affich_population(taille_grille, population, num_personnes);
            printf("\n0 = SAIN");
            printf("\n1 = INCUBANT");
            printf("\n2 = CONTAGIEUX");
            printf("\n3 = GUERI\n");

            get_pause_input();
        }
        nb_contamination[j] = nb_contamination_jour;
        evolution_journaliere_maladie(population, num_personnes, duree_incubation, duree_contagion, duree_imunitee);
    }

    free(population);
    return nb_contamination;
}

int main() 
{
    // Initialisation du générateur de pseudo-aléatoire
    unsigned long init[4]={0x123, 0x234, 0x345, 0x456}, length=4;
    init_by_array(init, length);

    // Déclarations variables simulation
    double        proba_contamination[12] = {0, 0, 0, 0.6, 0.8, 0.7, 0.6, 0.5, 0.4, 0.3, 0.2, 0.1};
    int           jours;

    int           taille_grille;
    int           num_personnes;
    int           duree_incubation; // en jours
    int           duree_contagion; // en jours
    int           duree_imunitee; // en jours

    int           num_infect_init;


    // Menu principal
    accepted_inputs * princ_ai = init_AI(3, 1);
    add_input(princ_ai, 0, "1", "Valeurs par défaut");
    add_input(princ_ai, 1, "2", "Valeurs personnalisées");
    add_input(princ_ai, 2, "q", "Quitter");
    int conti = 1;

    while (conti) 
    {
        // Affichage menu
        printf("sim-rhinite : Mode Idle\n");
        affich_menu(princ_ai, 1, 1);

        // Récupération input user
        char * input = get_user_input(princ_ai);

        if (!strcmp(input,"1"))
        {
            // Initialisation variables simulation
            jours             = 10;
            taille_grille     = 40;
            num_personnes     = 50;
            duree_incubation  = 2;
            duree_contagion   = 9;
            duree_imunitee    = 15;
            num_infect_init   = 5;

            // Lancement idle avec valeurs par défaut
            int * contaminations = launch_idle(jours, taille_grille, num_personnes, duree_incubation, duree_contagion, duree_imunitee, proba_contamination, num_infect_init);

            // Affichage final avec les statistiques de contaminations
            printf("Nombre de contaminations par jour :\n");
            for (int j = 0; j < jours; j++)
            {
                printf("Jour %d : %d\n", j+1, contaminations[j]);
            }
            get_pause_input();

        }
        else if (!strcmp(input,"2"))
        {
            printf("WIP\n");
            get_pause_input();
        }
        else if (!strcmp(input,"q"))
        {
            conti = 0;
        }
    }
    

}
