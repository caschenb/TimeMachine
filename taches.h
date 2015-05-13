#if !defined(_TACHES_H)
#define _TACHES_H
#include <iostream>
#include <string>
#include <vector>
#include "Z:\LO21\TD5\timing.h"

/**************************************************** EXCEPTIONS ***************************************/
class CalendarException{
public:
	CalendarException(const std::string& message):info(message){}
	std::string getInfo() const { return info; }
private:
	std::string info;
};


/***************************************************** TACHES *****************************************************/
class Tache {  //la classe est abstraite
	private:
	std::string identificateur;
	std::string titre;
	TIME::Date date_disponibilite;
	TIME::Date echeance;
	public:
	//Tache(const Tache& t); //empêche la duplication  (constructeur de recopie)
	//Tache& operator=(const Tache& t);
	Tache(const std::string& id, const std::string& t, const TIME::Date& dispo, const TIME::Date& deadline):
			identificateur(id), titre(t), date_disponibilite(dispo), echeance(deadline){}
	//friend Tache& TacheManager::ajouterTache(const string& id, const string& t, const Duree& dur, const Date& dispo, const Date& deadline);
    public:
    virtual ~Tache() {};
    std::string getId() const { return identificateur; }
	std::string getTitre() const { return titre; }
	TIME::Date getDateDisponibilite() const {  return date_disponibilite; }
	TIME::Date getDateEcheance() const { return echeance; }
	//exemple de fonction virtuelle virtual void afficher(std::ostream& f= std::cout) const =0;
};

class Tache_unitaire : public Tache {
	private:
	TIME::Duree duree;
	public:
	Tache_unitaire(const std::string& id, const std::string& t, const TIME::Duree& dur, const TIME::Date& dispo, const TIME::Date& deadline): Tache(id, t, dispo, deadline), duree (dur) {}
	TIME::Duree getDuree() const { return duree; }
};

class Tache_composite : public Tache {
	private:
	typedef std::vector<Tache*> contTache;
	contTache taches_composees;
	//recopie et affectation interdite???
	Tache_composite& operator<<(Tache& t ){
        taches_composees.push_back(&t);
        return *this;
    }
	virtual ~Tache_composite(){} //a faire
	Tache_composite(const std::string& id, const std::string& t, const TIME::Duree& dur, const TIME::Date& dispo, const TIME::Date& deadline): Tache(id, t, dispo, deadline) {
	    taches_composees.reserve(10);}
};

class Tache_preempte: public Tache_unitaire {
	public:
	Tache_preempte(const std::string& id, const std::string& t, const TIME::Duree& dur, const TIME::Date& dispo, const TIME::Date& deadline): Tache_unitaire(id, t, dur, dispo, deadline) {}
	};

class Tache_non_preempte: public Tache_unitaire {
	public:
	Tache_non_preempte(const std::string& id, const std::string& t, const TIME::Duree& dur, const TIME::Date& dispo, const TIME::Date& deadline): Tache_unitaire(id, t, dur, dispo, deadline) {}
	};

/******************************************** TACHE MANAGER **********************************************/
//attention a penser a declarer tachemanager avant tache ou le contraire en cas d'erreur
class TacheManager {
private:
	Tache** taches;
	unsigned int nb;
	unsigned int nbMax;
	void addItem(Tache* t);
	Tache* trouverTache(const std::string& id) const;
	std::string file;
	  //on doit bloquer toutes les possibilités de
	  //création et de destruction de l'opérateur =>privé
	//TacheManager(const TacheManager& um);
	TacheManager& operator=(const TacheManager& um);
	struct Handler {
	    TacheManager* instanceUnique;
	    Handler(): instanceUnique(0) {}
	    ~Handler() {delete instanceUnique;}
	};
	static Handler handler;
	public:
	TacheManager();
	~TacheManager();
	Tache& ajouterTache(const std::string& id, const std::string& t,const TIME::Date& dispo, const TIME::Date& deadline);
	Tache& getTache(const std::string& id);
	const Tache& getTache(const std::string& code) const;
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
           if(indice_tache>=getInstance().nb)  //singleton permet d'acceder à cela n'importe ou dans le code.
           throw "indirection d'un itérateur en fin de séquence";
           return *getInstance().taches[indice_tache];
           }
	    bool isDone() const {return indice_tache==getInstance().nb;}
        void  next () {
            if(indice_tache>=getInstance().nb)  //singleton permet d'acceder à cela n'importe ou dans le code.
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
           if(indice_tache>=getInstance().nb)  //singleton permet d'acceder à cela n'importe ou dans le code.
           throw "indirection d'un itérateur en fin de séquence";
           return *getInstance().taches[indice_tache];
           }
	    bool isDone() const {return indice_tache==getInstance().nb;}
        void  next () ;
	};
	DisponibiliteFilterIterator getDisponibiliteFilterIterator(const TIME::Date& d)
	{
	    return DisponibiliteFilterIterator(d);
	}
};

/*************************************** PROJET ******************************************************/

class Projet {
    private:
    TIME::Date date_dispo;
    TIME::Date echeance;
    Tache** taches;
	unsigned int nb;
	unsigned int nbMax;
	~Projet(){
    		for (unsigned int i=0; i<nb; i++) delete taches[i];
    		delete[] taches;
    		nb=0;
    		nbMax=0;
    		taches=0;
		}
    public:
    Projet(const TIME::Date& d, const TIME::Date e): date_dispo(d), echeance(e), taches(0){}
};


std::ostream& operator<<(std::ostream& f, const Tache& t);
//std::ostream& operator<<(std::ostream& f, const Programmation& p);
#endif
