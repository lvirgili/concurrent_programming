#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "ep1_ll.h"
#include "ep1_io.h"
#include "ep1_stat.h"

int main(int argc, char **argv) {
     int flag, i;
     double d, s;
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
          d = rand_velocity(60,5);
          s = rand_velocity(30,5);
          if (d < 20.0) printf("d < 20\n");
          if (s > 80.0) printf("s > 80\n");
     }
     printf("velocidades ok.\n");
     return 0;
}
