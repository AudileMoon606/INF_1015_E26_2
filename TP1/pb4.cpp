/**
 * \mainpage TP1 - Exercice 4
 * \file    pb4.cpp
 * \author	Aloys Russel Tonfo & Mohamed Elbahrawy
 * \date 	12-05-2026
 * Crée le  12-05-2026
*/

#include <iostream>
#include "fonctions.h"
using namespace std;

int main() {
    const int size = 10;
    int arr[size];

    cout << "Entrez " << size << " entiers : ";
    for (int i = 0; i < size; ++i) {
        cin >> arr[i];
    }

    // Tri par sélection
    for (int i = 0; i < size - 1; ++i) {
        int minIndex = i;
        for (int j = i + 1; j < size; ++j) {
            if (arr[j] < arr[minIndex]) {
                minIndex = j;
            }
        }
        int temp = arr[i];
        arr[i] = arr[minIndex];
        arr[minIndex] = temp;
    }

    cout << "Tableau trié : ";
    for (int i = 0; i < size; ++i) {
        cout << arr[i] << " ";
    }
    cout << endl;

    return 0;
}
