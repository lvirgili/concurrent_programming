#include "ep3_monitor.hpp"

//Construtor do monitor.
Monitor::Monitor(int C, int m) {
     _C = C;
     _m = m;
     carro_atual = 0;
     for (int i = 0; i < m; ++i) {
          //Coloca os lugares nos carros.
          vector<pair<int, bool> > aux;
          lugares.push_back(aux);
          //Inicializa os semaforos dos carros.
          sem_t *s;
          s = sem_open("s", O_CREAT, S_IWUSR, 1);
          cars_sems.push_back(s);
          //Carros parados
          moving.push_back(false);
     }
     //Tenta criar um semaforo chamado "entry" com valor inicial 1.
     entry = sem_open("entry", O_CREAT, S_IWUSR, 1);
     if (entry == SEM_FAILED) {
          cout << "[MONITOR] Erro ao criar o semaforo 'entry'.\n";
          exit(-1);
     }
     _np = 0;
}

//Destrutor de um monitor.
//Simplesmente fecha o semaforo.
Monitor::~Monitor() {
     for (unsigned i = 0; i < pass_sems.size(); ++i) {
          sem_close(pass_sems[i]);
     }
     for (unsigned i = 0; i < cars_sems.size(); ++i) {
          sem_close(cars_sems[i]);
     }
     sem_close(entry);
}

void Monitor::print(string evento) {
     if (evento == "inicio") {
          cout << "Inicio de um passeio.\n";
     } else if (evento == "fim") {
          cout << "Fim de um passeio.\n";
     } else {
          cout << "Passageiro chegou a fila.\n";
     }
     cout << fila_pass.size() << " passageiros estao esperando para entrar nos carros.\n";
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
          cout << p.tid() << turns[p.tid()] << endl;
     }
     cout << "Quantidade de passageiros nos carros:\n";
     for (int i = 0; i < _C; ++i) {
          cout << "Carro " << i << ' ' << lugares[i].size() << endl;
     }
     cout << "Passageiros nos carros:";
     for (int i = 0; i < _C; ++i) {
          cout << "No carro " << i << ":\n";
          for (unsigned j = 0; j < lugares[i].size(); ++j) {
               if (lugares[i][j].second == true) {
                    cout << "D";
               }
               cout << lugares[i][j].first << endl;
          }
     }
     cout << "Situacao dos carros:\n";
     for (int i = 0; i < _C; ++i) {
          cout << "Carro " << i << ": ";
          if (moving[i] == true) {
               cout << "em movimento.\n";
          } else {
               cout << "parado.\n";
          }
     }
}

void Monitor::monitor_entry() {
     sem_wait(entry);
}

void Monitor::monitor_exit() {
     sem_post(entry);
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
     sem_post(entry);
     sem_wait(pass_sems[x.tid()]);
     sem_wait(entry);
}

void Monitor::signal() {
     if (_np > 0) {
          --_np;
          int other_id = fila_pass.top().tid();
          sem_post(pass_sems[other_id]);
     }
}

void Monitor::signal_all() {
     if (_np > 0) {
          while (fila_pass.empty() == false) {
               --_np;
               int other_id = fila_pass.top().tid();
               sem_post(pass_sems[other_id]);
               fila_pass.pop();
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
     if (e == false) {
          //Ja tem gente, entao espera.
          wait(p);
     }
     lugares[carro_atual].push_back(make_pair(p.tid(), p.golden())); //Se coloca no carro.
     if (lugares[carro_atual].size() == (unsigned)_C) {
          //Carro cheio
          sem_post(cars_sems[carro_atual]);
     } else {
          //Ainda tem lugar no carro, entao espera. Ou seja, se bloqueia no seu semaforo.
          sem_wait(pass_sems[p.tid()]);
     }
     monitor_exit();
}

void Monitor::carrega(int carro) {
     monitor_entry();
     monitor_exit();
     sem_wait(cars_sems[carro]); //Espera ficar cheio.
     monitor_entry(); //Espera poder entrar no monitor.
     moving[carro] = true;
     print("inicio");
     sleep(1); //Passeia.
     monitor_exit();
}

void Monitor::descarrega(int carro) {
     monitor_entry();
     moving[carro] = false;
     print("fim");
     for (unsigned i = 0; i < lugares[carro].size(); ++i) {
          //Avisa os passageiros que a volta acabou.
          ++turns[lugares[carro][i].first];
          sem_post(pass_sems[lugares[carro][i].first]);
     }
     monitor_exit();
}

void Monitor::add_passageiro() {
     monitor_entry();
     turns.push_back(0);
     monitor_exit();
}
