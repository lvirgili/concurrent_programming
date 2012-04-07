#ifndef CICLISTA_H
#define CICLISTA_H
#include <pthread.h>
#include <sched.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#ifndef ROAD
#define ROAD
#define T_DESCIDA 0
#define T_PLANO 1
#define T_SUBIDA 2
#endif

typedef struct info {
     double velocidades[3];
     int tid;
     double ret[3];
} info;

typedef struct trecho {
     int tipo;
     int inicio;
     int fim;
     int ja_passaram;
} trecho;

trecho *trechos;

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
int fim;
int chegaram;
int minuto;

int fetch_and_add(unsigned long int *variable, unsigned long int value);
void bar();
void *ciclista(void *arg);
int alocar_coisas();
void print_info();
int change_trecho(int cur_trecho, double km, double min_move);
#endif
