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

void deplacement_alea(personne_t * population, int num_personnes, int taille_grille);

#endif
