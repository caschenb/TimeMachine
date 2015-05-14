#include <iostream>
#include"taches.h"
using namespace std;

int main()
{
    cout << "Hello world!" << endl;
    TacheManager& manager=TacheManager::getInstance();
	manager.ajouterTache("bjknam","Partage des scenarii",TIME::Duree(1,30), TIME::Date(1,1,2015),TIME::Date(1,9,2015));
	manager.ajouterTacheComposite("AW2","Identification des competences",TIME::Date(1,1,2015),TIME::Date(1,12,2015));
	manager.ajouterTache("bjam","Partage des scenarii",TIME::Date(1,1,2015),TIME::Date(1,9,2015));
	manager.ajouterTache("coucou", "Projet LO21",TIME::Duree(100,00), TIME::Date(16,05,2015), TIME::Date(14,06,2015));
	manager.ajouterSousTache("AW2", "coucou");
	manager.ajouterSousTache("AW2","bjam");


  for(TacheManager::Iterator it= manager.getIterator();!it.isDone();it.next()){
//std::cout<<it.current()<<"\n";
it.current().afficher();
  }
	std::cout<<"\n\n\n";
	manager.parcourirTacheComposite("AW2");

}
