#include <stdio.h>
#include <stdlib.h>

void write_int_array_CSV(const char *filename, int *array, int size)
{
    FILE *file = fopen(filename, "w");
    
    if (file == NULL) {
        perror("Erreur ouverture fichier");
        return;
    }
    
    for (int i = 0; i < size; i++) {
        fprintf(file, "%d", array[i]);
        if (i < size - 1) {
            fprintf(file, ",");
        }
    }
    
    fclose(file);
}