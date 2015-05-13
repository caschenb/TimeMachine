#if !defined(_EVENEMENT_H)
#define _EVENEMENT_H
#include "timing.h"

class Evenement {
private:
    Date date;
    Horaire horaire;
public:
    Evenement()=0;
    virtual void afficher() const=0;
    const std::Date& getDate() const { return date; }
    const std::Horaire& getHoraire() const { return horaire; }

    };

class ProgActivite: public Evenement {
private:
    std::string description;
    std::string lieu;
    std::string personnes;
public:
    ProgActivite(const Date& d, const std::string& des, const Horaire& h, const std::string& l, const std::string& p):Evenement(d,h),description(des),lieu(l),personnes(p){}
    const std::string& getDescription() const { return description; }
    const std::string& getLieu() const {return lieu;}
    const std::string& getPersonnes() const {return personnes;}
    void afficher(std::ostream& f= std::cout) const {
        f << "Description=" << decription << " Date=" << date << "\n" << "Horaire=" << horaire << " Lieu=" << lieu << "\n" << " Personnes=" << personnes << "\n";
        }
    ~ProgActivite();
};

class ProgTache: public Evenement {
	const Tache_unitaire* tache;
public:
	ProgTache(const Tache_unitaire& t, const Date& d, const Horaire& h):tache(&t), Evenement(d,h){}
	const Tache_unitaire& getTache() const { return *tache; }

};

class EvenementManager {
private:
	Evenement** evenement;
	unsigned int nb;
	unsigned int nbMax;
	void addItem(Evenement* t);
	//Evenement* trouverEvenement(const Tache& t) const;
public:
	EvenementManager();
	~EvenementManager();
	EvenementManager(const ProgrammationManager& um);
	EvenementManager& operator=(const EvenementnManager& um);
	//void ajouterEvenement(const Tache& t, const Date& d, const Horaire& h);
	void ajouterProgActivite(const Date& d, const std::string& des, const Horaire& h, const std::string& l, const std::string& p);
	void ajouterProgTache(const Tache_unitaire& t, const Date& d, const Horaire& h);
};

#endif


