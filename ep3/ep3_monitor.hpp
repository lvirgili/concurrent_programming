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
#include <ctime>
using namespace std;

class Monitor {
private:
     vector<bool> moving; //Se os carros estao em movimento.
     int _np;
     int _C, _m;
     int carro_atual; //Que carro esta na frente.
     vector<vector<pair<int,bool> > >lugares;
     vector<int> turns; //Qtas vezes o passageiro andou.

     //A fila de prioridade dos passageiros.
     priority_queue<Passageiro, vector<Passageiro>, comp_passageiro> fila_pass;

     //Funcoes do monitor.

     //Antes de mais nada, note que eu so tenho /uma/ fila, logo nao
     //faz sentido (ou seria inutil) passar o nome da fila como
     //parametro para as funcoes. Entao, elas nao tem.
     void empty(bool& ret);
     void wait(const Passageiro&);
     void wait(const int, const Passageiro&);
     void signal();
     void signal_all();
     void minrank(int&);

     //Funcao que imprime as informacoes pedidas.
     void printInfo(string);

public:
     Monitor(int, int);
     ~Monitor();
     void pegaCarona(Passageiro&);
     void carrega(int);
     void descarrega(int);
     void add_passageiro();
     void monitor_entry();
     void monitor_exit();
};
