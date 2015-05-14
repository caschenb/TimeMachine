#if !defined(_TACHES_H)
#define _TACHES_H
#include <iostream>
#include <string>
#include <vector>
#include "C:\Users\celine H\Desktop\UTC\LO21\TD5\timing.h" //penser a le mettre à jour à chaque fois

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
//attention a penser a declarer tachemanager avant tache ou le contraire en cas d'erreur
class TacheManager {
private:
	Tache** taches;
	unsigned int nb;
	unsigned int nbMax;
	void addItem(Tache* t);

    virtual Tache*  trouverTache(const std::string& id) const;
	std::string file;
	  //on doit bloquer toutes les possibilités de
	  //création et de destruction de l'opérateur =>privé
	//TacheManager(const TacheManager& um);
	TacheManager& operator=(const TacheManager& um);
	struct Handler {
	    TacheManager* instanceUnique;
	    Handler(): instanceUnique(0) {}
	    ~Handler() {if (instanceUnique) delete instanceUnique;}
	};
	static Handler handler;
public:
	TacheManager();
	virtual ~TacheManager();
	Tache& ajouterTache(const std::string& id, const std::string& t,const TIME::Date& dispo, const TIME::Date& deadline);
	Tache_unitaire& ajouterTache(const std::string& id, const std::string& t, const TIME::Duree& dur, const TIME::Date& dispo, const TIME::Date& deadline);
    Tache_composite& ajouterTacheComposite(const std::string& id, const std::string& t, const TIME::Date& dispo, const TIME::Date& deadline);
	void ajouterSousTache (const std::string& id_composite, const std::string& id_sous_tache);
	virtual Tache& getTache(const std::string& id);
	//Tache_composite& getTacheComposite(const std::string& id);
    bool isTacheExistante(const std::string& id) const { return trouverTache(id)!=0; }
	const Tache& getTache(const std::string& code) const;
	void load(const std::string& f);
	void save(const std::string& f);
	friend class Tache_composite;
	void parcourirTacheComposite(const std::string&id);
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
    /*   Tache& currentc() const {
           if(indice_tache>=getInstance().nb)  //singleton permet d'acceder à cela n'importe ou dans le code.
           throw "indirection d'un itérateur en fin de séquence";
           return *getInstance().Tache_composite::taches_composees[indice_tache];
           }*/
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

/***************************************************** TACHES *****************************************************/

class Tache {  //la classe est abstraite!!! a changer
private:
	std::string identificateur;
	std::string titre;
	TIME::Date date_disponibilite;
	TIME::Date echeance;
public:
    Tache(const Tache& t);
    Tache& operator=(const Tache& t);
	Tache(const std::string& id, const std::string& t, const TIME::Date& dispo, const TIME::Date& deadline):
			identificateur(id), titre(t), date_disponibilite(dispo), echeance(deadline){}
    friend Tache& TacheManager::ajouterTache(const std::string& id, const std::string& t, const TIME::Date& dispo, const TIME::Date& deadline);
public:
    virtual ~Tache() {};
    std::string getId() const { return identificateur; }
    void setId(const std::string& str);
	std::string getTitre() const { return titre; }
	void setTitre(const std::string& str) {titre=str;}
	TIME::Date getDateDisponibilite() const {  return date_disponibilite; }
	TIME::Date getDateEcheance() const { return echeance; }
	void setDatesDisponibiliteEcheance(const TIME::Date& disp, const TIME::Date& e) {
        if (e<disp) throw CalendarException("erreur Tache : date echeance < date disponibilite");
        date_disponibilite=disp; echeance=e;
    }
    virtual void afficher(std::ostream& fout= std::cout) const;
};

std::ostream& operator<<(std::ostream& f, const Tache& t);

class Tache_unitaire : public Tache {
private:
	TIME::Duree duree;
	bool preemptive;
public:
	Tache_unitaire(const std::string& id, const std::string& t, const TIME::Duree& dur, const TIME::Date& dispo, const TIME::Date& deadline, bool preempt=false ): Tache(id, t, dispo, deadline), duree (dur), preemptive(preempt) {}
	TIME::Duree getDuree() const { return duree; }
	bool isPreemptive() const { return preemptive; }
    void setPreemptive() { preemptive=true; }
    void setNonPreemptive() { preemptive=false; }
    friend Tache_unitaire& TacheManager::ajouterTache(const std::string& id, const std::string& t, const TIME::Duree& dur, const TIME::Date& dispo, const TIME::Date& deadline);
    void afficher(std::ostream& fout) const;
};

std::ostream& operator<<(std::ostream& fout, const Tache_unitaire& t);

class Tache_composite : public Tache {

//private:
protected:
	typedef std::vector<Tache*> contTache;
	contTache taches_composees;
        //recopie et affectation interdite???
     //   friend Tache& TacheManager::ajouterTache(const std::string& id, const std::string& t,const TIME::Date& dispo, const TIME::Date& deadline);

public:
  /*  Tache_composite& ajouterTacheComp(const TacheManager& m,const std::string& id, const std::string& t, const TIME::Date& dispo, const TIME::Date& deadline){
	    m.TacheManager::ajouterTache(id,t,dispo,deadline);
        taches_composees.push_back(&m);
       return *this;
    }*/
    friend class TacheManager;
    Tache_composite& ajouterTacheComp(Tache& t ){
        taches_composees.push_back(&t);
        return *this;
    }
	virtual ~Tache_composite(){} //a faire
	Tache_composite(const std::string& id, const std::string& t, const TIME::Date& dispo, const TIME::Date& deadline): Tache(id, t, dispo, deadline) {
	    taches_composees.reserve(10);}
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
   /* Projet& ajouterTacheP(Tache& t ){
        taches.push_back(&t);
        return *this;
    } */
};


#endif
