#if !defined(_PRECEDENCE_H)
#define _PRECEDENCE_H
#include "taches.h"

class Precedence {
private:
    Tache* pred;
    Tache*succ;
public:
    Precedence(Tache& i, Tache& j): pred(&i),succ(&j){}
    Tache& getPredecesseur() const {return *pred;}
    Tache& getSuccesseur() const {return *succ;}
};

class PrecedenceManager {
private:
    Precedence** tab;
    int nb;
    int nbMax;
    static PrecedenceManager* instance;
    PrecedenceManager& operator=(const PrecedenceManager& m);
    PrecedenceManager(const PrecedenceManager& m);
    PrecedenceManager();
    ~PrecedenceManager();
public:
    static PrecedenceManager& getInstance();
    static void libererInstance();
};



#endif
