/**
 * \mainpage TP1 - Exercice 2
 * \file    pb2.cpp
 * \author	Aloys Russel Tonfo & Mohamed Elbahrawy
 * \date 	12-05-2026
 * Crée le  12-05-2026
*/


#include <iostream>
#include <random>
using namespace std;

int readIntInRange(const string& prompt, float min, float max) {
    float value;
    while (true) {
        cout << prompt;
        cin >> value;

        if (value >= min && value <= max) {
            return static_cast<int>(value);
        }
    }
}

int main() {
    const float min = 0;
    const float max = 1000;
    string prompt = "Entrez un nombre entier : ";
    bool guessed = false;
    int nAttempts = 0;

    // Génération de nombre aléatoire inspiré de l'IA générative Gemini de Google
    random_device rd; 
    mt19937 gen(rd());    
    uniform_int_distribution<int> distrib(min, max);
    int randomNumber = distrib(gen);

    
    while (!guessed) {
        nAttempts += 1;
        int userGuess = readIntInRange(prompt, min, max);

        if (userGuess < randomNumber) {
            cout << "Trop bas." << endl;
        } else if (userGuess > randomNumber) {
            cout << "Trop haut." << endl;
        }
        else {
            guessed = true;
            cout << "Bravo! Vous avez réussi en " << nAttempts << " tentatives!" << endl;
        }
    }   
}
