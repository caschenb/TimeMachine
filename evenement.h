#if !defined(_EVENEMENT_H)
#define _EVENEMENT_H
#include "timing.h"
#include "taches.h"

/********************************************************CLASSE MERE EVENEMENT***********************************************/
class Evenement {
private:
    TIME::Date date;
    TIME::Horaire horaire;
public:
    Evenement();
    Evenement(const TIME::Date& d, const TIME::Horaire& h):date(d), horaire(h){}
    virtual void afficher(std::ostream& f= std::cout) const=0;
    const TIME::Date& getDate() const { return date; }
    const TIME::Horaire& getHoraire() const { return horaire; }
    virtual ~Evenement();
    };

/********************************************************CLASSE FILLE1 PROG ACTIVITE***********************************************/
class ProgActivite: public Evenement {
private:
    std::string description;
    std::string lieu;
    std::string personnes;
public:
    ProgActivite(const TIME::Date& d, const std::string& des, const TIME::Horaire& h, const std::string& l, const std::string& p):Evenement(d,h),description(des),lieu(l),personnes(p){}
    const std::string& getDescription() const { return description; }
    const std::string& getLieu() const {return lieu;}
    const std::string& getPersonnes() const {return personnes;}
    void afficher(std::ostream& f= std::cout) const {
        f << "Description=" << description << " Date=" << getDate() << "\n" << "Horaire=" << getHoraire() << " Lieu=" << lieu << "\n" << " Personnes=" << personnes << "\n";
        }
    ~ProgActivite();
};

/********************************************************CLASSE FILLE2 PROG TACHE***********************************************/
class ProgTache: public Evenement {
	const Tache_unitaire* tache;
public:
	ProgTache(const Tache_unitaire& t, const TIME::Date& d, const TIME::Horaire& h):tache(&t), Evenement(d,h){}
	~ProgTache();
	const Tache_unitaire& getTache() const { return *tache; }
	void afficher(std::ostream& f= std::cout) const {
        f << "ID=" << tache->getId() << " Titre=" << tache->getTitre() << "\n" << "Date de disponibilité=" << tache->getDateDisponibilite() << " Date d'échéance=" << tache->getDateEcheance() << "\n" ;
        }

};

/********************************************************CLASSE EVENEMENT MANAGER***********************************************/
class EvenementManager {
private:
	Evenement** evenements;
	unsigned int nb;
	unsigned int nbMax;
	void addItem(Evenement* e);
	Evenement* trouverEvenement(const Tache_unitaire& t) const; //trouve la programmation d'une activité
	Evenement* trouverEvenement(const std::string& des) const; //trouve la programmation d'une tache
public:
	EvenementManager();
	~EvenementManager();
	EvenementManager(const EvenementManager& autre_manager);
	EvenementManager& operator=(const EvenementManager& autre_manager);
	void ajouterEvenement(const TIME::Date& d, const std::string& des, const TIME::Horaire& h, const std::string& l, const std::string& p); //ajout de la programmmation d'une activité
	void ajouterEvenement(const Tache_unitaire& t, const TIME::Date& d, const TIME::Horaire& h); //ajout de la programmation d'une tache
};

#endif


