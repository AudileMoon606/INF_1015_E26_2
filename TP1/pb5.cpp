/**
 * \mainpage TP1 - Exercice 5
 * \file    pb5.cpp
 * \author	Aloys Russel Tonfo & Mohamed Elbahrawy
 * \date 	12-05-2026
 * Crée le  12-05-2026
*/

/* 5. Magasin : Écrire un programme de traitement de données pour une épicerie (aliment, type, quantité, prix
* unitaire). Le programme doit créer un tableau de structures, y placer l’inventaire en le lisant d’un fichi/er,
* et ensuite afficher dans une fonction l’aliment le plus cher dans le format suivant :/
* quantité nom (type) a prix$ chacun/
* Un fichier « inventaire.txt » vous/ est fourni, où chaque ligne contient un aliment sous la forme :
* nom → type → quantité → prix ↵/
* Le → est le caractère de tabul/ation indiqué par « \t » en C/C++, et ↵ est le caractère de fin de ligne
* « \n ». Les champs nom et type peuvent contenir des espaces, le document Moodle « Exemples/annexe/
* pour entrées-sorties avec les "streams" C++ » montre des exemples de lecture de ce type. Vous pou/vez
* supposer que l’inventaire contiendra au maximum 10 aliments (constante définie dans votre programme)
* et que le nom du fichier est toujours « inventaire.txt ». 
*/

#include <iostream>
#include <fstream>
#include <string>
using namespace std;

struct Aliment {
    string name;
    string type;
    int quantity;
    double unitPrice;
};

void alimentMostExpensive(const Aliment inventory[], int count) {
    if (count == 0) {
        cout << "Aucun aliment trouvé dans l'inventaire." << endl;
        return;
    }

    Aliment mostExpensive = inventory[0];
    for (int i = 1; i < count; i++) {
        if (inventory[i].unitPrice > mostExpensive.unitPrice) {
            mostExpensive = inventory[i];
        }
    }
    cout << mostExpensive.quantity << " " << mostExpensive.name << " (" << mostExpensive.type << ") à " << mostExpensive.unitPrice << "$ chacun" << endl;
}


int main() {
    const int maxAliments = 10;
    Aliment inventory[maxAliments];
    int count = 0;

    ifstream infile("inventaire.txt");
    while (count < maxAliments && infile) {
        string line;
        if (!getline(infile, line)) {
            break;
        }

        size_t pos1 = line.find('\t');
        size_t pos2 = line.find('\t', pos1 + 1);
        size_t pos3 = line.find('\t', pos2 + 1);

        //Inspiré en partie de l'IA générative ChatGPT par OpenAI 
        inventory[count].name = line.substr(0, pos1);
        inventory[count].type = line.substr(pos1 + 1, pos2 - pos1 - 1);
        inventory[count].quantity = stoi(line.substr(pos2 + 1, pos3 - pos2 - 1));
        inventory[count].unitPrice = stod(line.substr(pos3 + 1));
        
        count++;
    }

    infile.close();

    alimentMostExpensive(inventory, count);
    return 0;
}
