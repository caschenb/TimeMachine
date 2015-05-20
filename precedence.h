#if !defined(_PRECEDENCE_H)
#define _PRECEDENCE_H
#include "taches.h"
class Precedence {
private:
Tache* pred;
Tache*succ;
public:
Precedence(Tache& i, Tache& j): pred(&i),succ(&j){}
Tache& getPredecesseur() const {return *pred;}
Tache& getSuccesseur() const {return *succ;}
};
class PrecedenceManager {
private:
Precedence** tab;
int nb;
int nbMax;
static PrecedenceManager* instance;
PrecedenceManager& operator=(const PrecedenceManager& m);
PrecedenceManager(const PrecedenceManager& m);
PrecedenceManager();
~PrecedenceManager();
void addItem(Precedence* t);
public:
static PrecedenceManager& getInstance();
static void libererInstance();
int tacheProgrammable(const Tache& t)const;

};
#endif
/*
class Projet;
class ProjetManager { //a mettre en singleton
private:
Projet** projets;
unsigned int nb;
unsigned int nbMax;
void addItem(Projet* t);
//constructeur de recopie et operateur d'affectation uniquement déclarés car (futur) singleton
ProjetManager(const ProjetManager& p);
ProjetManager& operator=(const ProjetManager& p);
Projet* trouverProjet(const std::string& id) const; //on y a acces via getProjet
public:
ProjetManager(): projets(0), nb(0), nbMax(0) {}
virtual ~ProjetManager();
void ajouterTacheProjet( TacheManager& m, const std::string& id_projet, const std::string& id_tache);
void parcourirTacheProjet(const std::string&id);
Projet& getProjet(const std::string& id); //vérifie que le projet existe avant d'appeler trouverProjet
Projet& ajouterProjet(const std::string& id, const std::string& t, const TIME::Date& dispo, const TIME::Date& deadline);
};
*/
