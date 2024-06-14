#ifndef RHINITE_H
#define RHINITE_H

// Structures
typedef enum {SAIN, INCUBANT, CONTAGIEUX, GUERI} etat_t;

typedef struct {
    int x, y;
    etat_t etat;
    int jour_infection;
} personne_t;

//Fonctions

personne_t * init_population(int num_personnes, int taille_grille);

void init_contamination(personne_t * population, int nb_initial_contamines, int num_personnes);

int * launch_sim(int nb_jours, personne_t * population, int taille_grille, int num_personnes, int duree_incubation, int duree_contagion, int duree_imunitee, double * proba_contamination);

#endif
