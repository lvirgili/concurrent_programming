#include "ep3_monitor.hpp"
#include "ep3_passageiro.hpp"
#include <iostream>
#include <cstdlib>
#include <ctime>
using namespace std;

int main(int argc, char **argv) {
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
     cout << "C = " << C << endl;
     cout << "m = " << m << endl;
     cout << "t = " << t << endl;
     srand(time(NULL));
     priority_queue<Passageiro, vector<Passageiro>, comp_passageiro> fila;
     for (int i = 0; i < 5; ++i) {
          Passageiro x(i);
          sleep(1);
          cout << x.tid() << ' ' << x.golden() << ' ' << x.arrive() << endl;
          fila.push(x);
     }
     cout << "======\n";
     while (fila.empty() == false) {
          Passageiro x(fila.top());
          fila.pop();
          cout << x.tid() << ' ' << x.golden() << ' ' << x.arrive() << endl;
     }
     return 0;
}
