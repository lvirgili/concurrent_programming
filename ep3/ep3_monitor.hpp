#pragma once

#include <fcntl.h>
#include <semaphore.h>
#include <iostream>
#include <queue>
#include <vector>
#include <thread>
#include "ep3_passageiro.hpp"

#include <errno.h>
#include <cstdio>
#include <cstring>
using namespace std;

class Monitor {
private:
     sem_t mutex;
     priority_queue<Passageiro, vector<Passageiro>, comp_passageiro> fila_pass;
     vector<sem_t *> pass_sems; //Semaforos privados dos Passageiros.
     sem_t *cars_sems; //Semaforos privados dos carros.
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
     void printInfo(string);

public:
     Monitor(int, int);
     ~Monitor();
     void pegaCarona(const Passageiro&);
     void carrega(int);
     void descarrega(int);
     void add_passageiro();
     void monitor_entry();
     void monitor_exit();
     void monitor_print(int);
};
