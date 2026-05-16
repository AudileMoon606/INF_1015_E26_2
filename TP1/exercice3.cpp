/**
 * \mainpage TP1 - Exercice 3
 * \file    pb3.cpp
 * \author	Aloys Russel Tonfo & Mohamed Elbahrawy
 * \date 	12-05-2026
 * Crée le  12-05-2026
*/

#include <iostream>
#include <cmath>
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

int positionAfterTime(int initialPosition, int speed, int time) {
    return initialPosition + speed * time;
}

bool collisionDuringSecond(int pos1Start, int speed1, int pos2Start, int speed2) {
    int pos1End = pos1Start + speed1;
    int pos2End = pos2Start + speed2;
    
    // Check if they occupy the same position at any point
    if (pos1Start == pos2Start || pos1End == pos2End) {
        return true;
    }
    
    // Check if they crossed (one was ahead, then behind)
    if ((pos1Start < pos2Start && pos1End > pos2End) || 
        (pos1Start > pos2Start && pos1End < pos2End)) {
        return true;
    }
    
    return false;
}

int main() {
    float minTime = 1.0;
    float maxTime = INFINITY;
    string promptTime = "Entrez le temps total : ";
    int time = readIntInRange(promptTime, minTime, maxTime);

    float minPositionTrain1 = -INFINITY;
    float maxPositionTrain1 = INFINITY;
    string promptPositionTrain1 = "Entrez la position initiale du premier train : ";
    int positionTrain1 = readIntInRange(promptPositionTrain1, minPositionTrain1, maxPositionTrain1);

    float minSpeedTrain1 = -100.0;
    float maxSpeedTrain1 = 100.0;
    string promptSpeedTrain1 = "Entrez la vitesse du premier train : ";
    int speedTrain1 = readIntInRange(promptSpeedTrain1, minSpeedTrain1, maxSpeedTrain1);

    float minPositionTrain2 = -INFINITY;
    float maxPositionTrain2 = INFINITY;
    string promptPositionTrain2 = "Entrez la position initiale du second train : ";
    int positionTrain2 = readIntInRange(promptPositionTrain2, minPositionTrain2, maxPositionTrain2);

    float minSpeedTrain2 = -100.0;
    float maxSpeedTrain2 = 100.0;
    string promptSpeedTrain2 = "Entrez la vitesse du second train : ";
    int speedTrain2 = readIntInRange(promptSpeedTrain2, minSpeedTrain2, maxSpeedTrain2);

    int timeElapsed = 0;

    while (timeElapsed < time) {
        int previousPositionTrain1 = positionTrain1;
        int previousPositionTrain2 = positionTrain2;

        timeElapsed++;
        positionTrain1 = positionAfterTime(positionTrain1, speedTrain1, 1);
        positionTrain2 = positionAfterTime(positionTrain2, speedTrain2, 1);
        cout << "Apres " << timeElapsed << " seconde(s)" << endl;
        cout << "Train 1 : " << positionTrain1 << endl;
        cout << "Train 2 : " << positionTrain2 << endl;

        if (collisionDuringSecond(previousPositionTrain1, speedTrain1, previousPositionTrain2, speedTrain2)) {
            cout << "Il y a eu une collision pendant la " << timeElapsed << "e seconde!" << endl;
            return 0;
        }
    }
    cout << "Il n'y a pas eu de collision." << endl;
}
