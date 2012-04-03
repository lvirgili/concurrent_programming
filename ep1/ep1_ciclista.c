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

void *ciclista(void *arg) {
     int tid = (int)arg;
     int i;
     unsigned long my_ticket;
     for (i = 0; i < 2; ++i) {
          my_ticket = fetch_and_add(&next_ticket,1);

          while (my_ticket != cur_ticket) {
               pthread_yield(NULL);
          }
          printf("thread %d has the ticket %ld and the current turn is %ld\n", tid, my_ticket,cur_ticket);
          printf("thread %d turn\n", tid);
          ++cur_ticket;
     }
     pthread_exit(NULL);
}
