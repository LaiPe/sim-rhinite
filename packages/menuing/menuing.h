#ifndef MENUING_H
#define MENUING_H

// Structures
typedef struct 
{
    char * symb;
    char * descr;
} input;

typedef struct 
{
    input * inputs;
    int num_inputs;

} accepted_inputs;

// Fonctions
void clear();

accepted_inputs * init_AI(int num_inputs, int size_max_input);

void free_AI(accepted_inputs * ai);

accepted_inputs * add_input(accepted_inputs * ai, int index, char * symb_input, char * descr_input);

void affich_menu(accepted_inputs * ai, int bool_menu_princ, int nb_tabs);

char * get_user_input(accepted_inputs * ai);

void get_pause_input();

#endif