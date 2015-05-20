#include "evenement.h"

Evenement::~Evenement(){}

void EvenementManager::addItem(Evenement* e){
if (nb==nbMax){
    Evenement** newtab=new Evenement*[nbMax+10];
for(unsigned int i=0; i<nb; i++) newtab[i]=evenements[i];
nbMax+=10;
Evenement** old=evenements;
evenements=newtab;
delete[] old;
}
evenements[nb++]=e;
}

Evenement* EvenementManager::trouverEvenement(const Tache_unitaire& t)const{
for(unsigned int i=0; i<nb; i++)
if (&t==&(dynamic_cast<ProgTache*>(evenements[i]))->getTache()) return evenements[i];
return 0;
}

Evenement* EvenementManager::trouverEvenement(const std::string& des) const{
for(unsigned int i=0; i<nb; i++)
if (des==(dynamic_cast<ProgActivite*>(evenements[i]))->getDescription()) return evenements[i];
return 0;
}

void EvenementManager::ajouterEvenement(const TIME::Date& d, const std::string& des, const TIME::Horaire& h, const std::string& l, const std::string& p){
if (trouverEvenement(des)) throw CalendarException("erreur, EvenementManager, Evenement deja existant");
ProgActivite* newA=new ProgActivite(d,des,h,l,p);
addItem(newA);
}

void EvenementManager::ajouterEvenement(const Tache_unitaire& t, const TIME::Date& d, const TIME::Horaire& h){
if (trouverEvenement(t)) throw CalendarException("erreur, ProgrammationManager, Programmation deja existante");
ProgTache* newT=new ProgTache(t,d,h);
addItem(newT);
}

EvenementManager::~EvenementManager(){
    for(unsigned int i=0; i<nb; i++) delete evenements[i];
    delete[] evenements;
    }
int EvenementManager::predProgrammee(const Tache& pred,const Tache& succ)const{
    Evenement* ePred = trouverEvenement(pred);
    Evenement* eSucc = trouverEvenement(succ);
    if(ePred==0) return 0;
    unsigned short int horairemin = (ePred->getHoraire().getHeure()*60)+(ePred->getHoraire().getMinute());
    horairemin+=TIME::Duree::getDureeEnMinutes(Tache_unitaire::getDuree());
    if(horairemin>=24*60){
        horairemin-=24*60;
        TIME::Date nouveauJour=ePred->getDate().demain();
    }
    else {
        nouveauJour=ePred->getDate();}
    if (nouveauJour<eSucc->getDate())
        return 1;
    else if(nouveauJour==eSucc->getDate() && horairemin<eSucc->getHoraire)
        return 1;
    else return 0;
}

/*
EvenementManager::EvenementManager(const EvenementManager& autre_manager):nb(autre_manager.nb),nbMax(autre_manager.nbMax), evenements(new Evenement*[autre_manager.nb]){
for(unsigned int i=0; i<nb; i++) evenements[i]=(Evenement*)new Evenement(*autre_manager.evenements[i]);
}
EvenementManager& EvenementManager::operator=(const EvenementManager& autre_manager){
if (this==&autre_manager) return *this;
this->~EvenementManager();
for(unsigned int i=0; i<autre_manager.nb; i++) addItem((Evenement*)new Evenement(*autre_manager.evenements[i]));
return *this;
}
*/
