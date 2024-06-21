#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "../packages/mt19937ar/mt19937ar.h"
#include "../packages/csv/csv.h"
#include "./rhinite.h"

int main() 
{
    printf("sim-rhinite : Mode Statistiques\n");

    // Initialisation du générateur de pseudo-aléatoire
    unsigned long init[4]={0x123, 0x234, 0x345, 0x456}, length=4;
    init_by_array(init, length);


    // Déclaration constantes programme
    double        proba_contamination[12]   = {0, 0, 0, 0.6, 0.8, 0.7, 0.6, 0.5, 0.4, 0.3, 0.2, 0.1};
    int           duree_incubation          = 2; // en jours
    int           duree_contagion           = 9; // en jours
    int           jours                     = 365; // un an de simulation
    int           nb_replications           = 30;

    // Déclarations variables programme (constantes expérience)
    int           taille_grille;
    int           num_personnes;
    int           duree_immunite; // en jours

    // Déclaration variable programme (variable expérience)
    int           num_infect_init;
    
    // Initialisation constantes expérience
    taille_grille     = 50;
    num_personnes     = 100;
    duree_immunite    = 10;


    // Pour chaque valeur de notre variable d'experience : [1,2,...,10]
    for (num_infect_init = 1; num_infect_init <= 10; num_infect_init++)
    {
        printf("Simulation w/ num_infect_init=%d :",num_infect_init);
        fflush(stdout);

        // Statistiques sur la propagation de la maladie sur un an
        double **   result_final = malloc(jours * sizeof(double*));
        if (result_final == NULL) {
            perror("Erreur allocation result_final");
            exit(1);
        }

        // Stockage des réplications
        int ** temp_stat_contagion = malloc(nb_replications * sizeof(int*));
        if (temp_stat_contagion == NULL) {
            perror("Erreur allocation temp_stat_contagion");
            exit(1);
        }


        // Pour le nombre de réplications
        for (int i = 0; i < nb_replications; i++)
        {
            // Lancement de la simulation, récupération du nombre de contaminations par jour sur un an
            temp_stat_contagion[i] = launch_sim(jours, taille_grille, num_personnes, duree_incubation, duree_contagion, duree_immunite, proba_contamination, num_infect_init);
            printf("|");
            fflush(stdout);
        }
        printf(" Done !\n");
        
        for (int j = 0; j < jours; j++)
        {
            // Calcul de la moyenne 
            double moyenne = 0;
            for (int i = 0; i < nb_replications; i++)
            {
                moyenne += temp_stat_contagion[i][j];
            }
            moyenne /= nb_replications;
        
            // Calcul de l'écart-type
            double ecart_type = 0;
            for (int i = 0; i < nb_replications; i++)
            {
                ecart_type += pow((temp_stat_contagion[i][j] - moyenne),2);
            } 
            ecart_type = sqrt((ecart_type / (nb_replications - 1)));

            // Calcul de la marge d'erreur
            double marge_erreur = 1.96 * (ecart_type /sqrt(nb_replications));

            // Calcul intervalle de confiance
            result_final[j] = malloc(2 * sizeof(double));
            result_final[j][0] = moyenne - marge_erreur;
            result_final[j][1] = moyenne + marge_erreur;
        }


        // Écriture du resultat final dans un fichier csv
        char nom_fichier[50];
        snprintf(nom_fichier, sizeof(nom_fichier), "num_infect_init_%d.csv", num_infect_init);
        write_double_matrix_CSV(nom_fichier, result_final, jours, 2);


        // Libération
        for (int j = 0; j < jours; j++)
        {
            free(result_final[j]);
        }
        free(result_final);

        for (int i = 0; i < nb_replications; i++)
        {
            free(temp_stat_contagion[i]);
        }
        free(temp_stat_contagion);
        
    }
    
    return 0;
}