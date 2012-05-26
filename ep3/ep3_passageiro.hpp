#ifndef PASSAGEIRO
#define PASSAGEIRO

#include <cstdlib>
#include <ctime>
#include <iostream>

class Passageiro {
private:
     int _tid;
     bool _golden;
     time_t _arrive;

public:
     Passageiro(int);
     Passageiro(int, bool, time_t);
     Passageiro(const Passageiro&);
     int tid() const;
     bool golden() const;
     time_t arrive() const;
     void arrive(time_t);
};

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

#endif
