/**
 * \file     structures.hpp
 * \author   Aloys Russel Tonfo & Mohamed Elbahrawy
 * \date     26 mai 2026
 * \version  1.0
 * \brief    Déclarations des structures de données (Film, Acteur, Liste) pour le TP3.
 * Développé le 19-05-2026.
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

template<typename T>
class Liste { 
private:
    int capacite = 0;
    int nElements = 0;
    std::unique_ptr<shared_ptr<T>[]> elements;

public:
    Liste() = default;
    Liste(int cap) { initialiser(cap); }
    
    // Constructeur de copie
    Liste(const Liste& autre) {
        initialiser(autre.capacite);
        nElements = autre.nElements;
        for (size_t i = 0; i < static_cast<size_t>(nElements); ++i) {
            elements[i] = autre.elements[i]; 
        }
    }

    int getCapacite() const { return capacite; }
    int getNElements() const { return nElements; }
    
    shared_ptr<T>* getElements() { return elements.get(); }
    const shared_ptr<T>* getElements() const { return elements.get(); }

    void initialiser(int cap) {
    capacite = cap;
    nElements = cap;  
    elements = make_unique<shared_ptr<T>[]>(static_cast<size_t>(cap));
}

    void modifierElement(size_t index, const std::shared_ptr<T>& element) {
        if (index < static_cast<size_t>(capacite)) { 
            elements[index] = element;
            if (index >= static_cast<size_t>(nElements)) {
                nElements = static_cast<int>(index) + 1; 
            }
        }
    }
    shared_ptr<T>& operator[](size_t index) {
        return elements[index];
    }

    friend void enleverElement(Liste& liste, T* elementAEnlever) {
        
    }
};

using ListeActeurs = Liste<Acteur>; 

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
