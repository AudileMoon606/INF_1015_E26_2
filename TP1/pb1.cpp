/**
 * \mainpage TP1 - Exercice 1
 * \file    pb1.cpp
 * \author	Aloys Russel Tonfo & Mohamed Elbahrawy
 * \date 	12-05-2026
 * Crée le  12-05-2026
 */

// Premier Nombre premier apres un entier donner
#include <iostream>
using namespace std;

// Fonction pour vérifier si un nombre est premier

bool isPrime (int n){
    if (n <= 1 ){
        return false;
    }
    for (int i = 2 ; i <= n / 2 ; i++){
        if (n % i == 0){
            return false;
        }    
    }
    return true;        
}

int nextPrime (int n){
    int next = n + 1;
    while (true){
        if (isPrime(next)){
            return next;
        }
        next++;
    }
}

int main() {
    int n;
    cout << "Entrez un entier : ";
    cin >> n;
    int next = nextPrime(n);
    cout << "Le prochain nombre premier apres " << n << " est : " << next << endl;
    return 0;
}   
