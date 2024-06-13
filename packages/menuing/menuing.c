#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "./menuing.h"

void clear() 
{
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

// FONCTIONS DE MENUING

accepted_inputs * init_AI(int num_inputs, int size_max_input)
{   
    // Allocation de la structure
    accepted_inputs * ai = malloc(sizeof(accepted_inputs));
    if (ai == NULL) {
        perror("Erreur d'allocation de la structure accepted_inputs");
        exit(1);
    }

    // Allocation nombre inputs
    ai->num_inputs = num_inputs;  

    // Allocation du array de la sous-structure 'input'
    ai->inputs = malloc(num_inputs * sizeof(input));
    if (ai->inputs == NULL) {
        perror("Erreur d'allocation de l'array inputs");
        free(ai);
        exit(1);
    }

    for (int i = 0; i < num_inputs; i++) 
    {   
        // Allocation input (au sens suite de caractère symbolique)
        ai->inputs[i].symb = malloc(size_max_input * sizeof(char));
        if (ai->inputs[i].symb == NULL) {
            fprintf(stderr,"Erreur d'allocation de mémoire : ai->inputs[%d].symb",i);

            // Libération des lignes déjà allouées
            for (int j = 0; j < i; j++) {
                free(ai->inputs[j].symb);
            }

            // Libération du reste de la structure
            free(ai->inputs);
            free(ai);
            exit(1);
        }

        // Allocation description input
        ai->inputs[i].descr = malloc(70 * sizeof(char));
        if (ai->inputs[i].descr == NULL) {
            fprintf(stderr,"Erreur d'allocation de mémoire : ai->inputs[%d].descr",i);

            // Libération des lignes déjà allouées
            for (int j = 0; j < i; j++) {
                free(ai->inputs[j].descr);
            }

            // Libération du reste de la structure
            for (int j = 0; j < num_inputs; j++) {
                free(ai->inputs[j].symb);
            }
            free(ai->inputs);
            free(ai);
            exit(1);
        }
    }

    return ai;
}

void free_AI(accepted_inputs * ai)
{
    // Libération des inputs
    for (int i = 0; i < ai->num_inputs ; i++) {
        free(ai->inputs[i].symb);
        free(ai->inputs[i].descr);
    }
    free(ai->inputs);

    // Libération du reste de la structure
    free(ai);
}

accepted_inputs * add_input(accepted_inputs * ai, int index, char * symb_input, char * descr_input)
{
    strcpy(ai->inputs[index].symb, symb_input);
    strcpy(ai->inputs[index].descr, descr_input);
    return ai;
}


void affich_menu(accepted_inputs * ai, int bool_menu_princ, int nb_tabs)
{
    // Banière exclusive au menu principal
    if (bool_menu_princ == 1){
        printf("====================== MENU ======================\n");
    }
    
    // Affichage des choix d'inputs possibles
    for (int i = 0 ; i < ai->num_inputs ; i++){
        for (int j = 0 ; j < nb_tabs ; j++){
            printf("\t");
        }
        printf("%s. %s\n", ai->inputs[i].symb, ai->inputs[i].descr);
    }
    
    printf("==================================================\n");
}




int valid_input(char * buffer, accepted_inputs * ai)
{   
    // Pour chaque input valable
    for (int i = 0 ; i < ai->num_inputs ; i++){

        // Comparer si le buffer user est équivalent
        int c = strcmp(buffer, ai->inputs[i].symb);
        if (c == 0) {
            return 0;
        }
    }
    return 1;
}

char * get_user_input(accepted_inputs * ai)
{
    char * buffer = malloc(40 * sizeof(char));
    if (buffer == NULL) {
        perror("Erreur d'allocation mémoire buffer\n");
        exit(1);
    }

    while (1) {    
        // Récupération de l'input de l'utilisateur
        printf("Choix?:");
        scanf("%s", buffer);

        // Si l'input est valide 
        int v = valid_input(buffer, ai);
        if (v == 0) 
        {
            int      len = strlen(buffer) + 1; // + 1 pour le caractère nul '\0'
            char * input = malloc(len * sizeof(char));
            if (input == NULL) {
                perror("Erreur d'allocation mémoire input\n");
                free(buffer);
                exit(1);
            }

            // Copie de la string contenue dans le buffer dans un espace mémoire optimisé
            strcpy(input, buffer);

            // Libération du buffer
            free(buffer);

            //Netoyage affichage précédent
            clear();

            return input;
        } else {
            printf("Veuillez entrer un caractère valide.\n");
        }     
    }
}

void get_pause_input()
{   
    printf("\n(rentrez n'importe quel input pour continuer)\n");
    char * buffer = malloc(40 * sizeof(char));
    scanf(" %s",buffer);
    
    clear();
    free(buffer);
}