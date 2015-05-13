#if !defined(_TACHES_H)
#define _TACHES_H
#include <iostream>
#include <string>
#include "timing.h"
/* ****************************************************TACHES**************************************************** */
class Tache {  //la classe est abstraite
	private:
	string titre;
	Date date_disponibilite;
	Date echeance;
	public:
	//Tache(const Tache& t); //empêche la duplication  (constructeur de recopie)
	//Tache& operator=(const Tache& t);
	Tache(const string& t, const Date& dispo, const Date& deadline):
			titre(t), date_disponibilite(dispo), echeance(deadline){}
	//friend Tache& TacheManager::ajouterTache(const string& id, const string& t, const Duree& dur, const Date& dispo, const Date& deadline);

public:
  virtual ~Tache() {}; 
	string getTitre() const { return titre; }
	Date getDateDisponibilite() const {  return date_disponibilite; }
	Date getDateEcheance() const { return echeance; }
	//exemple de fonction virtuelle virtual void afficher(std::ostream& f= std::cout) const =0;
};

class Tache_unitaire : public Tache {
	private:
	Duree duree; 
	public:
	Tache_unitaire(const string& t, const Duree& dur, const Date& dispo, const Date& deadline): Tache(t, dispo, deadline), duree (dur) {}
	Duree getDuree() const { return duree; }
};

class Tache_composite : public Tache {
	private:
	Tache** taches;
	unsigned int nb;
	unsigned int nbMax;
	~Tache_composite(){
    		for (unsigned int i=0; i<nb; i++) delete taches[i];
    		delete[] taches;
    		nb=0;
    		nbMax=0;
    		taches=0;
		}
	Tache_composite(const string& t, const Duree& dur, const Date& dispo, const Date& deadline): Tache(t, dispo, deadline) {}
};

class Tache_preempte: public Tache_unitaire {
	public:	
	Tache_preempte(const string& t, const Duree& dur, const Date& dispo, const Date& deadline): Tache_unitaire(t, dur, dispo, deadline) {}
	};

class Tache_non_preempte: public Tache_unitaire {
	public:	
	Tache_non_preempte(const string& t, const Duree& dur, const Date& dispo, const Date& deadline): Tache_unitaire(t, dur, dispo, deadline) {}
	};	
	
/******************************************** TACHE MANAGER**********************************************/
//attention a penser a declarer tachemanager avant tache ou le contraire en cas d'erreur
class TacheManager {
private:
	Tache** taches;
	unsigned int nb;
	unsigned int nbMax;
	void addItem(Tache* t);
	Tache* trouverTache(const string& id) const;
	string file;
	TacheManager();   //on doit bloquer toutes les possibilités de
	~TacheManager();  //création et de destruction de l'opérateur =>privé
	//TacheManager(const TacheManager& um);
	TacheManager& operator=(const TacheManager& um);
	struct Handler {
	    TacheManager* instanceUnique;
	    Handler(): instanceUnique(0) {}
	    ~Handler() {delete instanceUnique;}
	};
	static Handler handler;
	public:
	Tache& ajouterTache(const string& id, const string& t, const Duree& dur, const Date& dispo, const Date& deadline);
	Tache& getTache(const string& id);
	const Tache& getTache(const string& code) const;
	void load(const string& f);
	void save(const string& f);
	static TacheManager& getInstance();
	static void libererInstance();

	class iterator {
	      int indice_tache;
	    public:
	    iterator():indice_tache(0){}
        Tache& operator*() const { if(indice_tache>=getInstance().nb)  //singleton permet d'acceder à cela n'importe ou dans le code.
           throw "indirection d'un itérateur en fin de séquence";
           return *getInstance().taches[indice_tache];
           }
           iterator& operator++() {
               if(indice_tache>=getInstance().nb)  //singleton permet d'acceder à cela n'importe ou dans le code.
           throw "incrementation d'un itérateur en fin de séquence";
           ++indice_tache;
           return *this;
           }
            bool operator!=(iterator it) const {
                 return indice_tache!=it.indice_tache;
                 }
            bool operator==(iterator it) const {
                 return indice_tache==it.indice_tache;
                 }

            friend class TacheManager;
	};
    iterator begin() {return iterator(); }
    iterator end() {
        iterator tmp;
         tmp.indice_tache=nb;
         return tmp;
    }
    /*classe d’iterateur qui permet de parcourir l’ensemble des objets Tache dont la date de disponibilité
est avant une date donnée, peut être utile plus tard */
    friend class Tache;
    friend class DisponibiliteFilterIterator;
    class DisponibiliteFilterIterator{
	      int indice_tache;
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
#endif
