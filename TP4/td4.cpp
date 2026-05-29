/**
 * \mainpage Description du projet TP4
 * \file     td4.cpp
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

#include "cppitertools/range.hpp"
#include "gsl/span"

#include "bibliotheque_cours.hpp"
#include "verification_allocation.hpp" // Nos fonctions pour le rapport de fuites de mémoire.
#include "debogage_memoire.hpp"		   // Ajout des numéros de ligne des "new" dans le rapport de fuites.  Doit être après les include du système, qui peuvent utiliser des "placement new" (non supporté par notre ajout de numéros de lignes).

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
	fichier.read((char *)&texte[0], streamsize(sizeof(texte[0])) * texte.length());
	return texte;
}

#pragma endregion //}

// TODO: Une fonction pour ajouter un Film à une ListeFilms, le film existant déjà; on veut uniquement ajouter le pointeur vers le film existant.  Cette fonction doit doubler la taille du tableau alloué, avec au minimum un élément, dans le cas où la capacité est insuffisante pour ajouter l'élément.  Il faut alors allouer un nouveau tableau plus grand, copier ce qu'il y avait dans l'ancien, et éliminer l'ancien trop petit.  Cette fonction ne doit copier aucun Film ni Acteur, elle doit copier uniquement des pointeurs.

// ListeActeurs::ListeActeurs(int cap)
// {
// 	capacite = cap;
// 	nElements = cap;
// 	elements = make_unique<shared_ptr<Acteur>[]>(static_cast<size_t>(capacite));
// }

// Copie de la liste des acteur en cas de besoins au vu du fait que cela est un unique ptr qui ne peut pas etre copier directement
// ListeActeurs::ListeActeurs(const ListeActeurs &autre)
// {
// 	capacite = autre.capacite;
// 	nElements = autre.nElements;

// 	elements = make_unique<shared_ptr<Acteur>[]>(static_cast<size_t>(capacite));

// 	for (size_t i = 0; i < static_cast<size_t>(nElements); ++i)
// 	{
// 		elements[i] = autre.elements[i];
// 	}
// }

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
			// Décale tous les films suivants vers la gauche pour maintenir l'ordre
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
		for (const shared_ptr<Acteur> &acteur : span(film->acteurs.getElements(), static_cast<size_t>(film->acteurs.getNElements())))
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
	if (acteurExistant)
	{
		for (int i = 0; i < listeFilms.getNElements(); i++)
		{
			Film *f = listeFilms.getElements()[i];
			for (int j = 0; j < f->acteurs.getNElements(); j++)
			{
				// Vérification de sécurité pour éviter de lire un shared_ptr non assigné
				if (f->acteurs.getElements()[j] && f->acteurs.getElements()[j].get() == acteurExistant)
				{
					return f->acteurs.getElements()[j];
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
	film->titre = lireString(fichier);
	film->realisateur = lireString(fichier);
	film->anneeSortie = int(lireUintTailleVariable(fichier));
	film->recette = int(lireUintTailleVariable(fichier));
	// film->acteurs.nElements = int(lireUintTailleVariable(fichier)); // NOTE: Vous avez le droit d'allouer d'un coup le tableau pour les acteurs, sans faire de réallocation comme pour ListeFilms.  Vous pouvez aussi copier-coller les fonctions d'allocation de ListeFilms ci-dessus dans des nouvelles fonctions et faire un remplacement de Film par Acteur, pour réutiliser cette réallocation.
	film->acteurs.initialiser(int(lireUintTailleVariable(fichier)));
	for (size_t i = 0; i < static_cast<size_t>(film->acteurs.getNElements()); i++)
	{
		shared_ptr<Acteur> acteur = lireActeur(fichier, listeFilms);
		film->acteurs.modifierElement(i, acteur);
		// acteur->joueDans.ajouterFilm(film.get());
	}
	return film.release();
}

// Implementation de la fonction de recherche avec une fonction lambda

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
	// for (shared_ptr<Acteur>& acteur : span(film->acteurs.getElements(), static_cast<size_t>(film->acteurs.getNElements())))
	// {
	// 	acteur->joueDans.enleverFilm(film);
	// 	if (acteur->joueDans.getNElements() == 0)
	// 	{
	// 		cout << "Destruction acteur: " << acteur->nom << endl;
	// 		acteur->joueDans.detruire();
	// 		delete acteur;
	// 	}
	// }
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

ostream &operator<<(std::ostream &os, const Film &film)
{
	os << "Titre       : " << film.titre << endl;
	os << "Réalisateur : " << film.realisateur << endl;
	os << "Année       : " << film.anneeSortie << endl;
	os << "Recette     : " << film.recette << " M$" << endl;
	os << "Acteurs:" << endl;
	for (const shared_ptr<Acteur> &acteur : span(film.acteurs.getElements(), static_cast<size_t>(film.acteurs.getNElements())))
	{
		os << "  " << acteur->nom << ", " << acteur->anneeNaissance << " " << acteur->sexe << "\n";
	}
	return os;
}

void afficherListeFilms(const ListeFilms &listeFilms)
{
	static const string ligneDeSeparation = "\n\033[36m━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\033[0m\n";
	cout << ligneDeSeparation;
	for (const Film *film : span(listeFilms.getElements(), static_cast<size_t>(listeFilms.getNElements())))
	{
		cout << *film << endl;
		cout << ligneDeSeparation;
	}
}

// void afficherFilmographieActeur(const ListeFilms &listeFilms, const string &nomActeur)
// {
// 	const Acteur *acteur = trouverActeur(listeFilms, nomActeur);
// 	if (acteur == nullptr)
// 		cout << "Aucun acteur de ce nom" << endl;
// 	else
// 		afficherListeFilms(acteur->joueDans);
// }

int main()
{
	bibliotheque_cours::activerCouleursAnsi(); // Permet sous Windows les "ANSI escape code" pour changer de couleurs https://en.wikipedia.org/wiki/ANSI_escape_code ; les consoles Linux/Mac les supportent normalement par défaut.

	// int *fuite = new int; // TODO: Enlever cette ligne après avoir vérifié qu'il y a bien un "Fuite detectee" de "4 octets" affiché à la fin de l'exécution, qui réfère à cette ligne du programme.

	static const string ligneDeSeparation = "\n\033[35m════════════════════════════════════════\033[0m\n";

	// TODO: Chaque TODO dans cette fonction devrait se faire en 1 ou 2 lignes, en appelant les fonctions écrites.

	// TODO: La ligne suivante devrait lire le fichier binaire en allouant la mémoire nécessaire.  Devrait afficher les noms de 20 acteurs sans doublons (par l'affichage pour fins de débogage dans votre fonction lireActeur).
	ListeFilms listeFilms = creerListe("films.bin");

	cout << ligneDeSeparation << "Le premier film de la liste est:" << endl;
	// TODO: Afficher le premier film de la liste.  Devrait être Alien.
	if (listeFilms.getNElements() > 0)
	{
		cout << ligneDeSeparation << "Les films sont:" << endl;
		cout << **listeFilms.getElements() << endl;
	}

	cout << ligneDeSeparation << "Les films sont:" << endl;
	// TODO: Afficher la liste des films.  Il devrait y en avoir 7.
	afficherListeFilms(listeFilms);

	// TODO: Modifier l'année de naissance de Benedict Cumberbatch pour être 1976 (elle était 0 dans les données lues du fichier).  Vous ne pouvez pas supposer l'ordre des films et des acteurs dans les listes, il faut y aller par son nom.
	const Acteur *acteurBenedict = trouverActeur(listeFilms, "Benedict Cumberbatch");
	if (acteurBenedict)
		const_cast<Acteur *>(acteurBenedict)->anneeNaissance = 1976;

	cout << ligneDeSeparation << "Liste des films où Benedict Cumberbatch joue sont:" << endl;
	// TODO: Afficher la liste des films où Benedict Cumberbatch joue.  Il devrait y avoir Le Hobbit et Le jeu de l'imitation.
	// afficherFilmographieActeur(listeFilms, "Benedict Cumberbatch");

	// TODO: Détruire et enlever le premier film de la liste (Alien).  Ceci devrait "automatiquement" (par ce que font vos fonctions) détruire les acteurs Tom Skerritt et John Hurt, mais pas Sigourney Weaver puisqu'elle joue aussi dans Avatar.

	// Operation sur le film Skylien

	// Creation du film
	Film skylien = *listeFilms.getElements()[0];

	// Changement du titre du film skylien pour "Skylien"
	skylien.titre = "Skylien";

	// changement du premier acteur
	skylien.acteurs.modifierElement(0, listeFilms.getElements()[1]->acteurs.getElements()[0]);

	// changement du nom du premier acteur
	skylien.acteurs.getElements()[0]->nom = "Daniel Wroughton Craig";

	// Affichage des modifications pour ce film
	cout << ligneDeSeparation << "AFFICHAGE DES FILMS (VÉRIFICATION DE LA COPIE ET DU PARTAGE) :" << endl;
	cout << skylien << endl;

	// test de la fonction de recherche
	Film *filmTrouve = listeFilms.chercherFilm([](const Film *f)
											   { return f->recette == 955; });
	if (filmTrouve)
	{
		cout << "Film trouve: " << filmTrouve->titre << endl;
	}

	// consigne chapitre 9
	cout << ligneDeSeparation << "LISTE GENERIQUE :" << endl;

	// Étape 2 : Construction d'une Liste<string> et ajout de deux éléments
	Liste<string> listeTextes;
	listeTextes.initialiser(2);
	listeTextes.modifierElement(0, make_shared<string>("Texte Initial A"));
	listeTextes.modifierElement(1, make_shared<string>("Texte Initial B"));

	// Copie de la liste dans une nouvelle liste
	Liste<string> listeTextes2 = listeTextes;

	// Étape 3 : Remplacement du premier et modification du texte du deuxième
	listeTextes[0] = make_shared<string>("Nouveau Texte A");
	*listeTextes[1] = "Texte B Modifie";

	// Étape 4 : Affichage des 4 textes
	cout << "listeTextes[0]  : " << *listeTextes[0] << endl;
	cout << "listeTextes[1]  : " << *listeTextes[1] << endl;
	cout << "listeTextes2[0] : " << *listeTextes2[0] << endl;
	cout << "listeTextes2[1] : " << *listeTextes2[1] << endl;

	if (listeFilms.getNElements() > 0)
{
    Film *filmADetruire = listeFilms.getElements()[0]; 
    listeFilms.enleverFilm(filmADetruire);         
    detruireFilm(filmADetruire);                  
}

	cout << ligneDeSeparation << "Les films sont maintenant:" << endl;
	// TODO: Afficher la liste des films.
	afficherListeFilms(listeFilms);

	// TODO: Faire les appels qui manquent pour avoir 0% de lignes non exécutées dans le programme (aucune ligne rouge dans la couverture de code; c'est normal que les lignes de "new" et "delete" soient jaunes).  Vous avez aussi le droit d'effacer les lignes du programmes qui ne sont pas exécutée, si finalement vous pensez qu'elle ne sont pas utiles.

	// TODO: Détruire tout avant de terminer le programme.  La bibliothèque de verification_allocation devrait afficher "Aucune fuite detectee." a la sortie du programme; il affichera "Fuite detectee:" avec la liste des blocs, s'il manque des delete.
	detruireListeFilms(listeFilms);

	return 0;
}