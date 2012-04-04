#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "ep1_ll.h"
#include "ep1_io.h"
#include "ep1_stat.h"
#include "ep1_ciclista.h"

#define _OPEN_THREADS

int main(int argc, char **argv) {
     int flag, i;
     double de, s;
     pthread_attr_t attr;
     pthread_t ciclistasid[3];
     info *args;

     if (argc != 2) {
          printf("Uso: ./ep arquivo_de_entrada.txt\n");
          return 0;
     }
     srand(time(NULL));
     flag = read_file(argv[1]);
     if (flag == -1) {
          printf("O arquivo %s nao pode ser lido.\n", argv[1]);
          return 0;
     }
     tamanho_estrada = d;
     largura_estrada = n;
     minuto = 0;
     if (alocar_coisas() != 0) {
          exit(-1);
     }
     print_vars();
     ll_print();
     ll_clean();
     printf("STATS:\n");
     switch (v) {
     case 'U':
     case 'u':
          printf("Velocidade constante: %lf\n", 50.0);
          break;
     case 'A':
     case 'a':
          printf("Velocidades aleatorias:\nD: %lf\nP: %lf\nS: %lf\n", rand_velocity(60,5),rand_velocity(50,10),rand_velocity(30,5));
          break;
     }
     for (i = 0; i <= 10000; ++i) {
          de = rand_velocity(60,5);
          s = rand_velocity(30,5);
          if (de < 20.0) printf("de < 20\n");
          if (s > 80.0) printf("s > 80\n");
     }
     printf("velocidades ok.\n");
     cur_ticket = 0;
     next_ticket = 0;
     pthread_attr_init(&attr);
     pthread_attr_setscope(&attr, PTHREAD_SCOPE_SYSTEM);
     pthread_mutex_init(&barreira, NULL);
     pthread_cond_init(&go, NULL);
     num_ciclistas = 3;
     for (i = 0; i < 3; ++i) {
          args = (info *)malloc(sizeof(*args));
          args->velocidades[0] = rand_velocity(30,5);
          args->velocidades[1] = rand_velocity(50,10);
          args->velocidades[2] = rand_velocity(60,5);
          args->tid = i;
          pthread_create(&ciclistasid[i], &attr, ciclista, (void *)args);
     }
     pthread_exit(NULL);
}
