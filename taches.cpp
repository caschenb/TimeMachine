#include "taches.h"
#include <fstream>
/***************************** AFFICHER ******************************/
void Tache::afficher(std::ostream& fout) const{
fout<<"****************************************************************\n";
fout<<getId()<<"\n";
fout<<getTitre()<<"\n";
fout<<getDateDisponibilite()<<"\n";
fout<<getDateEcheance()<<"\n";
}
void Tache_unitaire::afficher(std::ostream& fout) const{
Tache::afficher(fout);
fout<<duree<<"\n";
if (isPreemptive()) fout<<"tache preemptive\n";
}
/****************************************** TACHES ********************************/
void Tache::setId(const std::string& str){
if (TacheManager::getInstance().isTacheExistante((str))) throw CalendarException("erreur TacheManager : tache id deja existante");
identificateur=str;
}
Tache_composite::~Tache_composite() {
taches_composees.erase(taches_composees.begin(), taches_composees.end());
}
Tache_composite& Tache_composite::ajouterTacheComp(Tache& t ){
if (t.isIn()) throw CalendarException("tache deja dans un projet ou une tache composite!");
taches_composees.push_back(&t);
t.setIn();
return *this;
}
/*********************************************************** TACHE MANAGER ***************************************************************/
void TacheManager::addItem(Tache* t){
if (nb==nbMax){
Tache** newtab=new Tache*[nbMax+10];
for(unsigned int i=0; i<nb; i++) newtab[i]=taches[i];
// ou memcpy(newtab,taches,nb*sizeof(Tache*));
nbMax+=10;
Tache** old=taches;
taches=newtab;
delete[] old;
}
taches[nb++]=t;
}
Tache* TacheManager::trouverTache(const std::string& id) const{
for(unsigned int i=0; i<nb; i++)
if (id==taches[i]->getId()) return taches[i];
return 0;
}
TacheManager::~TacheManager(){
if (file!="") save(file);
for(unsigned int i=0; i<nb; i++) delete taches[i];
delete[] taches;
file="";
}
Tache_composite& TacheManager::ajouterTache(const std::string& id, const std::string& t, const TIME::Date& dispo, const TIME::Date& deadline){
if (trouverTache(id)) throw CalendarException("erreur, TacheManager, tache deja existante");
Tache_composite* newt=new Tache_composite(id,t,dispo,deadline);
addItem(newt);
std::cout<<"tache ajoutée: "<<t<<"\n";
return *newt;
}
Tache_unitaire& TacheManager::ajouterTache(const std::string& id, const std::string& t, const TIME::Duree& dur, const TIME::Date& dispo, const TIME::Date& deadline, bool preempt) {
if (trouverTache(id)) throw CalendarException("erreur, TacheManager, tache deja existante");
Tache_unitaire* newt=new Tache_unitaire(id,t,dur,dispo,deadline, preempt);
addItem(newt);
std::cout<<"tache ajoutée: "<<t<<"\n";
return *newt;
}
Tache_composite& TacheManager::ajouterTacheComposite(const std::string& id, const std::string& t, const TIME::Date& dispo, const TIME::Date& deadline){
if (trouverTache(id)) throw CalendarException("erreur, TacheManager, tache deja existante");
Tache_composite* newt=new Tache_composite(id,t,dispo,deadline);
addItem(newt);
std::cout<<"tache ajoutée: "<<t<<"\n";
return *newt;
}
void TacheManager::ajouterSousTache(const std::string& id_composite, const std::string& id_sous_tache){
Tache& ta=getTache(id_sous_tache);
Tache_composite* tc=dynamic_cast<Tache_composite*>(&getTache(id_composite));
if (tc->getDateEcheance()<ta.getDateEcheance()) std::cout<<"date d'écheance de la tache composite pas correcte, changez la avant de reessayer!\n";
else tc->ajouterTacheComp(ta);
}
Tache& TacheManager::getTache(const std::string& id){
Tache* t=trouverTache(id);
if (!t) throw CalendarException("erreur, TacheManager, tache inexistante");
return *t;
}
const Tache& TacheManager::getTache(const std::string& id)const{
return const_cast<TacheManager*>(this)->getTache(id);
}
void TacheManager::parcourirTacheComposite(const std::string&id){
Tache_composite* tc=dynamic_cast<Tache_composite*>(&getTache(id));
std::cout<<"Parcours des SousTaches de "<<tc->getTitre()<<"\n";
for (Tache_composite::contTache::const_iterator it=tc->taches_composees.begin();
it!=tc->Tache_composite::taches_composees.end(); ++it) (*it)->afficher();
}
void TacheManager::load(const std::string& f){
if (file!=f) this->~TacheManager();
file=f;
std::ifstream fin(file.c_str()); // open file
if (!fin) throw CalendarException("erreur, lors de l'ouverture du fichier de taches");
char tmp[256];
while (!fin.eof()&&fin.good()){
fin.getline(tmp,256); // get code
if (fin.bad()) throw CalendarException("erreur, fichier taches, lecture identificateur tache");
std::string id=tmp;
fin.getline(tmp,256); // get titre
if (fin.bad()) throw CalendarException("erreur, fichier taches, lecture titre tache");
std::string titre=tmp;
TIME::Duree duree;
fin>>duree;
if (fin.bad()) throw CalendarException("erreur, fichier taches, lecture duree tache");
TIME::Date dispo;
fin>>dispo;
if (fin.bad()) throw CalendarException("erreur, fichier taches, lecture date disponibilite");
TIME::Date echeance;
fin>>echeance;
if (fin.bad()) throw CalendarException("erreur, fichier taches, lecture date echeance");
while (fin.peek()=='\r') fin.ignore();
while (fin.peek()=='\n') fin.ignore();
std::cout<<"LOAD "<<id<<"-"<<titre<<"-"<<duree<<"-"<<dispo<<"-"<<echeance<<"\n";
ajouterTache(id,titre,dispo,echeance);
}
fin.close(); // close file
}
void TacheManager::save(const std::string& f){
file=f;
std::ofstream fout(f.c_str(),std::ofstream::trunc); // toutes les taches existantes sont écrasées
for(unsigned int i=0; i<nb; i++){
(taches[i])->afficher();
}
fout.close();
}
TacheManager::Handler TacheManager::handler=
TacheManager::Handler();
TacheManager& TacheManager::getInstance() {
if (handler.instanceUnique==0)
handler.instanceUnique=new TacheManager;
return *handler.instanceUnique;
}
void TacheManager::libererInstance(){
// if(instanceUnique!=0)
delete handler.instanceUnique;
handler.instanceUnique=0;
}
TacheManager::DisponibiliteFilterIterator::DisponibiliteFilterIterator(const TIME::Date& d): //constructeur
indice_tache(0), disp(d){
while (indice_tache<getInstance().nb && disp<getInstance().taches[indice_tache]->getDateDisponibilite() )
++indice_tache;
}
void TacheManager::DisponibiliteFilterIterator::next () {
if(indice_tache>=getInstance().nb) //singleton permet d'acceder à cela n'importe ou dans le code.
throw "incrementation d'un itérateur en fin de séquence";
while (indice_tache<getInstance().nb && disp<getInstance().taches[indice_tache]->getDateDisponibilite() )
++indice_tache;
}
/********************************************** PROJET MANAGER *********************************************/
void ProjetManager::addItem(Projet* p){
if (nb==nbMax){
Projet** newtab=new Projet*[nbMax+10];
for(unsigned int i=0; i<nb; i++) newtab[i]=projets[i];
// ou memcpy(newtab,taches,nb*sizeof(Tache*));
nbMax+=10;
Projet** old=projets;
projets=newtab;
delete[] old;
}
projets[nb++]=p;
}
ProjetManager::~ProjetManager(){
for(unsigned int i=0; i<nb; i++) delete projets[i];
delete[] projets;
}
void ProjetManager::ajouterTacheProjet( TacheManager& m, const std::string& id_projet, const std::string& id_tache){
Tache& ta=m.getTache(id_tache);
Projet& p=getProjet(id_projet);
if (p.getDateEcheance()<ta.getDateEcheance()) std::cout<<"date d'écheance du projet pas correcte, changez la avant de reessayer!\n";
else p.ajouterTacheP(ta);
}
void ProjetManager::parcourirTacheProjet(const std::string&id){
Projet& p=getProjet(id);
std::cout<<"\nParcours des Taches de "<<p.getTitre()<<"\n";
for (Projet::contTache::const_iterator it=p.taches_projet.begin();
it!=p.Projet::taches_projet.end(); ++it) (*it)->afficher();
}
Projet& ProjetManager::getProjet(const std::string& id){
Projet* p=trouverProjet(id);
if (!p) throw CalendarException("erreur,ProjetManager, projet inexistant");
return *p;
}
Projet* ProjetManager::trouverProjet(const std::string& id) const{
for(unsigned int i=0; i<nb; i++)
if (id==projets[i]->getId()) return projets[i];
return 0;
}
Projet& ProjetManager::ajouterProjet(const std::string& id, const std::string& t, const TIME::Date& dispo, const TIME::Date& deadline) {
if (trouverProjet(id)) throw CalendarException("erreur, TacheProjet, projet deja existant");
Projet* newt=new Projet(id,t,dispo,deadline);
addItem(newt);
std::cout<<"projet ajouté: "<<t<<"\n";
return *newt;
}
/************************************************** PROJET**************************************************************/
Projet& Projet::ajouterTacheP(Tache& t ){
if (t.isIn()) throw CalendarException("tache deja dans un projet ou une tache composite!");
taches_projet.push_back(&t);
t.setIn();
return *this;
}
