#ifndef CICLISTA_H
#define CICLISTA_H
#include <pthread.h>
#include <stdio.h>

unsigned long int cur_ticket;
unsigned long int next_ticket;

int fetch_and_add(unsigned long int *variable, unsigned long int value);
void *ciclista(void *arg);
#endif
