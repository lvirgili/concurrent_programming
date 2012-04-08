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

/* struct que armazena as informacoes das threads. */
typedef struct info {
     double velocidades[3];
     int tid;
     int ret[4];
} info;

/* struct que armazena as informacoes dos trechos. */
typedef struct trecho {
     int tipo;
     int inicio;
     int fim;
     int ja_passaram;
     int quem_passou[3];
} trecho;

/* struct que e usada para fazer as comparacoes no final. */
typedef struct ret {
     int tid;
     int tempo;
     int plano;
     int subida;
} ret;

/* Vetor de trechos. */
trecho *trechos;

/* Vetor(uma matriz, na verdade =P) estrada e um vetor auxiliar que
 * armazena quantas posicoes estao ocupadas em cada km. */
int **estrada;
int *next_position;

/* tickets do algoritmo da padaria. */
unsigned long int cur_ticket;
unsigned long int next_ticket;

/* variaveis utilizadas pela barreira. */
pthread_mutex_t barreira;
pthread_cond_t go;

/* Variaveis do problema. */
int num_ciclistas;
int num_arrived;
int tamanho_estrada;
int largura_estrada;
int fim;
int chegaram;
int minuto;

void *ciclista(void *arg);
int alocar_coisas();
int compare_time(const void *a, const void *b);
int compare_subida(const void *a, const void *b);
int compare_plano(const void *a, const void *b);
#endif
