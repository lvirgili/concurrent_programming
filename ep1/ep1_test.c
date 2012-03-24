#include <stdio.h>
#include "ep1_ll.h"
#include "ep1_io.h"

int main(int argc, char **argv) {
     int flag;
     if (argc != 2) {
          printf("Uso: ./ep arquivo_de_entrada.txt\n");
          return 0;
     }
     flag = read_file(argv[1]);
     if (flag == -1) {
          printf("O arquivo %s nao pode ser lido.\n", argv[1]);
          return 0;
     }
     print_vars();
     ll_print();
     ll_clean();
     return 0;
}
