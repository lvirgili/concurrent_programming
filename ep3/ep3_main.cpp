#include "ep3_monitor.hpp"
#include "ep3_passageiro.hpp"
#include <iostream>
#include <cstdlib>
#include <ctime>
using namespace std;

int main() {
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
