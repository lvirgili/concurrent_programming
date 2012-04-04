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
 * acessar o vetor estrada e andar. */
void *ciclista(void *arg) {
     info *tinfo = (info *)arg;
     int i, tid;
     double v[3];
     tid = tinfo->tid;
     v[0] = tinfo->velocidades[0];
     v[1] = tinfo->velocidades[1];
     v[2] = tinfo->velocidades[2];
     free(tinfo);
     unsigned long my_ticket;
     for (i = 0; i < 5; ++i) {
          my_ticket = fetch_and_add(&next_ticket,1);
          while (my_ticket != cur_ticket) {
               sched_yield(); /* pthread_yield() warning? */
          }
          printf("thead %d has velocities S = %lf, P = %lf, D = %lf in turn %d\n", tid, v[0], v[1], v[2], minuto);
          ++cur_ticket;
          bar();
     }
     pthread_exit(NULL);
}
