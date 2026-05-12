/**
 * 
 * \file 	
 * \author	
 * \date 	
 * Crée le 12-05-2026
 */

// Collision : Écrivez un programme qui détermine si une collision se produit dans les x premières secondes
// d’un trajet. Le temps total (nombre entier supérieur à 0) est fourni par l’utilisateur. Vous devez ensuite
// lire la position initiale en x (quelconque) et la vitesse en x (entre -100 et 100) de deux trains. Toutes les
// valeurs doivent être lues avec la fonction définie dans l’exercice 2. Vous pouvez utiliser la constante
// INFINITY de cmath lorsqu’il n’y a pas de borne supérieure ou inférieure pour l’intervalle. Puis, vous
// devez indiquer l’emplacement à chaque seconde des deux trains jusqu’à arriver au temps total ou à la
// première collision. Vous devez finalement indiquer après combien de secondes il y a eu une collision ou
// indiquer qu’il n’y en a pas eu. Le but est de vérifier à chaque seconde s’il y a une collision, et non de
// calculer analytiquement le moment de la collision

#include <iostream>
#include <cmath>
#include "fonctions.h"
using namespace std;

int main(){
    int totalTime = readInRange("Entrez le temps total du trajet (en secondes, > 0) : ", 1, INFINITY);
    
    cout << "Train 1 :" << endl;
    int initialPosition1 = readInRange("Entrez la position initiale en x : ", -INFINITY, INFINITY);
    int velocity1 = readInRange("Entrez la vitesse en x (entre -100 et 100) : ", -100, 100);
    
    cout << "Train 2 :" << endl;
    int initialPosition2 = readInRange("Entrez la position initiale en x : ", -INFINITY, INFINITY);
    int velocity2 = readInRange("Entrez la vitesse en x (entre -100 et 100) : ", -100, 100);
    
    bool collisionOccurred = false;
    int collisionTime = -1;

    for (int t = 0; t <= totalTime; t++) {
        int position1 = initialPosition1 + velocity1 * t;
        int position2 = initialPosition2 + velocity2 * t;

        cout << "Temps: " << t << "s - Train 1 Position: " << position1 << " | Train 2 Position: " << position2 << endl;

        if (position1 == position2) {
            collisionOccurred = true;
            collisionTime = t;
            break;
        }
    }

    if (collisionOccurred) {
        cout << "Collision détectée après " << collisionTime << " secondes." << endl;
    } else {
        cout << "Aucune collision détectée pendant le trajet." << endl;
    }

    return 0;
}