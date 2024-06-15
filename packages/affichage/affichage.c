#include <stdio.h>

void affich_matrice_double(double ** tableau, int lignes, int colonnes) 
{
    for (int i = 0; i < lignes; i++) {
        for (int j = 0; j < colonnes; j++) {
            printf("%lf ", tableau[i][j]);
        }
        printf("\n");
    }
}

void affich_matrice_int(int ** tableau, int lignes, int colonnes) 
{
    for (int i = 0; i < lignes; i++) {
        for (int j = 0; j < colonnes; j++) {
            printf("%d ", tableau[i][j]);
        }
        printf("\n");
    }
}

/* ------------------------------------------------------------------------------------ */
/* affich_tab_double                                     affiche un tableau de n double */
/*                                                                                      */
/* Entrées :                                                                            */
/*     - t : tableau de flotants doubles                                                */
/*     - n : taille du tableau                                                          */
/*                                                                                      */
/* Effet de bord : affiche le tableau dans la sortie standard                           */
/* ------------------------------------------------------------------------------------ */

void affich_tab_double(double * t, int n)
{
    for (int i=0;i<n;i++){
        // Affiche le i-ème flotant du tableau 
        printf("%f ",t[i]); 
    }
    // Retour à la ligne
    printf("\n"); 
}