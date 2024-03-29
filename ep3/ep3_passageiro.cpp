#include "ep3_passageiro.hpp"

//Aqui eu so implemento os construtores. Um passageiro tem 20% de
//probabilidade de ganhar um bilhete dourado.

Passageiro::Passageiro(int tid) {
     _tid = tid;
     _golden = (rand()%50 < 10) ? true : false;
     time(&_arrive);
}

Passageiro::Passageiro(int tid, bool golden) {
     _tid = tid;
     _golden = golden;
     time(&_arrive);
}

Passageiro::Passageiro(int tid, bool golden, time_t t) {
     _tid = tid;
     _golden = golden;
     _arrive = t;
}

Passageiro::Passageiro(const Passageiro& other) {
     _tid = other.tid();
     _golden = other.golden();
     _arrive = other.arrive();
}

int Passageiro::tid() const {
     return _tid;
}

bool Passageiro::golden() const {
     return _golden;
}

time_t Passageiro::arrive() const {
     return _arrive;
}

void Passageiro::arrive(time_t t) {
     _arrive = t;
}
