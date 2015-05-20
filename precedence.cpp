#include "precedence.h"

PrecedenceManager* PrecedenceManager::instance=0;

PrecedenceManager& PrecedenceManager::getInstance(){
    if (instance==0) instance= new PrecedenceManager;
    return *instance;
}

void PrecedenceManager::libererInstance(){
    delete instance;
    instance = 0;
}
PrecedenceManager::PrecedenceManager(){}
PrecedenceManager::~PrecedenceManager(){}

int PrecedenceManager::tacheProgrammable(const Tache& t)const{
    for(int i=0;i<nb;i++){
        if(tab[i]->getSuccesseur()==&t){
            if(not(predProgrammee((tab[i]->getPredecesseur()),(tab[i]->getSuccesseur())))) return 0;
        }
    }
    return 1;
}
