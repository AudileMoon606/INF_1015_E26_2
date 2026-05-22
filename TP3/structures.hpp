/**
 * \mainpage TP3 
 * \file    structures.hpp
 * \author	Aloys Russel Tonfo & Mohamed Elbahrawy
 * \date 	21-05-2026
 * Crée le  19-05-2026
*/

#pragma once
// Structures mémoires pour une collection de films.

#include <string>

struct Film; struct Acteur; // Permet d'utiliser les types alors qu'ils seront défini après.

class ListeFilms {

private:
    int capacite_, nElements_;
    Film** elements_; 


public:
    ListeFilms() {
        capacite_ = 0;
        nElements_ = 0;
        elements_ = nullptr;
    }

    int getCapacite() const { return capacite_; }
    int getNElements() const { return nElements_; }
    Film** getElements() const { return elements_; }

    void ajouterFilm(Film* film);
    void enleverFilm(const Film* film);
    void detruire() {
        delete[] elements_;  
        elements_ = nullptr; 
        capacite_ = 0;   
        nElements_ = 0;
    }


};

struct ListeActeurs {
	int capacite, nElements;
	Acteur** elements; // Pointeur vers un tableau de Acteur*, chaque Acteur* pointant vers un Acteur.
};

struct Film
{
	std::string titre, realisateur; // Titre et nom du réalisateur (on suppose qu'il n'y a qu'un réalisateur).
	int anneeSortie, recette; // Année de sortie et recette globale du film en millions de dollars
	ListeActeurs acteurs;
};

struct Acteur
{
	std::string nom; int anneeNaissance; char sexe;
	ListeFilms joueDans;
};
