#if !defined(_TACHES_H)
#define _TACHES_H
#include <iostream>
#include <string>
//#include "timing.h"

class Tache {  //la classe est abstraite
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

#endif
