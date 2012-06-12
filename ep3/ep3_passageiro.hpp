#pragma once

#include <cstdlib>
#include <ctime>
#include <iostream>

//Classe que representa o passageiro.
//So tem o id, se tem um bilhete dourado e quando ele chegou.
class Passageiro {
private:
     int _tid;
     bool _golden;
     time_t _arrive;

public:
     Passageiro(int);
     Passageiro(int, bool);
     Passageiro(int, bool, time_t);
     Passageiro(const Passageiro&);
     int tid() const;
     bool golden() const;
     time_t arrive() const;
     void arrive(time_t);
};

//Como ordenar a fila.
struct comp_passageiro {
     bool operator()(const Passageiro& a, const Passageiro& b) const {
          if (a.golden() == true && b.golden() == false) {
               return false;
          }
          if (a.golden() == false && b.golden() == true) {
               return true;
          }
          return a.arrive() > b.arrive();
     }
};
