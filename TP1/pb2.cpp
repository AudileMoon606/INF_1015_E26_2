/**
 * 
 * \file 	
 * \author	
 * \date 	
 * Crée le 12-05-2026
 */


#include <iostream>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include "fonctions.h"
using namespace std;   

int main (){
    int n = readInRange("Entrez un entier : ", -INFINITY, INFINITY);
    srand(time(0));
    int randomNum = rand() % 100 + 1; // Génère un nombre aléatoire entre 1 et 100
    cout << "Le nombre aléatoire généré est : " << randomNum << endl;
    return 0;
}





