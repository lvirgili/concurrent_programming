#include "ep3_monitor.hpp"

//Construtor do monitor.
Monitor::Monitor() {
     //Tenta criar um semaforo chamado "entry" com valor inicial 1.
     entry = sem_open("entry", O_CREAT, S_IWUSR, 1);
     if (entry == SEM_FAILED) {
          cout << "[MONITOR] Erro ao criar o semaforo 'entry'.\n";
          exit(-1);
     }
     _nc = 0;
}

//Destrutor de um monitor.
//Simplesmente fecha o semaforo.
Monitor::~Monitor() {
     for (unsigned i = 0; i < priv_sems.size(); ++i) {
          sem_close(priv_sems[i]);
     }
     sem_close(entry);
}

//Operacao 'empty'.
//Simplesmente retorna se a fila esta vazia.
bool Monitor::empty() const {
     sem_wait(entry);
     sem_post(entry);
     return fila.empty();
}

//Operacao 'wait'.
void Monitor::wait(const Passageiro& x) {
     wait(x.golden(), x);
}

//Operacao 'wait' com rank.
void Monitor::wait(const int rank, const Passageiro& x) {
     sem_wait(entry);
     ++_nc;
     fila.push(x.tid());
     sem_post(entry);
     sem_wait(priv_sems[x.tid()]);
     sem_wait(entry);
}

void Monitor::signal() {
     sem_wait(entry);
     if (_nc > 0) {
          --_nc;
          int other_id = fila.top().tid();
          sem_post(priv_sems[other_id]);
     }
     sem_post(entry);
}

void Monitor::signal_all() {
     sem_wait(entry);
     if (_nc > 0) {
          while (fila.empty() == false) {
               --_nc;
               int other_id = fila.top().tid();
               sem_post(priv_sems[other_id]);
               fila.pop();
          }
     }
     sem_post(entry);
}

//Operacao 'minrank'.
//Retorna a prioridade do elemento no comeco da fila.
int Monitor::minrank() const {
     sem_wait(entry);
     int rank = 1234;
     if (fila.empty() == true) {
          rank = -1;
     } else {
          rank = fila.top().golden();
     }
     sem_post(entry);
     return rank;
}
