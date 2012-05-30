#pragma once

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
     vector<sem_t *> priv_sems; //Semaforos privados dos Passageiros.
     vector<sem_t *> cars_sems; //Semaforos privados dos carros.
     int _np;

     void empty(bool&);
     void wait(const Passageiro&);
     void wait(const int, const Passageiro&);
     void signal();
     void signal_all();
     void minrank(int&);

public:
     Monitor();
     ~Monitor();
};
