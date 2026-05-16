/**
 * \mainpage TP1 - Exercice 5
 * \file    pb5.cpp
 * \author	Aloys Russel Tonfo & Mohamed Elbahrawy
 * \date 	12-05-2026
 * Crée le  12-05-2026
*/
#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>

using namespace std;

struct Aliment {
    string nom;
    string type;
    int quantite;
    double prixUnitaire;
};

void afficherAlimentLePlusCher(const Aliment aliments[], int taille) {
    if (taille == 0) {
        cout << "Aucun aliment dans l'inventaire." << endl;
        return;
    }

    int indexMax = 0;
    for (int i = 1; i < taille; ++i) {
        if (aliments[i].prixUnitaire > aliments[indexMax].prixUnitaire) {
            indexMax = i;
        }
    }

    const Aliment& lePlusCher = aliments[indexMax];
    cout << lePlusCher.quantite << " " << lePlusCher.nom 
        << " (" << lePlusCher.type << ") a " 
        << fixed << setprecision(2) << lePlusCher.prixUnitaire 
        << "$ chacun" << endl;
}

int main() {
    const int MAX_ALIMENTS = 10;
    Aliment aliments[MAX_ALIMENTS];
    int nombreAliments = 0;

    ifstream fichier("inventaire.txt");
    if (!fichier) {
        cerr << "Erreur lors de l'ouverture du fichier inventaire.txt" << endl;
        return 1;
    }

    string tempNom, tempType, tempQuantite, tempPrix;

    while (nombreAliments < MAX_ALIMENTS && getline(fichier, tempNom, '\t')) { 
        
        getline(fichier, tempType, '\t');     
        getline(fichier, tempQuantite, '\t'); 
        getline(fichier, tempPrix);           

        aliments[nombreAliments].nom = tempNom;
        aliments[nombreAliments].type = tempType;
        aliments[nombreAliments].quantite = stoi(tempQuantite);
        aliments[nombreAliments].prixUnitaire = stod(tempPrix);

        nombreAliments++;
    }

    fichier.close();

    afficherAlimentLePlusCher(aliments, nombreAliments);

    return 0;
}