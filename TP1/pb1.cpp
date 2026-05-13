/**
 * \mainpage TP1 - Exercice 1
 * \file    pb1.cpp
 * \author	Aloys Russel Tonfo & Mohamed Elbahrawy
 * \date 	12-05-2026
 * Crée le  12-05-2026
*/


#include <iostream>
#include <cmath>

using namespace std;


bool isPrime(int number) {

    for (int i = 2; i <= sqrt(number); ++i) {
        if (number % i == 0) return false; 
    }
    return true;
}

int nextPrime(int number) {
    while (true) {
        if (isPrime(number)) return number;
        ++number;
    }
}

int main() {
    while (true) {
        int number;
        cout << "Entrez un nombre entier : ";
        cin >> number;

        if (number == -1) break;

        int nextPrimeNumber = nextPrime(number);
        cout << "Le prochain nombre premier est " << nextPrimeNumber << "." << endl;
    }
}