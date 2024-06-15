#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>

#include "./csv.h"

void write_int_array_CSV(const char * filename, int * array, int size)
{
    // Création du répertoire ./out s'il n'existe pas
    struct stat st = {0};
    if (stat("./out", &st) == -1) {
        if (mkdir("./out", 0700) != 0) {
            perror("Error creating directory");
            return;
        }
    }

    // Construction du chemin complet du fichier
    char filepath[100];
    snprintf(filepath, sizeof(filepath), "./out/%s", filename);

    FILE *file = fopen(filepath, "w");
    if (file == NULL) {
        perror("Error opening file");
        return;
    }
    
    // Écriture
    for (int i = 0; i < size; i++) {
        fprintf(file, "%d", array[i]);
        if (i < size - 1) {
            fprintf(file, ",");
        }
    }
    
    fclose(file);
}


void write_double_matrix_CSV(const char *filename, double **array, int size_x, int size_y)
{
    // Création du répertoire ./out s'il n'existe pas
    struct stat st = {0};
    if (stat("./out", &st) == -1) {
        if (mkdir("./out", 0700) != 0) {
            perror("Error creating directory");
            return;
        }
    }

    // Construction du chemin complet du fichier
    char filepath[256];
    snprintf(filepath, sizeof(filepath), "./out/%s", filename);

    FILE *file = fopen(filepath, "w");
    if (file == NULL) {
        perror("Error opening file");
        return;
    }
    
    // Écriture
    for (int i = 0; i < size_x; i++) {
        for (int j = 0; j < size_y; j++) {
            fprintf(file, "%f", array[i][j]);
            if (j < size_y - 1) {
                fprintf(file, ",");
            }
        }
        fprintf(file, "\n");
    }
    
    fclose(file);
}
