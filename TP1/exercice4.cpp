/**
 * \mainpage TP1 - Exercice 4
 * \file    pb4.cpp
 * \author	Aloys Russel Tonfo & Mohamed Elbahrawy
 * \date 	12-05-2026
 * Crée le  12-05-2026
*/

#include <iostream>
using namespace std;

int main() {
    const int size = 10;
    int nombres[size];

    cout << "Entrez " << size << " entiers : ";
    for (int i = 0; i < size; ++i) {
        cin >> nombres[i];
    }

    // Tri par sélection
    for (int i = 0; i < size - 1; ++i) {
        int minIndex = i;
        for (int j = i + 1; j < size; ++j) {
            if (nombres[j] < nombres[minIndex]) {
                minIndex = j;
            }
        }
        int temp = nombres[i];
        nombres[i] = nombres[minIndex];
        nombres[minIndex] = temp;
    }

    cout << "Voici le tableau trie :";
    for (int i = 0; i < size; ++i) {
        cout << nombres[i] << " ";
    }
    cout << endl;

    return 0;
}
