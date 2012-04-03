#ifndef CICLISTA_H
#define CICLISTA_H
#include <pthread.h>
#include <sched.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct info {
     double velocidades[3];
     int tid;
} info;

unsigned long int cur_ticket;
unsigned long int next_ticket;

int fetch_and_add(unsigned long int *variable, unsigned long int value);
void *ciclista(void *arg);
#endif
