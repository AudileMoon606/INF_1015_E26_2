#include "fonctions.h"
#include <iostream>

void maSuperFonction() {
    std::cout << "Salut depuis l'autre fichier !" << std::endl;
}

int readInRange(const string& prompt, int min, int max) {
    int value;
    while (true) {
        cout << prompt;
        cin >> value;
        if (value >= min && value <= max) {
            return value;
        }
        cout << "Veuillez entrer un nombre entre " << min << " et " << max << "." << endl;
    }
}
