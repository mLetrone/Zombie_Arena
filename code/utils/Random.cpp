#include "Random.hpp"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void InitRandom() {/*Initialise le srand pour l'ensemble du programme*/
    srand(time(NULL));
}
/*Fonction qui renvoie un nombre aléatoirre entre le min et max
mis en parametre*/
int nb_randint(int min, int max) {
    return(rand() / (((double)RAND_MAX) + 1) * (max - min + 1) + min);
}
