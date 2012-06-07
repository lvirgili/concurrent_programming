#include "ep3_monitor.hpp"
#include "ep3_passageiro.hpp"
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <thread>
using namespace std;

Monitor *mon;
int np = 3;

void carro(int cid) {
     int i = 0;
     while (true) {
          //if (i == 2) break;
          mon->carrega(cid);
          mon->descarrega(cid);
          ++i;
          //if (i == 2) break;
     }
}

void passageiro(Passageiro& p) {
     for (int volta = 0; volta < 2; ++volta) {
          mon->pegaCarona(p);
     }
     mon->monitor_entry();
     cout << "Passageiro " << p.tid() << " foi embora.\n";
     mon->monitor_exit();
}

void test(int x) {
     mon->monitor_print(x);
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
     srand(time(NULL));
     t = t+1;
     mon = new Monitor(C, m);
     for (int i = 0; i < m; ++i) {
          cars.push_back(thread(carro, i));
     }
     for (int i = 0; i < np; ++i) {
          if (i == 0) {
               Passageiro p(i, true);
               mon->add_passageiro();
               pass.push_back(thread(passageiro, p));

          } else {
               Passageiro p(i);
               mon->add_passageiro();
               pass.push_back(thread(passageiro, p));
          }
     }
     for (int i = 0; i < np; ++i) {
          pass[i].join();
     }
     for (int i = 0; i < m; ++i) {
          cars[i].join();
     }

     // mon = new Monitor(1,1);
     // vector<thread> t;
     // for (int i = 0; i < 5; ++i) {
     //      t.push_back(thread(test, i));
     // }
     // for (int i = 0; i < 5; ++i) {
     //      t[i].join();
     // }

     delete mon;
     return 0;
}
