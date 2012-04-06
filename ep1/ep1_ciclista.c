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

void print_info() {
     FILE *f;
     int i, j;
     f = fopen("saida.txt", "a");
     fprintf(f, "Minuto %d:\n", minuto);
     for (i = 0; i < tamanho_estrada; ++i) {
          fprintf(f, "Km %d: ", i);
          for (j = 0; j < next_position[i]; ++j) {
               fprintf(f, "%d ", estrada[i][j]);
          }
          fprintf(f, "\n");
     }
     fclose(f);
}

int change_trecho(int cur_trecho, double km, double min_move) {
     double target = km + min_move;
     if (target > trechos[cur_trecho].fim) {
          return 1;
     }
     return 0;
}

/* Alterado de:
 * http://en.wikipedia.org/wiki/Fetch-and-add#x86_implementation. */
inline int fetch_and_add(unsigned long int *variable, unsigned long int value){
     __asm__ volatile(
          "lock; xaddl %%eax, %2;"
          :"=a" (value)                   //Output
          : "a" (value), "m" (*variable)  //Input
          :"memory" );
     return value;
}

/* Pag. 248 do livro do Andrews. */
void bar() {
     pthread_mutex_lock(&barreira);
     ++num_arrived;
     if (num_arrived < num_ciclistas) {
          pthread_cond_wait(&go, &barreira);
     } else {
          num_arrived = 0;
          print_info();
          ++minuto;
          pthread_cond_broadcast(&go);
     }
     pthread_mutex_unlock(&barreira);
}

/* As threads usam o algoritmo da padaria para controlar quem pode
 * acessar o vetor estrada e andar.  Quando todo mundo teve a
 * oportunidade de andar a barreira deixa comecar de novo.
 */
void *ciclista(void *arg) {
     int i, j, tid, cur_km, cur_position, pos, cur_trecho, pontuacoes[3];
     double v[3], km, move, rest, tempo;
     unsigned long my_ticket;
     info *tinfo = (info *)arg;
     km = 0;
     cur_km = 0;
     cur_trecho = 0;
     cur_position = -1;
     tid = tinfo->tid;
     pontuacoes[T_DESCIDA] = pontuacoes[T_PLANO] = pontuacoes[T_SUBIDA] = 0;
     v[T_DESCIDA] = tinfo->velocidades[0];
     v[T_PLANO] = tinfo->velocidades[1];
     v[T_SUBIDA] = tinfo->velocidades[2];
     while (km < tamanho_estrada) {
          /* Thread pega um ticket. */
          my_ticket = fetch_and_add(&next_ticket,1);
          /* Espera a vez dela. Se nao for, ela abdica do seu tempo. */
          while (my_ticket != cur_ticket) {
               sched_yield(); /* pthread_yield() warning? */
          }
          /* A partir daqui comeca a secao critica. */
          move = v[trechos[cur_trecho].tipo] / 3600;
          if (change_trecho(cur_trecho, km, move) == 0) {
               /* Aqui o infeliz ve se pode andar e faz o que precisa */
               for (i = ceil(cur_km); i <= floor(cur_km+move) && i < tamanho_estrada; ++i) {
                    if (next_position[i] < num_ciclistas) {
                         /* Pode andar para o proximo km. */
                         if (cur_position != -1) {
                              for (j = 0; j < largura_estrada; ++j) {
                                   if (estrada[cur_position][j] == tid) {
                                        /* Remove o identificador. */
                                        estrada[cur_position][j] = -1;
                                        --next_position[cur_position];
                                        break;
                                   }
                              }
                         }
                         for (j = 0; j < largura_estrada; ++j) {
                              if (estrada[i][j] == -1) {
                                   /* Armazena o identificador no km seguinte. */
                                   estrada[i][j] = tid;
                                   ++next_position[i];
                                   ++cur_position;
                                   break;
                              }
                         }
                    } else {
                         /* Tem gente na frente, entao nao anda mais. */
                         break;
                    }
               }
          } else {
               rest = trechos[cur_trecho].fim - km;
               tempo = rest / v[trechos[cur_trecho].tipo];
               /* Aqui ele ve se pode mudar de trecho e faz o que precisa. */
          }
          km += 5;
          /* Protocolo de saida, incrementar o ticket. */
          ++cur_ticket;
          /* A barreira que sincroniza as threads. */
          bar();
     }
     tinfo->ret[0] = (double)tid;
     tinfo->ret[1] = (double)tid;
     tinfo->ret[2] = (double)tid;
     pthread_exit(NULL);
}
