#include <stdio.h>

#include "./rhinite.h"
#include "../packages/menuing/menuing.h"

int main() 
{
    // Menu principal
    printf("sim-rhinite : Mode Idle\n");
    accepted_inputs * princ_ai = init_AI(3, 1);
    add_input(princ_ai, 0, "1", "Valeurs par défaut");
    add_input(princ_ai, 1, "2", "Valeurs personnalisées");
    add_input(princ_ai, 2, "q", "Quitter");

    affich_menu(princ_ai, 1, 1);
}
