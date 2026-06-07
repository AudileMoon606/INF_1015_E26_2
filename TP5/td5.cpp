/**
 * \mainpage Description du projet TP4
 * \file     td5.cpp
 * \author   Aloys Russel Tonfo & Mohamed Elbahrawy
 * \date     26 mai 2026
 * \version  1.0
 * \brief    Code principal pour le TP3 - Structures de données et listes génériques.
 * Développé le 19-05-2026.
 */
#pragma region "Includes"//{
#define _CRT_SECURE_NO_WARNINGS // On permet d'utiliser les fonctions de copies de chaînes qui sont considérées non sécuritaires.

#include "structures.hpp" // Structures de données pour la collection de films en mémoire.

#include <iostream>
#include <fstream>
#include <string>
#include <limits>
#include <algorithm>
#include <vector>
#include <iomanip>
#include <forward_list> 
#include <map>          
#include <set>          

#include "cppitertools/range.hpp"
#include "gsl/span"

#include "bibliotheque_cours.hpp"
#include "verification_allocation.hpp" // Nos fonctions pour le rapport de fuites de mémoire.
#include "debogage_memoire.hpp"        // Ajout des numéros de ligne des "new" dans le rapport de fuites.  Doit être après les include du système, qui peuvent utiliser des "placement new" (non supporté par notre ajout de numéros de lignes).

using namespace std;
using namespace iter;
using namespace gsl;

#pragma endregion //}

typedef uint8_t UInt8;
typedef uint16_t UInt16;

#pragma region "Fonctions de base pour lire le fichier binaire"//{
template <typename T>
T lireType(istream &fichier)
{
    T valeur{};
    fichier.read(reinterpret_cast<char *>(&valeur), sizeof(valeur));
    return valeur;
}
#define erreurFataleAssert(message) assert(false && (message)), terminate()
static const uint8_t enteteTailleVariableDeBase = 0xA0;
size_t lireUintTailleVariable(istream &fichier)
{
    uint8_t entete = lireType<uint8_t>(fichier);
    switch (entete)
    {
    case enteteTailleVariableDeBase + 0:
        return lireType<uint8_t>(fichier);
    case enteteTailleVariableDeBase + 1:
        return lireType<uint16_t>(fichier);
    case enteteTailleVariableDeBase + 2:
        return lireType<uint32_t>(fichier);
    default:
        erreurFataleAssert("Tentative de lire un entier de taille variable alors que le fichier contient autre chose à cet emplacement.");
    }
}

string lireString(istream &fichier)
{
    string texte;
    texte.resize(lireUintTailleVariable(fichier));
    fichier.read(reinterpret_cast<char *>(&texte[0]), streamsize(sizeof(texte[0])) * texte.length());
    return texte;
}

#pragma endregion //}

// TODO: Une fonction pour ajouter un Film à une ListeFilms, le film existant déjà; on veut uniquement ajouter le pointeur vers le film existant.  Cette fonction doit doubler la taille du tableau alloué, avec au minimum un élément, dans le cas où la capacité est insuffisante pour ajouter l'élément.  Il faut alors allouer un nouveau tableau plus grand, copier ce qu'il y avait dans l'ancien, et éliminer l'ancien trop petit.  Cette fonction ne doit copier aucun Film ni Acteur, elle doit copier uniquement des pointeurs.
void ListeFilms::ajouterFilm(Film *film)
{
    if (nElements_ == capacite_)
    {
        int nouvelleCapacite = max(1, 2 * capacite_);
        Film **nouveauTableau = new Film *[nouvelleCapacite];
        copy(elements_, elements_ + nElements_, nouveauTableau);
        delete[] elements_;
        elements_ = nouveauTableau;
        capacite_ = nouvelleCapacite;
    }
    elements_[nElements_] = film;
    nElements_++;
}

void ListeFilms::enleverFilm(const Film *film)
{
    for (int i = 0; i < nElements_; ++i)
    {
        if (elements_[i] == film)
        {
            for (int j = i; j < nElements_ - 1; ++j)
            {
                elements_[j] = elements_[j + 1];
            }
            nElements_--;
            return;
        }
    }
}

void enleverActeur(Liste<Acteur> &listeActeurs, Acteur *acteur)
{
    for (int i = 0; i < listeActeurs.getNElements(); ++i)
    {
        if (listeActeurs.getElements()[i].get() == acteur)
        {
            listeActeurs.getElements()[i] = listeActeurs.getElements()[listeActeurs.getNElements() - 1];
            listeActeurs.modifierElement(static_cast<size_t>(listeActeurs.getNElements() - 1), nullptr);
            return;
        }
    }
}

// TODO: Une fonction pour trouver un Acteur par son nom dans une ListeFilms, qui retourne un pointeur vers l'acteur, ou nullptr si l'acteur n'est pas trouvé.  Devrait utiliser span.
const Acteur *trouverActeur(const ListeFilms &listeFilms, const string &nomActeur)
{
    for (const Film *film : span(listeFilms.getElements(), static_cast<size_t>(listeFilms.getNElements())))
    {
        for (const shared_ptr<Acteur> &acteur : span(film->getActeurs().getElements(), static_cast<size_t>(film->getActeurs().getNElements())))
        {
            if (acteur->nom == nomActeur)
                return acteur.get();
        }
    }
    return nullptr;
}

// TODO: Compléter les fonctions pour lire le fichier et créer/allouer une ListeFilms.  La ListeFilms devra être passée entre les fonctions, pour vérifier l'existence d'un Acteur avant de l'allouer à nouveau (cherché par nom en utilisant la fonction ci-dessus).
shared_ptr<Acteur> lireActeur(istream &fichier, ListeFilms &listeFilms)
{
    Acteur acteur = {};
    acteur.nom = lireString(fichier);
    acteur.anneeNaissance = int(lireUintTailleVariable(fichier));
    acteur.sexe = char(lireUintTailleVariable(fichier));

    const Acteur *acteurExistant = trouverActeur(listeFilms, acteur.nom);

    if (acteurExistant != nullptr)
    {
        for (int i = 0; i < listeFilms.getNElements(); i++)
        {
            Film *f = listeFilms.getElements()[i];
            for (int j = 0; j < f->getActeurs().getNElements(); j++)
            {
                if (f->getActeurs().getElements()[j] && f->getActeurs().getElements()[j].get() == acteurExistant)
                {
                    return f->getActeurs().getElements()[j];
                }
            }
        }
    }

    auto nouvelActeur = make_shared<Acteur>();
    nouvelActeur->nom = acteur.nom;
    nouvelActeur->anneeNaissance = acteur.anneeNaissance;
    nouvelActeur->sexe = acteur.sexe;

    cout << "Création acteur: " << nouvelActeur->nom << endl;
    return nouvelActeur;
}

Film *lireFilm(istream &fichier, ListeFilms &listeFilms)
{
    auto film = make_unique<Film>();

    film->titre_ = lireString(fichier);
    film->realisateur_ = lireString(fichier);
    film->annee_ = int(lireUintTailleVariable(fichier));
    film->recette_ = int(lireUintTailleVariable(fichier));

    film->acteurs_.initialiser(int(lireUintTailleVariable(fichier)));
    for (size_t i = 0; i < static_cast<size_t>(film->acteurs_.getNElements()); i++)
    {
        shared_ptr<Acteur> acteur = lireActeur(fichier, listeFilms);
        film->acteurs_.modifierElement(i, acteur);
    }
    return film.release();
}

Film *ListeFilms::chercherFilm(const function<bool(const Film *)> &critere) const
{
    for (int i = 0; i < nElements_; ++i)
    {
        if (critere(elements_[i]))
        {
            return elements_[i];
        }
    }
    return nullptr;
}

ListeFilms creerListe(string nomFichier)
{
    ifstream fichier(nomFichier, ios::binary);
    fichier.exceptions(ios::failbit);

    int nElements = int(lireUintTailleVariable(fichier));

    ListeFilms listeFilms;
    for (int i = 0; i < nElements; i++)
    {
        Film *film = lireFilm(fichier, listeFilms);
        listeFilms.ajouterFilm(film);
    }

    return listeFilms;
}

void detruireFilm(Film *film)
{
    delete film;
}

void detruireListeFilms(ListeFilms &listeFilms)
{
    for (Film *film : span(listeFilms.getElements(), static_cast<size_t>(listeFilms.getNElements())))
    {
        detruireFilm(film);
    }
    listeFilms.detruire();
}

void afficherActeur(const Acteur &acteur)
{
    cout << "  " << acteur.nom << ", " << acteur.anneeNaissance << " " << acteur.sexe << endl;
}

void lireLivres(const string &nomFichier, vector<Item*> &bibliotheque)
{
    ifstream fichier(nomFichier);
    if (!fichier.is_open())
        return;

    string titre, auteur;
    int annee, copiesVendues, nPages;

    while (fichier >> quoted(titre) >> annee >> quoted(auteur) >> copiesVendues >> nPages)
    {
        bibliotheque.push_back(new Livre(titre, annee, auteur, copiesVendues, nPages));
    }
}

void afficherBibliotheque(const vector<Item*> &bibliotheque)
{
    static const string ligneDeSeparation = "\n\033[36m━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\033[0m\n";
    cout << ligneDeSeparation;
    for (const Item *item : bibliotheque)
    {
        if (item != nullptr)
        {
            cout << *item << endl;
            cout << ligneDeSeparation;
        }
    }
}

//Fonction d'affichage d'une liste d"item

template <typename Conteneur>
void afficherListeItems(const Conteneur& conteneur)
{
    static const string ligneDeSeparation = "\n\033[36m━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\033[0m\n";
    cout << ligneDeSeparation;
    for (const auto& item : conteneur)
    {
        if (item != nullptr)
        {
            item->afficherCourt(cout);
            cout << endl;
            cout << ligneDeSeparation;
        }
    }
}

int main()
{
    bibliotheque_cours::activerCouleursAnsi(); 

    static const string ligneDeSeparation = "\n\033[35m════════════════════════════════════════\033[0m\n";

    // TODO: Chaque TODO dans cette fonction devrait se faire en 1 ou 2 lines, en appelant les fonctions écrites.

    // TODO: La ligne suivante devrait lire le fichier binaire en allouant la mémoire nécessaire.  Devrait afficher les noms de 20 acteurs sans doublons (par l'affichage pour fins de débogage dans votre fonction lireActeur).
    ListeFilms listeFilms = creerListe("films.bin");

    cout << ligneDeSeparation << "Le premier film de la liste est:" << endl;
    // TODO: Afficher le premier film de la liste.  Devrait être Alien.
    if (listeFilms.getNElements() > 0)
    {
        cout << *listeFilms.getElements()[0] << endl;
    }

    cout << ligneDeSeparation << "Les films sont:" << endl;
    // TODO: Afficher la liste des films.  Il devrait y en avoir 7.
    for (int i = 0; i < listeFilms.getNElements(); ++i)
    {
        cout << *listeFilms.getElements()[i] << endl;
    }

    // TODO: Modifier l'année de naissance de Benedict Cumberbatch pour être 1976 (elle était 0 dans les données lues du fichier).  Vous ne pouvez pas supposer l'ordre des films et des acteurs dans les listes, il faut y aller par son nom.
    const Acteur *acteurBenedict = trouverActeur(listeFilms, "Benedict Cumberbatch");
    if (acteurBenedict)
        const_cast<Acteur *>(acteurBenedict)->anneeNaissance = 1976;

    cout << ligneDeSeparation << "Liste des films où Benedict Cumberbatch joue sont:" << endl;
    // TODO: Afficher la liste des films où Benedict Cumberbatch joue.  Il devrait y avoir Le Hobbit et Le jeu de l'imitation.
    for (int i = 0; i < listeFilms.getNElements(); i++)
    {
        Film *f = listeFilms.getElements()[i];
        for (int j = 0; j < f->getActeurs().getNElements(); j++)
        {
            if (f->getActeurs().getElements()[j] && f->getActeurs().getElements()[j]->nom == "Benedict Cumberbatch")
            {
                cout << f->getTitre() << endl;
                break;
            }
        }
    }

    Film skylien = *listeFilms.getElements()[0];

    cout << ligneDeSeparation << "AFFICHAGE DES FILMS (VÉRIFICATION DE LA COPIE ET DU PARTAGE) :" << endl;
    cout << skylien << endl;

    Film *filmTrouve = listeFilms.chercherFilm([](const Film *f) { return f->getTitre() == "Alien"; });

    cout << ligneDeSeparation << "LISTE GENERIQUE :" << endl;

    Liste<string> listeTextes;
    listeTextes.initialiser(2);
    listeTextes.modifierElement(0, make_shared<string>("Texte Initial A"));
    listeTextes.modifierElement(1, make_shared<string>("Texte Initial B"));

    Liste<string> listeTextes2 = listeTextes;

    listeTextes[0] = make_shared<string>("Nouveau Texte A");
    *listeTextes[1] = "Texte B Modifie";

    cout << "listeTextes[0]  : " << *listeTextes[0] << endl;
    cout << "listeTextes[1]  : " << *listeTextes[1] << endl;
    cout << "listeTextes2[0] : " << *listeTextes2[0] << endl;
    cout << "listeTextes2[1] : " << *listeTextes2[1] << endl;

    // Question 1.5 : Implementation du code d'itterateur pour notre type liste 
    
    // index 0
    if (listeFilms.getNElements() > 0)
    {
        const Film& premierFilm = *listeFilms.getElements()[0];

        for (auto&& acteur : premierFilm.getActeurs())
        {
            if (acteur != nullptr)
            {
                cout << "  " << acteur->nom << endl;
            }
        }
    }

    if (listeFilms.getNElements() > 0)
    {
        Film *filmADetruire = listeFilms.getElements()[0];
        listeFilms.enleverFilm(filmADetruire);
        detruireFilm(filmADetruire);
    }

    // MISE EN PLACE DE LA BIBLIOTHÈQUE INTERMÉDIAIRE (QUESTIONS 2, 3, 4)
    vector<Item*> bibliotheque;

    for (int i = 0; i < listeFilms.getNElements(); i++)
    {
        bibliotheque.push_back(listeFilms.getElements()[i]);
    }
    listeFilms.detruire(); 

    lireLivres("livres.txt", bibliotheque);

    Film* filmHobbit = nullptr;
    Livre* livreHobbit = nullptr;
    for (Item* item : bibliotheque)
    {
        if (item->getTitre() == "Le Hobbit : La Bataille des Cinq Armées")
            filmHobbit = dynamic_cast<Film*>(item);
        if (item->getTitre() == "The Hobbit")
            livreHobbit = dynamic_cast<Livre*>(item);
    }
    if (filmHobbit && livreHobbit)
    {
        bibliotheque.push_back(new FilmLivre(*filmHobbit, *livreHobbit));
    }

    cout << ligneDeSeparation << "Les films sont maintenant:" << endl;
    // TODO: Afficher la liste des films.
    afficherListeItems(bibliotheque);

    // Question 1.1 : Copie du vecteur vers une forward_list en préservant l'ordre
    forward_list<Item*> listeLiee(bibliotheque.begin(), bibliotheque.end());

    // Question 1.2 : Copier la liste à l'envers en O(n) à l'aide de push_front
    forward_list<Item*> listeInverse;
    for (const auto& item : listeLiee)
    {
        listeInverse.push_front(item);
    }

    // Question 1.3 : Copier la liste dans le même ordre en O(n) via insert_after
    forward_list<Item*> listeMemeOrdre;
    if (!listeLiee.empty())
    {
        listeMemeOrdre.push_front(listeLiee.front());
        auto itDernier = listeMemeOrdre.begin();
        auto itOriginal = listeLiee.begin();
        itOriginal++; 
        while (itOriginal != listeLiee.end())
        {
            itDernier = listeMemeOrdre.insert_after(itDernier, *itOriginal);
            itOriginal++;
        }
    }

    // Question 1.4 : Copier la liste à l'envers dans un vector en O(n)
    // Complexité : O(n) temporel car le parcours de la liste et l'inversion du vecteur 
    // se font tous deux en temps linéaire par rapport au nombre d'éléments.
    vector<Item*> vecteurInverse;
    for (const auto& item : listeLiee)
    {
        vecteurInverse.push_back(item);
    }
    if (!vecteurInverse.empty())
    {
        size_t gauche = 0;
        size_t droite = vecteurInverse.size() - 1;
        while (gauche < droite)
        {
            swap(vecteurInverse[gauche], vecteurInverse[droite]);
            gauche++;
            droite--;
        }
    }

    // Question 2.1 : Utilisation d'un std::set 
    auto comparateurItem = [](const Item* a, const Item* b) {
        return a->getTitre() < b->getTitre();
    };
    
    set<Item*, decltype(comparateurItem)> bibliothequeTrie(comparateurItem);
    for (const auto& item : bibliotheque)
    {
        bibliothequeTrie.insert(item);
    }
    cout << ligneDeSeparation << "Bibliothèque en ordre alphabétique (Question 2.1) :" << endl;
    afficherListeItems(bibliothequeTrie);

    // Question 2.2 : Utilisation d'un std::unordered_map pour une recherche en O(1) en moyenne
    unordered_map<string, Item*> tableBibliotheque;
    for (const auto& item : bibliotheque)
    {
        if (item != nullptr)
        {
            tableBibliotheque[item->getTitre()] = item;
        }
    }

    // Test de la Recherche sur  l'item "The Hobbit" 
    cout << ligneDeSeparation << "Recherche et affichage de 'The Hobbit' (Question 2.2) :" << endl;
    auto itTrouve = tableBibliotheque.find("The Hobbit");

    if (itTrouve != tableBibliotheque.end())
    {
        // On récupère le pointeur d'Item associé à la clé
        Item* itemHobbit = itTrouve->second;
        if (itemHobbit != nullptr)
        {
            itemHobbit->afficher(cout);
            cout << endl;
        }
    }

    // TODO: Faire les appels qui manquent pour avoir 0% de lignes non exécutées dans le programme (aucune ligne rouge dans la couverture de code; c'est normal que les lignes de "new" et "delete" soient jaunes).  Vous avez aussi le droit d'effacer les lignes du programmes qui ne sont pas exécutée, si finalement vous pensez qu'elle ne sont pas utiles.
    if (filmTrouve != nullptr) {
        filmTrouve = nullptr;
    }

    // TODO: Détruire tout avant de terminer le programme.  La bibliothèque de verification_allocation devrait afficher "Aucune fuite detectee." a la sortie du programme; il affichera "Fuite detectee:" avec la liste des blocs, s'il manque des delete.
    for (Item* item : bibliotheque)
    {
        delete item;
    }
    bibliotheque.clear();

    return 0;
}