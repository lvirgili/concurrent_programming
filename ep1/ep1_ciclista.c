#include "ep1_ciclista.h"

int alocar_coisas() {
     int i, j;
     estrada = (int **)malloc(tamanho_estrada*sizeof(int *));
     if (!estrada) {
          fprintf(stderr, "Erro alocando a estrada.\n");
     }
     next_position = (int *)malloc(tamanho_estrada*sizeof(int));
     if (next_position == NULL) {
          fprintf(stderr, "Erro alocando o vetor auxiliar.\n");
          return -1;
     } else {
          for (i = 0; i < tamanho_estrada; ++i) {
               estrada[i] = (int *)malloc(largura_estrada*sizeof(int));
               if (!estrada[i]) {
                    fprintf(stderr, "Erro alocando a estrada 2.\n");
                    return -1;
               }
          }
     }
     for (i = 0; i < tamanho_estrada; ++i) {
          next_position[i] = 0;
          for (j = 0; j < largura_estrada; ++j) {
               estrada[i][j] = -1;
          }
     }
     return 0;
}

static void print_info() {
     FILE *f;
     int i, j;
     if (chegaram < num_ciclistas) {
          f = fopen("saida.txt", "a");
          fprintf(f, "=== Minuto %d: ===\n", minuto);
          for (i = 0; i < tamanho_estrada; ++i) {
               if (next_position[i] > 0) {
                    fprintf(f, "Km %d: ", i);
                    for (j = 0; j < largura_estrada; ++j) {
                         if (estrada[i][j] != -1)
                              fprintf(f, "%d ", estrada[i][j]);
                    }
                    fprintf(f, "\n");
               }
          }
          fclose(f);
     }
}

static int give_points(trecho *cur_trecho, int cur_position, int tid) {
     int check_pos;
     switch (cur_trecho->tipo) {
     case T_PLANO:
          check_pos = (cur_trecho->fim + cur_trecho->inicio) / 2;
          break;
     case T_SUBIDA:
          check_pos = cur_trecho->fim-1;
          break;
     case T_DESCIDA:
          return 0;
          break;
     }
     if (cur_position == check_pos) {
          switch (cur_trecho->ja_passaram) {
          case 0:
               ++cur_trecho->ja_passaram;
               cur_trecho->quem_passou[0] = tid;
               return 45;
               break;
          case 1:
               ++cur_trecho->ja_passaram;
               cur_trecho->quem_passou[1] = tid;
               return 35;
               break;
          case 2:
               ++cur_trecho->ja_passaram;
               cur_trecho->quem_passou[2] = tid;
               return 25;
               break;
          case 3:
               ++cur_trecho->ja_passaram;
               return 15;
               break;
          case 4:
               ++cur_trecho->ja_passaram;
               return 10;
               break;
          case 5:
               ++cur_trecho->ja_passaram;
               return 5;
               break;
          default:
               ++cur_trecho->ja_passaram;
               return 0;
               break;
          }
     } else {
          return 0;
     }
}

/* Alterado de:
 * http://en.wikipedia.org/wiki/Fetch-and-add#x86_implementation. */
static inline int fetch_and_add(unsigned long int *variable, unsigned long int value){
     __asm__ volatile(
          "lock; xaddl %%eax, %2;"
          :"=a" (value)                   //Output
          : "a" (value), "m" (*variable)  //Input
          :"memory" );
     return value;
}

/* Pag. 248 do livro do Andrews. */
static void bar() {
     pthread_mutex_lock(&barreira);
     ++num_arrived;
     if (num_arrived < num_ciclistas) {
          pthread_cond_wait(&go, &barreira);
     } else {
          if (chegaram == (num_ciclistas)) {
               printf("============FIM===========\n");
               fim = 1;
          } else {
               print_info();
               ++minuto;
          }
          num_arrived = 0;
          pthread_cond_broadcast(&go);
     }
     pthread_mutex_unlock(&barreira);
}

/* As threads usam o algoritmo da padaria para controlar quem pode
 * acessar o vetor estrada e andar.  Quando todo mundo teve a
 * oportunidade de andar a barreira deixa comecar de novo.
 */
void *ciclista(void *arg) {
     int i, j, tid, cur_position, cur_trecho, pontuacoes[3], dist, cheguei;
     double v[3], km, move;
     unsigned long my_ticket;
     info *tinfo = (info *)arg;
     km = 0.0;
     cheguei = 0;
     cur_trecho = -1;
     cur_position = 0;
     tid = tinfo->tid;
     pontuacoes[T_DESCIDA] = pontuacoes[T_PLANO] = pontuacoes[T_SUBIDA] = 0;
     v[T_DESCIDA] = tinfo->velocidades[0];
     v[T_PLANO] = tinfo->velocidades[1];
     v[T_SUBIDA] = tinfo->velocidades[2];
     while (fim == 0) {
          if (cheguei == 0) {
               /* Thread pega um ticket. */
               my_ticket = fetch_and_add(&next_ticket,1);
               /* Espera a vez dela. Se nao for, ela abdica do seu tempo. */
               while (my_ticket != cur_ticket) {
                    sched_yield(); /* pthread_yield() warning? */
               }
               if (cur_trecho == -1) {
                    /* A thread nem entrou na estrada ainda. */
                    if (next_position[0] < largura_estrada) {
                         /* Tem espaco para entrar. */
                         for (i = 0; i < largura_estrada; ++i) {
                              if (estrada[0][i] == -1) {
                                   estrada[0][i] = tid;
                                   ++cur_trecho;
                                   ++next_position[0];
                                   break;
                              }
                         }
                    }
               }
               if (cur_trecho >= 0) {
                    /* A thread ja entrou, agora so vai andar. */
                    move = v[trechos[cur_trecho].tipo] / 60.0;
                    //if (floor(km+move) < trechos[cur_trecho].fim) {
                    dist = floor(km + move) - floor(km);
                    if (dist == 0) {
                         /* A thread nao vai mudar de km, logo so aumentar o qto ela andou. */
                         km += move;
                    } else {
                         for (j = 1; j <= dist; ++j) {
                              /* A thread acabou a corrida? */
                              if ((cur_position+1) == tamanho_estrada) {
                                   for (i = 0; i < largura_estrada; ++i) {
                                        if (estrada[cur_position][i] == tid) {
                                             estrada[cur_position][i] = -1;
                                             tinfo->ret[0] = minuto;
                                             tinfo->ret[1] = pontuacoes[T_DESCIDA];
                                             tinfo->ret[2] = pontuacoes[T_PLANO];
                                             tinfo->ret[3] = pontuacoes[T_SUBIDA];
                                             printf("a thread %d saiu no minuto %d estrada %d\n", tid, minuto, estrada[cur_position][i]);
                                             --next_position[cur_position];
                                             break;
                                        }
                                   }
                                   ++chegaram;
                                   cheguei = 1;
                                   break;
                              }
                              if (next_position[cur_position+1] < largura_estrada) {
                                   /* Tem espaco na proxima posicao. */
                                   for (i = 0; i < largura_estrada; ++i) {
                                        if (estrada[cur_position][i] == tid) {
                                             estrada[cur_position][i] = -1;
                                             --next_position[cur_position];
                                             break;
                                        }
                                   }
                                   ++cur_position;
                                   /* Aqui precisa chegar se ela chegou num checkpoint. */
                                   pontuacoes[trechos[cur_trecho].tipo] += give_points(&trechos[cur_trecho], cur_position, tid);
                                   if (cur_position >= trechos[cur_trecho].fim) {
                                        ++cur_trecho;
                                   }
                                   for (i = 0; i < largura_estrada; ++i) {
                                        if (estrada[cur_position][i] == -1) {
                                             estrada[cur_position][i] = tid;
                                             ++next_position[cur_position];
                                             break;
                                        }
                                   }
                                   km += move;
                              } else {
                                   printf("thread %d passou a vez.\n", tid);
                                   break;
                              }
                         }
                    }
               }
               /* Protocolo de saida, incrementar o ticket. */
               ++cur_ticket;
          }
          /* A barreira que sincroniza as threads. */
          bar();
     }
     pthread_exit(NULL);
}
