#include <iostream>
#include"taches.h"
using namespace std;

int main()
{
    try {
    cout << "Hello world!" << endl;
    TacheManager& manager=TacheManager::getInstance();
	manager.ajouterTache("bjknam","Partage des scenarii",TIME::Duree(1,30), TIME::Date(1,1,2015),TIME::Date(1,9,2016), true);
	manager.ajouterTacheComposite("AW2","Identification des competences",TIME::Date(1,1,2015),TIME::Date(1,12,2015));
	manager.ajouterTache("bjam","Partage des scenarii",TIME::Duree(0,10), TIME::Date(1,1,2015),TIME::Date(1,9,2015),0 );
	manager.ajouterTache("coucou", "Projet LO21",TIME::Duree(10,00), TIME::Date(16,05,2015), TIME::Date(14,06,2015),0);
	manager.ajouterSousTache("AW2", "coucou");
	manager.ajouterSousTache("AW2","bjam");
	//manager.ajouterSousTache("AW2","bjam");


  for(TacheManager::Iterator it= manager.getIterator();!it.isDone();it.next()){
//std::cout<<it.current()<<"\n";
it.current().afficher();
  }
	std::cout<<"\n\n\n";
	manager.parcourirTacheComposite("AW2");

ProjetManager p;
p.ajouterProjet("1", "ProjetLO21", TIME::Date(01,01,2015), TIME::Date(14,06,2016));
p.ajouterProjet("2", "ProjetNF17", TIME::Date(01,01,2015), TIME::Date(17,06,2015));

p.ajouterTacheProjet( manager,"1", "bjknam");
p.ajouterTacheProjet(manager, "1","AW2");
p.parcourirTacheProjet("1");

manager.getTache("bjknam").setDatesDisponibiliteEcheance(TIME::Date(1,1,2015),TIME::Date(1,9,2015) );
p.ajouterTacheProjet(manager, "1","bjknam");
p.parcourirTacheProjet("1");

/*
p.ajouterTacheProjet( manager,"2", "coucou");
p.ajouterTacheProjet(manager, "2","AW2");
p.parcourirTacheProjet("2");*/

    }
    catch (CalendarException e)
{
   std::cout<<e.getInfo();
}
}
