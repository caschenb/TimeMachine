#include "taches.h"
#include <fstream>

std::ostream& operator<<(std::ostream& fout, const Tache& t){
	fout<<t.getId()<<"\n";
	fout<<t.getTitre()<<"\n";
	fout<<t.getDateDisponibilite()<<"\n";
	fout<<t.getDateEcheance()<<"\n";
	return fout;
}

//std::ostream& operator<<(std::ostream& f, const Programmation& p);

TacheManager::TacheManager():taches(0),nb(0),nbMax(0){}

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

Tache* TacheManager::trouverTache(const std::string& id)const{
	for(unsigned int i=0; i<nb; i++)
		if (id==taches[i]->getId()) return taches[i];
	return 0;
}

Tache& TacheManager::ajouterTache(const std::string& id, const std::string& t, const TIME::Date& dispo, const TIME::Date& deadline){
	if (trouverTache(id)) throw CalendarException("erreur, TacheManager, tache deja existante");
	Tache* newt=new Tache(id,t,dispo,deadline);
	addItem(newt);
	std::cout<<"tache ajoutée: "<<t<<"\n";
	return *newt;
}

Tache& TacheManager::getTache(const std::string& id){
	Tache* t=trouverTache(id);
	if (!t) throw CalendarException("erreur, TacheManager, tache inexistante");
	return *t;
}

const Tache& TacheManager::getTache(const std::string& id)const{
	return const_cast<TacheManager*>(this)->getTache(id);
}

TacheManager::~TacheManager(){
	if (file!="") save(file);
	for(unsigned int i=0; i<nb; i++) delete taches[i];
	delete[] taches;
	file="";
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

void  TacheManager::save(const std::string& f){
	file=f;
	std::ofstream fout(f.c_str(),std::ofstream::trunc); // toutes les taches existantes sont écrasées
	for(unsigned int i=0; i<nb; i++){
		fout<<*taches[i];
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

void  TacheManager::DisponibiliteFilterIterator::next () {
            if(indice_tache>=getInstance().nb)  //singleton permet d'acceder à cela n'importe ou dans le code.
           throw "incrementation d'un itérateur en fin de séquence";
            while (indice_tache<getInstance().nb && disp<getInstance().taches[indice_tache]->getDateDisponibilite() )
	       ++indice_tache;
           }

