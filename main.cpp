#include <iostream>
#include"taches.h"
using namespace std;

int main()
{
    cout << "Hello world!" << endl;
    TacheManager& manager=TacheManager::getInstance();
	manager.ajouterTache("bjknam","Partage des scenarii",TIME::Date(1,1,2015),TIME::Date(1,9,2015));
	manager.ajouterTache("AW2","Identification des competences",TIME::Date(1,1,2015),TIME::Date(1,12,2015));
	manager.ajouterTache("bjam","Partage des scenarii",TIME::Date(1,1,2015),TIME::Date(1,9,2015));
  for(TacheManager::Iterator it= manager.getIterator();!it.isDone();it.next()){
std::cout<<it.current()<<"\n";
}

}
