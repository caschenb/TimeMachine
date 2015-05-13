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
