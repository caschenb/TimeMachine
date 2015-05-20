#if !defined(_TACHES_H)
#define _TACHES_H
#include <iostream>
#include <string>
#include <vector>
//#include "Z:\LO21\TD5\timing.h" //penser a le mettre à jour à chaque fois
#include "timing.h"
#include "evenement.h"
#include "precedence.h"
/**************************************************** EXCEPTIONS ***************************************/
class CalendarException{
public:
CalendarException(const std::string& message):info(message){}
std::string getInfo() const { return info; }
private:
std::string info;
};
/******************************************** TACHE MANAGER **********************************************/
class Tache;
class Tache_unitaire;
class Tache_composite;
class TacheManager {
private:
Tache** taches;
unsigned int nb;
unsigned int nbMax;
std::string file;
TacheManager(const TacheManager& um); //empeche la duplication
TacheManager& operator=(const TacheManager& um);
void addItem(Tache* t); //était en private dans le TD
Tache* trouverTache(const std::string& id) const; //on accede à cette fonction via getTache
struct Handler {
TacheManager* instanceUnique;
Handler(): instanceUnique(0) {}
~Handler() {if (instanceUnique) delete instanceUnique;}
};
static Handler handler;
public:
TacheManager():taches(0),nb(0),nbMax(0){}
virtual ~TacheManager();
/* regler probleme entre 2 ajouter tache différent ou 1 qui reconnait tache composite unitaire, mais ne pas laisser les 2*/
Tache_composite& ajouterTache(const std::string& id, const std::string& t,const TIME::Date& dispo, const TIME::Date& deadline);
Tache_unitaire& ajouterTache(const std::string& id, const std::string& t, const TIME::Duree& dur, const TIME::Date& dispo, const TIME::Date& deadline, bool preempt);
Tache_composite& ajouterTacheComposite(const std::string& id, const std::string& t, const TIME::Date& dispo, const TIME::Date& deadline);
void ajouterSousTache (const std::string& id_composite, const std::string& id_sous_tache); //ajoute une tache à une tache composite
Tache& getTache(const std::string& id);
const Tache& getTache(const std::string& code) const;
bool isTacheExistante(const std::string& id) const { return trouverTache(id)!=0; }
void parcourirTacheComposite(const std::string&id);
void load(const std::string& f);
void save(const std::string& f);
static TacheManager& getInstance();
static void libererInstance();
friend class Iterator;
class Iterator {
unsigned int indice_tache;
public:
Iterator():indice_tache(0){};
Tache& current() const {
if(indice_tache>=getInstance().nb) //singleton permet d'acceder à cela n'importe ou dans le code.
throw "indirection d'un itérateur en fin de séquence";
return *getInstance().taches[indice_tache];
}
bool isDone() const {return indice_tache==getInstance().nb;}
void next () {
if(indice_tache>=getInstance().nb) //singleton permet d'acceder à cela n'importe ou dans le code.
throw "incrementation d'un itérateur en fin de séquence";
++indice_tache;
}
};
Iterator getIterator() {return Iterator();}
/*classe d’iterateur qui permet de parcourir l’ensemble des objets Tache dont la date de disponibilité
est avant une date donnée, peut être utile plus tard */
friend class Tache;
friend class DisponibiliteFilterIterator;
class DisponibiliteFilterIterator{
unsigned int indice_tache;
TIME::Date disp;
public:
DisponibiliteFilterIterator(const TIME::Date& d); //constructeur
Tache& current() const {
if(indice_tache>=getInstance().nb) //singleton permet d'acceder à cela n'importe ou dans le code.
throw "indirection d'un itérateur en fin de séquence";
return *getInstance().taches[indice_tache];
}
bool isDone() const {return indice_tache==getInstance().nb;}
void next () ;
};
DisponibiliteFilterIterator getDisponibiliteFilterIterator(const TIME::Date& d)
{
return DisponibiliteFilterIterator(d);
}
};
/***************************************************** TACHES *****************************************************/
class Tache { //la classe est abstraite!!! a changer
private:
std::string identificateur;
std::string titre;
TIME::Date date_disponibilite;
TIME::Date echeance;
bool in; /*permet de savoir si la tache est déjà dans une tache composite ou dans un projet, on prend l'hypothese qu'elle ne peut etre que dans une seule tache composite ou projet */
bool terminee;
Tache(const Tache& t);
Tache& operator=(const Tache& t);
public:
Tache(const std::string& id, const std::string& t, const TIME::Date& dispo, const TIME::Date& deadline):
identificateur(id), titre(t), date_disponibilite(dispo), echeance(deadline), in(0) {}
virtual ~Tache() {};
std::string getId() const { return identificateur; }
void setId(const std::string& str);
std::string getTitre() const { return titre; }
void setTitre(const std::string& str) {titre=str;}
TIME::Date getDateDisponibilite() const { return date_disponibilite; }
TIME::Date getDateEcheance() const { return echeance; }
void setDatesDisponibiliteEcheance(const TIME::Date& disp, const TIME::Date& e) {
if (e<disp) throw CalendarException("erreur Tache : date echeance < date disponibilite");
date_disponibilite=disp; echeance=e;
}
virtual void afficher(std::ostream& fout=std::cout) const=0;
bool isIn() const { return in; } //fonctions utilisées pour savoir si tache dans projet/tache composite
void setIn() { in=true; }
void setNotIn() { in=false; }
void setTerminee() { terminee=true; }
void setNotTerminee() { terminee=false; }
};
//std::ostream& operator<<(std::ostream& f, const Tache& t); inutile pour le moment
class Tache_unitaire : public Tache {
private:
TIME::Duree duree;
bool preemptive;
public:
Tache_unitaire(const std::string& id, const std::string& t, const TIME::Duree& dur, const TIME::Date& dispo, const TIME::Date& deadline, bool preempt=false ): Tache(id, t, dispo, deadline), duree (dur), preemptive(preempt) {
if (preempt && dur.getDureeEnMinutes()>60*12) throw CalendarException("duree tache trop grande!");} //une tache preemptive ne peut avoir une durée >12h
TIME::Duree getDuree() const { return duree; }
bool isPreemptive() const { return preemptive; }
void setPreemptive() { preemptive=true; }
void setNonPreemptive() { preemptive=false; }
void afficher(std::ostream& fout) const;
};
//std::ostream& operator<<(std::ostream& fout, const Tache_unitaire& t); inutile pour le moment
class Tache_composite : public Tache {
private:
typedef std::vector<Tache*> contTache;
contTache taches_composees;
public:
friend class TacheManager; //on utilise dans tache manager l'attribut taches_composees
Tache_composite& ajouterTacheComp(Tache& t ); //ajoute la tache dans le tableau de pointeurs
~Tache_composite(); //a faire
Tache_composite(const std::string& id, const std::string& t, const TIME::Date& dispo, const TIME::Date& deadline): Tache(id, t, dispo, deadline) { //definie la tache et reserve de la place pour le tableau de pointeurs
taches_composees.reserve(10);
}
void afficher(std::ostream& fout= std::cout) const {
Tache::afficher(fout);
}
};
/******************************************* PROJET MANAGER **********************************************/
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
/*************************************** PROJET ******************************************************/
class Projet {
private:
std::string identificateur;
std::string titre;
TIME::Date date_dispo;
TIME::Date echeance;
typedef std::vector<Tache*> contTache;
contTache taches_projet; //contient les taches appartenant au projet
public:
friend class ProjetManager; //ProjetManager utilise le tableau des taches du projet
virtual ~Projet(){}
Projet(const std::string& id, const std::string& t,const TIME::Date& d, const TIME::Date e): identificateur(id), titre(t), date_dispo(d), echeance(e){ taches_projet.reserve(10);}
Projet& ajouterTacheP(Tache& t ); //ajoute la tache au projet
std::string getId() const { return identificateur; }
void setId(const std::string& str);
std::string getTitre() const { return titre; }
void setTitre(const std::string& str) {titre=str;}
TIME::Date getDateDisponibilite() const { return date_dispo; }
TIME::Date getDateEcheance() const { return echeance; }
void setDatesDisponibiliteEcheance(const TIME::Date& disp, const TIME::Date& e) {
if (e<disp) throw CalendarException("erreur Projet : date echeance < date disponibilite");
date_dispo=disp; echeance=e;
}
};
#endif
