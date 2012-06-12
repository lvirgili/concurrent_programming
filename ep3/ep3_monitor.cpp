#include "ep3_monitor.hpp"

int npass = 6;    //No de passageiros. Nao esquecer de alterar na main
                                //tambem. Tambem recomendo um multiplo do numero de
                                //lugares que vc rodar, dado que vc quer queo programa acabe.
sem_t e;                 //Semaforo que controla a entrada ao monitor.
sem_t *cars_sems;               //Semaforos privados dos carros.
sem_t *pass_sems;               //Semaforos privados dos Passageiros.
int finished = 0;               //Marca quantos passageiros ja foram embora.

//Construtor do monitor.
//Nao faz nada de emocionante.
Monitor::Monitor(int C, int m) {
     int aux;
     aux = sem_init(&e, 0, 1);
     if (aux != 0) {
          cout << "DAMN\n";
          exit(-1);
     }
     if (aux != 0) {
          cout << "DAMN\n";
          exit(-1);
     }
     _C = C;
     _m = m;
     carro_atual = 0;
     cars_sems = new sem_t[m];
     pass_sems = new sem_t[npass];
     for (int i = 0; i < m; ++i) {
          //Coloca os lugares nos carros.
          for (int j = 0; j < C; ++j) {
               vector<pair<int, bool> > aux;
               lugares.push_back(aux);
          }
          //Inicializa os semaforos privados dos carros.
          aux = sem_init(&cars_sems[i], 0, 0);
          if (aux != 0) {
               cout << "DAMN\n";
               exit(-1);
          }
          //Carros parados
          moving.push_back(false);
     }
     for (int i = 0; i < npass; ++i) {
          aux = sem_init(&pass_sems[i], 0, 0);
          if (aux != 0) {
               cout << "DAMN\n";
               exit(-1);
          }
     }
     _np = 0;
}

//Destrutor do monitor.
Monitor::~Monitor() {
     for (int i = 0; i < npass; ++i) {
          sem_destroy(&pass_sems[i]);
     }
     for (int i = 0; i < _m; ++i) {
          sem_destroy(&cars_sems[i]);
     }
     delete[] cars_sems;
     delete[] pass_sems;
     sem_destroy(&e);
}

//Funcao que imprime o que foi pedido no enunciado.  Note que pelo
//enunciado a ordem de impressao nao deveria levar em conta o bilhete
//dourado, mas um email do professor no forum mudou isso.
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
     cout << "Lista dos passageiros esperando:\n";
     while (aux.empty() == false) {
          Passageiro p(aux.top());
          if (p.golden() == true) {
               cout << "D";
          }
          cout << p.tid() << ":" << turns[p.tid()] << endl;
          aux.pop();
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

//Protocolo de entrada ao monitor.
//Simplesmente espera no semaforo.
void Monitor::monitor_entry() {
     sem_wait(&e);
}

//Protocolo de saida do monitor.
//Libera o semaforo.
void Monitor::monitor_exit() {
     sem_post(&e);
}

//As operacoes basicas do monitor. Sao baseadas no codigo do
//livro. Logo, nada de emocionantes.

void Monitor::empty(bool& ret) {
     ret = fila_pass.empty();
}

//Simplesmente chama a wait com rank, ja que nao faz sentido no meu
//programa.
void Monitor::wait(const Passageiro& x) {
     wait(x.golden(), x);
}

void Monitor::wait(const int rank, const Passageiro& x) {
     ++_np;
     fila_pass.push(x);
     printInfo("fila");
     monitor_exit();
     sem_wait(&pass_sems[x.tid()]);
     monitor_entry();
}

void Monitor::signal() {
     if (_np > 0) {
          --_np;
          int other_id = fila_pass.top().tid();
          fila_pass.pop();
          sem_post(&pass_sems[other_id]);
     }
}

void Monitor::signal_all() {
     if (_np > 0) {
          while (fila_pass.empty() == false) {
               --_np;
               int other_id = fila_pass.top().tid();
               fila_pass.pop();
               sem_post(&pass_sems[other_id]);
          }
     }
}

void Monitor::minrank(int& rank) {
     if (fila_pass.empty() == true) {
          rank = -1;
     } else {
          rank = fila_pass.top().golden();
     }
}

//Funcao pegaCarona. Ela e chamada pelos passageiros para tentar
//entrar no carro. Se nao conseguirem entrar no carro eles esperam na
//fila. Se o carro ficar cheio o ultimo passageiro a entrar acorda o
//carro.
void Monitor::pegaCarona(Passageiro& p) {
     for (int turn = 0; turn < 2; ++turn) {
          monitor_entry();
          wait(p);
          lugares[carro_atual].push_back(make_pair(p.tid(), p.golden())); //Se coloca no carro.
          if (lugares[carro_atual].size() == (unsigned)_C) {
               //Carro cheio
               sem_post(&cars_sems[carro_atual]);
               carro_atual = (carro_atual + 1)  % _m;
          }
          ++finished;
          monitor_exit();
          sem_wait(&pass_sems[p.tid()]);
     }
}

//Funcao carrega. E chamada pelos carros para para levar os
//passageiros no passeio. Contudo o carro tem que esperar ate ficar
//cheio. Eu usei um timedwait para o carro acordar periodicamente e
//checar se tem passageiros na fila dormindo. No fim chama descarrega.
void Monitor::carrega(int carro) {
     bool go = false;
     struct timespec ts;
     ts.tv_sec = 3;
     while (go == false) {
          sem_timedwait(&cars_sems[carro], &ts);
          if (lugares[carro].size() == (unsigned)_C) {
               go = true;
          } else {
               if (fila_pass.empty() == false) signal();
          }
          if (finished == 2*npass) return;
     }
     monitor_entry();
     moving[carro] = true;
     printInfo("inicio");
     sleep(2); //Passeia.
     monitor_exit();
     descarrega(carro);
}

//A funcao descarrega so avisa os passageiros que deram a volta que a
//volta acabou, alem de incrementar o contador de quantas voltas cada
//passageiro deu.
void Monitor::descarrega(int carro) {
     monitor_entry();
     moving[carro] = false;
     printInfo("fim");
     for (unsigned i = 0; i < lugares[carro].size(); ++i) {
          //Avisa os passageiros que a volta acabou.
          ++turns[lugares[carro][i].first];
          sem_post(&pass_sems[lugares[carro][i].first]);
     }
     lugares[carro].clear();
     monitor_exit();
     if (finished < 2*npass) carrega(carro);
}

//Funcao auxiliar.
void Monitor::add_passageiro() {
     monitor_entry();
     turns.push_back(0);
     monitor_exit();
}
