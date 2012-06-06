#include "ep3_monitor.hpp"
#include "ep3_passageiro.hpp"
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <thread>
using namespace std;

Monitor *mon;

void carro(int cid) {
     int i = 0;
     while (true) {
          mon->carrega(cid);
          mon->descarrega(cid);
          ++i;
          cout << "******* " << i << " *********\n";
          if (i > 1) break;
     }
     cout << "ACABOU\n";
}

void passageiro(Passageiro& p) {
     for (int volta = 0; volta < 2; ++volta) {
          mon->pegaCarona(p);
     }
     cout << "Passageiro fim\n";
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
     cout << "C = " << C << endl;
     cout << "m = " << m << endl;
     cout << "t = " << t << endl;
     mon = new Monitor(C, m);
     for (int i = 0; i < m; ++i) {
          cars.push_back(thread(carro, i));
     }
     for (int i = 0; i < 1; ++i) {
          Passageiro p(i);
          mon->add_passageiro();
          pass.push_back(thread(passageiro, p));
     }
     for (int i = 0; i < 1; ++i) {
          pass[i].join();
     }
     for (int i = 0; i < m; ++i) {
          cars[i].join();
     }
     return 0;
}
