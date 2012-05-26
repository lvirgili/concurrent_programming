#ifndef MONITOR
#define MONITOR

#include <fcntl.h>
#include <semaphore.h>
#include <iostream>
#include <queue>
#include <vector>
#include "ep3_passageiro.hpp"
using namespace std;

class Monitor {
private:
     sem_t *entry;
     priority_queue<Passageiro, vector<Passageiro>, comp_passageiro> fila;
     vector<sem_t *> priv_sems;
     int _nc;

     bool empty() const;
     void wait(const Passageiro&);
     void wait(const int, const Passageiro&);
     void signal();
     void signal_all();
     int minrank() const;

public:
     Monitor();
     ~Monitor();
};


#endif
