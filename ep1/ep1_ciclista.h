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

pthread_mutex_t barreira;
pthread_cond_t go;
int num_ciclistas;
int num_arrived;

int fetch_and_add(unsigned long int *variable, unsigned long int value);
void bar();
void *ciclista(void *arg);
#endif
