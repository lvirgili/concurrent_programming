#include "ep3_monitor.hpp"
#include "ep3_passageiro.hpp"
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <thread>
using namespace std;

Monitor *mon;
int np = 6;
double sleep_time = 2; //Quanto tempo demora um paseio.

//Os carros carregam...
void carro(int cid) {
     mon->carrega(cid);
}

//... os passageiros sao carregados.
void passageiro(Passageiro& p) {
     mon->pegaCarona(p);
}

int main(int argc, char **argv) {
     vector<thread> cars;
     vector<thread> pass;
     int C, m; //C = capacidade do carro, m = no de carros
     double t; //t = taxa
     if (argc != 4) {
          cout << "Uso: ./ep3 C m t\n";
          return 0;
     } else {
          C = atoi(argv[1]);
          m = atoi(argv[2]);
          t = strtod(argv[3], NULL);
     }
     t *= 100;
     srand(time(NULL));
     mon = new Monitor(C, m);
     for (int i = 0; i < m; ++i) {
          cars.push_back(thread(carro, i));
     }
     for (int i = 0; i < np; ++i) {
          Passageiro p(i);
          mon->add_passageiro();
          pass.push_back(thread(passageiro, p));
          sleep(t * sleep_time);
     }
     for (int i = 0; i < np; ++i) {
          pass[i].join();
     }
     for (int i = 0; i < m; ++i) {
          cars[i].join();
     }

     delete mon;

     return 0;
}
