#include "ep3_monitor.hpp"

//Construtor do monitor.
Monitor::Monitor(int C, int m) {
     sem_init(&mutex, 1, 1);
     _C = C;
     _m = m;
     carro_atual = 0;
     cars_sems = new sem_t[m];
     for (int i = 0; i < m; ++i) {
          //Coloca os lugares nos carros.
          for (int j = 0; j < C; ++j) {
               vector<pair<int, bool> > aux;
               lugares.push_back(aux);
          }
          //Inicializa os semaforos privados dos carros.
          sem_init(&cars_sems[i], 1, 0);
          //Carros parados
          moving.push_back(false);
     }
     _np = 0;
}

//Destrutor de um monitor.
Monitor::~Monitor() {
     for (unsigned i = 0; i < pass_sems.size(); ++i) {
          //sem_close(pass_sems[i]);
          sem_destroy(pass_sems[i]);
     }
     for (int i = 0; i < _m; ++i) {
          //sem_close(cars_sems[i]);
          sem_destroy(&cars_sems[i]);
     }
     delete[] cars_sems;
     //sem_close(mutex);
     sem_destroy(&mutex);
}

void Monitor::printInfo(string evento) {
     cout << "===== EVENTO ESPECIAL =====\n";
     if (evento == "inicio") {
          cout << "Inicio de um passeio.\n";
     } else if (evento == "fim") {
          cout << "Fim de um passeio.\n";
     } else {
          cout << "Passageiro chegou a fila.\n";
     }
     cout << "*** 1 ***\n";
     cout << fila_pass.size() << " passageiros estao esperando para entrar nos carros.\n\n";
     cout << "*** 2 ***\n";
     priority_queue<Passageiro, vector<Passageiro>, comp_passageiro> aux(fila_pass);
     priority_queue<Passageiro, vector<Passageiro>, comp_to_print> fila;
     while (aux.empty() == false) {
          fila.push(aux.top());
          aux.pop();
     }
     cout << "Lista dos passageiros esperando:\n";
     while (fila.empty() == false) {
          Passageiro p(fila.top());
          if (p.golden() == true) {
               cout << "D";
          }
          cout << p.tid() << ' ' << turns[p.tid()] << endl;
          fila.pop();
     }
     cout << endl;
     cout << "*** 3 ***\n";
     cout << "Quantidade de passageiros nos carros:\n";
     for (int i = 0; i < _m; ++i) {
          cout << "Carro " << i << ": " << lugares[i].size() << endl;
     }
     cout << endl;
     cout << "*** 4 ***\n";
     cout << "Passageiros nos carros:\n";
     for (int i = 0; i < _m; ++i) {
          cout << "No carro " << i << ": ";
          for (unsigned j = 0; j < lugares[i].size(); ++j) {
               if (lugares[i][j].second == true) {
                    cout << "D";
               }
               cout << lugares[i][j].first << ' ';
          }
          cout << endl;
     }
     cout << endl;
     cout << "*** 5 ***\n";
     cout << "Situacao dos carros:\n";
     for (int i = 0; i < _m; ++i) {
          cout << "Carro " << i << ": ";
          if (moving[i] == true) {
               cout << "em movimento.\n";
          } else {
               cout << "parado.\n";
          }
     }
     cout << "==========\n\n";
}

void Monitor::monitor_entry() {
     sem_wait(&mutex);
}

void Monitor::monitor_exit() {
     sem_post(&mutex);
}

//Operacao 'empty'.
//Simplesmente retorna se a fila esta vazia.
void Monitor::empty(bool& ret) {
     ret = fila_pass.empty();
}

//Operacao 'wait'.
void Monitor::wait(const Passageiro& x) {
     wait(x.golden(), x);
}

//Operacao 'wait' com rank para os passageiros.
void Monitor::wait(const int rank, const Passageiro& x) {
     ++_np;
     fila_pass.push(x.tid());
     printInfo("fila");
     sem_post(&mutex);
     sem_wait(pass_sems[x.tid()]);
     sem_wait(&mutex);
}

void Monitor::signal() {
     if (_np > 0) {
          --_np;
          int other_id = fila_pass.top().tid();
          fila_pass.pop();
          sem_post(pass_sems[other_id]);
     }
}

void Monitor::signal_all() {
     if (_np > 0) {
          while (fila_pass.empty() == false) {
               --_np;
               int other_id = fila_pass.top().tid();
               fila_pass.pop();
               sem_post(pass_sems[other_id]);
          }
     }
}

//Operacao 'minrank'.
//Retorna a prioridade do elemento no comeco da fila.
void Monitor::minrank(int& rank) {
     if (fila_pass.empty() == true) {
          rank = -1;
     } else {
          rank = fila_pass.top().golden();
     }
}

void Monitor::pegaCarona(const Passageiro& p) {
     monitor_entry();
     bool e;
     empty(e);
     if (e == false || moving[carro_atual] == true || lugares[carro_atual].size() == (unsigned)_C) {
          //Ja tem gente, entao espera ou o carro esta andando (so deveria ter um, se cair nesse caso)
          wait(p);
     }
     lugares[carro_atual].push_back(make_pair(p.tid(), p.golden())); //Se coloca no carro.
     if (lugares[carro_atual].size() == (unsigned)_C) {
          //Carro cheio
          sem_post(&cars_sems[carro_atual]);
          carro_atual = (carro_atual + 1)  % _m;
     }
     monitor_exit();
     sem_wait(pass_sems[p.tid()]);
     monitor_entry();
     if (turns[p.tid()] < 2) {
          wait(p); //Se colocar na fila de novo.
     }
}

void Monitor::carrega(int carro) {
     monitor_entry();
     while (lugares[carro].size() < (unsigned)_C) {
          signal();
          monitor_exit();
          this_thread::yield();
     }
     //sem_wait(&cars_sems[carro]);
     monitor_entry(); //Espera poder entrar no monitor.
     moving[carro] = 1;
     printInfo("inicio");
     sleep(1); //Passeia.
     monitor_exit();
}

void Monitor::descarrega(int carro) {
     monitor_entry();
     moving[carro] = false;
     printInfo("fim");
     for (unsigned i = 0; i < lugares[carro].size(); ++i) {
          //Avisa os passageiros que a volta acabou.
          ++turns[lugares[carro][i].first];
          sem_post(pass_sems[lugares[carro][i].first]);
     }
     lugares[carro].clear();
     monitor_exit();
}

void Monitor::add_passageiro() {
     monitor_entry();
     turns.push_back(0);
     sem_t *aux;
     aux = new sem_t;
     sem_init(aux, 1, 0);
     pass_sems.push_back(aux);
     monitor_exit();
}

void Monitor::monitor_print(int x) {
     monitor_entry();
     cout << "Thread " << x << " inside the monitor.\n";
     monitor_exit();
}
