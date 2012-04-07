#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "ep1_ll.h"
#include "ep1_io.h"
#include "ep1_stat.h"
#include "ep1_ciclista.h"

#define _OPEN_THREADS

void ll_trechos(trecho *trechos) {
     struct node *p = head;
     int i = 0, last = 0;
     while (p != NULL) {
          switch(p->t_trecho) {
          case 'D':
               trechos[i].tipo = T_DESCIDA;
               break;
          case 'P':
               trechos[i].tipo = T_PLANO;
               break;
          case 'S':
               trechos[i].tipo = T_SUBIDA;
               break;
          }
          trechos[i].inicio = last;
          trechos[i].fim = trechos[i].inicio + p->k;
          trechos[i].ja_passaram = 0;
          last = trechos[i].fim;
          p = p->p;
          ++i;
     }
}

int main(int argc, char **argv) {
     int flag, i, num_trechos;
     pthread_attr_t attr;
     pthread_t *ciclistasid;
     info **args;

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
     printf("largura %d\n", largura_estrada);
     minuto = 0;
     if (alocar_coisas() != 0) {
          exit(-1);
     }
     print_vars();
     ll_print();

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
     num_trechos = ll_size;
     trechos = (trecho *)malloc(ll_size*sizeof(trecho));
     ll_trechos(trechos);
     for (i = 0; i < ll_size; ++i) {
          printf("Trecho %d: Tipo %d, inicio %d, fim %d\n", i, trechos[i].tipo, trechos[i].inicio, trechos[i].fim);
     }
     ciclistasid = (pthread_t *)malloc(m*sizeof(pthread_t));
     args = (info **)malloc(m*sizeof(info));
     printf("========== Informacoes sobre os ciclistas: ==========\n");
     for (i = 0; i < m; ++i) {
          args[i] = (info *)malloc(sizeof(info));
          if (v == 'A') {
               args[i]->velocidades[0] = rand_velocity(30,5);
               args[i]->velocidades[1] = rand_velocity(50,10);
               args[i]->velocidades[2] = rand_velocity(60,5);
          } else {
               args[i]->velocidades[0] = 50;
               args[i]->velocidades[1] = 50;
               args[i]->velocidades[2] = 50;
          }
          args[i]->tid = i;
          printf("O ciclista %d tem velocidades: S = %lf, P = %lf, D = %lf\n",
                 args[i]->tid, args[i]->velocidades[0], args[i]->velocidades[1], args[i]->velocidades[2]);
     }
     ll_clean();
     printf("========== Inicio da simulacao. ==========\n");
     cur_ticket = 0;
     next_ticket = 0;
     fim = 0;
     chegaram = 0;
     pthread_attr_init(&attr);
     pthread_attr_setscope(&attr, PTHREAD_SCOPE_SYSTEM);
     pthread_mutex_init(&barreira, NULL);
     pthread_cond_init(&go, NULL);
     num_ciclistas = m;
     for (i = 0; i < m; ++i) {
          pthread_create(&ciclistasid[i], &attr, ciclista, (void *)args[i]);
     }
     for (i = 0; i < m; ++i) {
          pthread_join(ciclistasid[i], NULL);
          printf("a thread %d retornou %d, %d, %d, %d.\n", i, args[i]->ret[0], args[i]->ret[1], args[i]->ret[2], args[i]->ret[3]);
     }
     for (i = 0; i < num_trechos; ++i) {
          if (trechos[i].tipo != T_DESCIDA)
               printf("No trecho %d os 3 primeiros foram: %d, %d e %d\n", i, trechos[i].quem_passou[0], trechos[i].quem_passou[1], trechos[i].quem_passou[2]);
     }
     for (i = 0; i < m; ++i) {
          free(args[i]);
     }
     free(args);
     printf("a main acabou.\n");
     pthread_exit(NULL);
}
