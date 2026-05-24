/**
 * \mainpage TP3 
 * \file    structures.hpp
 * \author	Aloys Russel Tonfo & Mohamed Elbahrawy
 * \date 	21-05-2026
 * Crée le  19-05-2026
*/

#pragma once
#include <iostream>
#include <string>
#include <memory>
#include <functional>

using namespace std;


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

    Film* chercherFilm(const function<bool(const Film*)>& critere) const;

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

    private:
	int capacite =0;
    int  nElements=0 ;
	unique_ptr<shared_ptr<Acteur>[]> elements; // Pointeur vers un tableau de Acteur*, chaque Acteur* pointant vers un Acteur.

    public:
    ListeActeurs() = default;
    ListeActeurs(int cap);
    ListeActeurs(const ListeActeurs& autre);

    int getCapacite() const{ return capacite; }
    int getNElements() const { return nElements; }
    shared_ptr<Acteur>* getElements() const { return elements.get(); }

    void initialiser(int cap) {
    capacite = cap;
    nElements = cap;
    elements = std::make_unique<std::shared_ptr<Acteur>[]>(static_cast<size_t>(capacite));
}

    void modifierElement(size_t index, const shared_ptr<Acteur>& acteur) {
        if (index < static_cast<size_t>(nElements)) {
            elements[index] = acteur;
        }
    }

    friend void enleverActeur(ListeActeurs& listeActeurs, Acteur* acteur);

};

struct Film
{
	std::string titre, realisateur; // Titre et nom du réalisateur (on suppose qu'il n'y a qu'un réalisateur).
	int anneeSortie = 0;
    int  recette = 0; 
	ListeActeurs acteurs;
    
};
ostream& operator<<(std::ostream& os, const Film& film);

struct Acteur
{
	std::string nom; 
    int anneeNaissance = 0; 
    char sexe = '\0';
	// ListeFilms joueDans;
};
