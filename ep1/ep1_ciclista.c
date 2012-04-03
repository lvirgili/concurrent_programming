#include "ep1_ciclista.h"

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

void bar() {
     pthread_mutex_lock(&barreira);
     ++num_arrived;
     if (num_arrived < num_ciclistas) {
          pthread_cond_wait(&go, &barreira);
     } else {
          num_arrived = 0;
          pthread_cond_broadcast(&go);
     }
     pthread_mutex_unlock(&barreira);
}

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
     for (i = 0; i < 2; ++i) {
          my_ticket = fetch_and_add(&next_ticket,1);
          while (my_ticket != cur_ticket) {
               sched_yield(); /* pthread_yield() warning? */
          }
          printf("thead %d has velocities S = %lf, P = %lf, D = %lf in turn %d\n", tid, v[0], v[1], v[2], i);
          ++cur_ticket;
          bar();
     }
     pthread_exit(NULL);
}
