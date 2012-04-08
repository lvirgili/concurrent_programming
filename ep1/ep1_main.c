#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "ep1_ll.h"
#include "ep1_io.h"
#include "ep1_stat.h"
#include "ep1_ciclista.h"

#define _OPEN_THREADS

/* Essa funcao le a lista ligada que foi montada na entrada e preenche
 * o vetor de trechos da prova. */
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
     /* Le o nome do arquivo de entrada. */
     if (argc != 2) {
          printf("Uso: ./ep arquivo_de_entrada.txt\n");
          return 0;
     }
     /* A partir daqui a main le o arquivo e faz as alocacoes
      * necessarias para a simulacao. */
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
     num_trechos = ll_size;
     trechos = (trecho *)malloc(ll_size*sizeof(trecho));
     ll_trechos(trechos);
     ciclistasid = (pthread_t *)malloc(m*sizeof(pthread_t));
     args = (info **)malloc(m*sizeof(info));
     int comps[m][4];
     /* Esse loop imprime as velocidades dos ciclistas. */
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
     /* Aqui comeca a simulacao em si. As variaveis de controle sao
      * setadas e as threas inicializadas. */
     printf("========== Inicio da simulacao ==========\n");
     cur_ticket = 0;
     next_ticket = 0;
     fim = 0;
     chegaram = 0;
     pthread_attr_init(&attr);
     pthread_attr_setscope(&attr, PTHREAD_SCOPE_SYSTEM);
     pthread_mutex_init(&barreira, NULL);
     pthread_cond_init(&go, NULL);
     num_ciclistas = m;
     /* Loop que cria as threads. */
     for (i = 0; i < m; ++i) {
          pthread_create(&ciclistasid[i], &attr, ciclista, (void *)args[i]);
     }
     /* Aqui a main espera todo mundo acabar e recebe as pontucaoes de
      * cada ciclista. */
     for (i = 0; i < m; ++i) {
          pthread_join(ciclistasid[i], NULL);
          comps[i][0] = i;
          comps[i][1] = args[i]->ret[0];
          comps[i][2] = args[i]->ret[2];
          comps[i][3] = args[i]->ret[3];
     }
     /* Aqui sao impressos as classificacoes para cada camiseta e
      * depois as posicoes nos checkpoints. */
     printf("========== Classificacao \"Camiseta Amarela\" ==========\n");
     qsort(comps, m, 4*sizeof(int), compare_time);
     for (i = 0; i < m; ++i) {
          printf("%do lugar: %d\n", i+1, comps[i][0]);
     }
     printf("========== Classificacao \"Camiseta Branca com Bolas Vermelhas\" ==========\n");
     qsort(comps, m, 4*sizeof(int), compare_subida);
     for (i = 0; i < m; ++i) {
          printf("%do lugar: %d\n", i+1, comps[i][0]);
     }
     printf("========== Classificacao \"Camiseta Verde\" ==========\n");
     qsort(comps, m, 4*sizeof(int), compare_plano);
     for (i = 0; i < m; ++i) {
          printf("%do lugar: %d\n", i+1, comps[i][0]);
     }
     printf("========== Posicoes nos checkpoints ==========\n");
     for (i = 0; i < num_trechos; ++i) {
          if (trechos[i].tipo != T_DESCIDA) {
               printf("Checkpoint %d: %d, %d e %d\n", i, trechos[i].quem_passou[0], trechos[i].quem_passou[1], trechos[i].quem_passou[2]);
          }
     }
     /* Libera o que ainda nao tinha sido e acaba. */
     for (i = 0; i < m; ++i) {
          free(args[i]);
     }
     free(args);
     return 0;
}
