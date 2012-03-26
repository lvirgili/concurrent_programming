#ifndef STAT_H
#define STAT_H

#define M_PI 3.14159265358979323846

#include <math.h>
#include <stdlib.h>

/* Retorna velocidade normalmente distribuida ao redor de media com
 * desvio padrao desv_pad. */
double rand_velocity(double media, double desv_pad);

#endif
