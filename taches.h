#if !defined(_TACHES_H)
#define _TACHES_H
#include <iostream>
#include <string>
//#include "timing.h"
/* ****************************************************TACHES**************************************************** */
class Tache {  //la classe est abstraite
	private:
	string titre;
	Date date_disponibilite;
	Date echeance;
	Tache(const Tache& t); //empêche la duplication  (constructeur de recopie)
	Tache& operator=(const Tache& t);
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
	Tache(const string& t, const Duree& dur, const Date& dispo, const Date& deadline): Tache(t, dispo, deadline), duree (dur) {}
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
	Tache(const string& t, const Duree& dur, const Date& dispo, const Date& deadline): Tache(t, dispo, deadline) {}
	~Tache_composite() {
};


/* *********************************** FABRIQUE TACHES ******************************************** */

class FabriqueTache {
public:
 virtual Tache_unitaire *createTache_unitaire() = 0;
 virtual Tache_composite *createTache_composite() = 0;
};


}
#endif
