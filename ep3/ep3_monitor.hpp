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
     priority_queue<Passageiro, vector<Passageiro>, comp_passageiro> fila_pass;
     vector<sem_t *> pass_sems; //Semaforos privados dos Passageiros.
     vector<sem_t *> cars_sems; //Semaforos privados dos carros.
     vector<bool> moving; //Se os carros estao em movimento.
     int _np;
     int _C, _m;
     int carro_atual; //Que carro esta na frente.
     vector<vector<pair<int,bool> > >lugares;
     vector<int> turns; //Qtas vezes o passageiro andou.

     void empty(bool& ret);
     void wait(const Passageiro&);
     void wait(const int, const Passageiro&);
     void signal();
     void signal_all();
     void minrank(int&);
     void monitor_entry();
     void monitor_exit();
     void print(string);

public:
     Monitor(int, int);
     ~Monitor();
     void pegaCarona(const Passageiro&);
     void carrega(int);
     void descarrega(int);
     void add_passageiro();
};
