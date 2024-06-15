#ifndef RHINITE_H
#define RHINITE_H

// Structures
typedef enum {SAIN, INCUBANT, CONTAGIEUX, GUERI} etat_t;

typedef struct {
    int x, y;
    etat_t etat;
    int jour_infection;
} personne_t;


// Fonctions Principales
personne_t * init_population(int num_personnes, int taille_grille);
void init_contamination(personne_t * population, int nb_initial_contamines, int num_personnes);
int * launch_sim(int nb_jours, int taille_grille, int num_personnes, int duree_incubation, int duree_contagion, int duree_imunitee, double * proba_contamination, int num_infect_init);

// Fonctions métier
void deplacement_alea(personne_t * population, int num_personnes, int taille_grille);
void evolution_journaliere_maladie(personne_t * population, int num_personnes, int duree_incubation, int duree_contagion, int duree_imunitee);
int contamination(personne_t * population, int num_personnes, int taille_grille, double * proba_contamination);


#endif
