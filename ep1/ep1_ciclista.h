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

int **estrada;
int *next_position;

unsigned long int cur_ticket;
unsigned long int next_ticket;

pthread_mutex_t barreira;
pthread_cond_t go;

int num_ciclistas;
int num_arrived;
int tamanho_estrada;
int largura_estrada;
int minuto;

int fetch_and_add(unsigned long int *variable, unsigned long int value);
void bar();
void *ciclista(void *arg);
int alocar_coisas();
void print_info();
#endif
