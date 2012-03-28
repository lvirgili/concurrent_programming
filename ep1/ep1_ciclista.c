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
