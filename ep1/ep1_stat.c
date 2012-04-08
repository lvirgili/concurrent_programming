#include "ep1_stat.h"

/* Utiliza o metodo de Box-Muller para gerar um numero normalmente
 * distribuido de media 0 e desvio padrao 1. */
static double gen_norm() {
     double u1 = (double)rand() / RAND_MAX;
     double u2 = (double)rand() / RAND_MAX;
     return (sqrt(-2*log(u1)) * cos(2*M_PI*u2));
}

/* Gera uma velocidade seguindo uma normal com media media e desvio
 * padrao desv_pad. */
double rand_velocity(double media, double desv_pad) {
     double z = gen_norm();
     return (desv_pad*z + media);
}
