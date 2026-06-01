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

class Item;
class Film;
class Livre;
class ListeFilms;

struct Film;
struct Acteur; // Permet d'utiliser les types alors qu'ils seront défini après.

struct Acteur
{
    std::string nom;
    int anneeNaissance = 0;
    char sexe = '\0';
    // ListeFilms joueDans;
};

template <typename T>
class Liste
{
private:
    int capacite = 0;
    int nElements = 0;
    std::unique_ptr<shared_ptr<T>[]> elements;

public:
    Liste() = default;
    Liste(int cap) { initialiser(cap); }

    // Constructeur de copie
    Liste(const Liste &autre)
    {
        initialiser(autre.capacite);
        nElements = autre.nElements;
        for (size_t i = 0; i < static_cast<size_t>(nElements); ++i)
        {
            elements[i] = autre.elements[i];
        }
    }

    int getCapacite() const { return capacite; }
    int getNElements() const { return nElements; }

    shared_ptr<T> *getElements() { return elements.get(); }
    const shared_ptr<T> *getElements() const { return elements.get(); }

    void initialiser(int cap)
    {
        capacite = cap;
        nElements = cap;
        elements = make_unique<shared_ptr<T>[]>(static_cast<size_t>(cap));
    }

    void modifierElement(size_t index, const std::shared_ptr<T> &element)
    {
        if (index < static_cast<size_t>(capacite))
        {
            elements[index] = element;
            if (index >= static_cast<size_t>(nElements))
            {
                nElements = static_cast<int>(index) + 1;
            }
        }
    }
    shared_ptr<T> &operator[](size_t index)
    {
        return elements[index];
    }

    friend void enleverElement(Liste &liste, T *elementAEnlever)
    {
    }
};

using ListeActeurs = Liste<Acteur>;

class Item
{
protected:
    string titre_;
    int annee_;

public:
    Item() : titre_(""), annee_(0) {}
    Item(const string &titre, int annee) : titre_(titre), annee_(annee) {}

    virtual ~Item() = default;

    virtual void afficher(ostream &os) const
    {
        os << "Titre: " << titre_ << " | Année: " << annee_;
    }

    friend Film *lireFilm(istream &fichier, ListeFilms &listeFilms);
    friend shared_ptr<Acteur> lireActeur(istream &fichier, ListeFilms &listeFilms);
    friend const Acteur *trouverActeur(const ListeFilms &listeFilms, const string &nomActeur);
};

inline ostream &operator<<(std::ostream &os, const Item &item)
{
    item.afficher(os);
    return os;
}

class Film : virtual public Item
{
private:
    string realisateur_;
    int recette_ = 0;
    ListeActeurs acteurs_;

public:
    Film() : Item(), realisateur_(""), recette_(0) {}
    Film(const string &titre, int annee, const string &realisateur, int recette)
        : Item(titre, annee), realisateur_(realisateur), recette_(recette) {}

    void afficher(ostream &os) const override
    {

        Item::afficher(os);
        os << "\nRéalisateur : " << realisateur_ << "\n" // Ajout du \n au début
           << "Recette     : " << recette_ << " M$\n"
           << "Acteurs:\n";
        for (size_t i = 0; i < static_cast<size_t>(acteurs_.getNElements()); ++i)
        {
            if (acteurs_.getElements()[i])
            {
                os << "  " << acteurs_.getElements()[i]->nom << ", "
                   << acteurs_.getElements()[i]->anneeNaissance << " "
                   << acteurs_.getElements()[i]->sexe << "\n";
            }
        }
    }

    // Accesseurs pour conserver le fonctionnement du code dans mon cpp
    ListeActeurs &getActeurs() { return acteurs_; }
    const ListeActeurs &getActeurs() const { return acteurs_; }

    friend Film *lireFilm(istream &fichier, ListeFilms &listeFilms);
    friend shared_ptr<Acteur> lireActeur(istream &fichier, ListeFilms &listeFilms);
    friend const Acteur *trouverActeur(const ListeFilms &listeFilms, const string &nomActeur);
    friend ostream &operator<<(ostream &os, const Film &film);
};

class ListeFilms
{

private:
    int capacite_, nElements_;
    Film **elements_;

public:
    ListeFilms()
    {
        capacite_ = 0;
        nElements_ = 0;
        elements_ = nullptr;
    }

    int getCapacite() const { return capacite_; }
    int getNElements() const { return nElements_; }
    Film **getElements() const { return elements_; }

    Film *chercherFilm(const function<bool(const Film *)> &critere) const;

    void ajouterFilm(Film *film);
    void enleverFilm(const Film *film);
    void detruire()
    {
        delete[] elements_;
        elements_ = nullptr;
        capacite_ = 0;
        nElements_ = 0;
    }
};

class Livre : virtual public Item
{
private:
    string auteur_;
    int copiesVendues_;
    int nPages_;

public:
    Livre() : Item(), auteur_(""), copiesVendues_(0), nPages_(0) {}
    Livre(const string &titre, int annee, const string &auteur, int copiesVendues, int nPages)
        : Item(titre, annee), auteur_(auteur), copiesVendues_(copiesVendues), nPages_(nPages) {}

    // Redéfinition de la méthode afficher pour les Livres
    void afficher(ostream &os) const override
    {
        Item::afficher(os);
        os << "Auteur      : " << auteur_ << "\n"
           << "Vendues     : " << copiesVendues_ << " millions de copies\n"
           << "Pages       : " << nPages_ << "\n";
    }

    friend void lireLivres(const string &nomFichier);
};