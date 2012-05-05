#include "ep2_io.hpp"

// Funcao que le a entrada.
void read_input(int argc, char **argv, int *n, int *nvertex, bool *debug, vector<vector<bool> >& adj) {
     int fpos = 0, npos = 0;
     if (argc < 3 || argc > 5) {
          cout << "Uso: ./ep2 [-debug] n /nome/do/arquivo\n";
          exit(-1);
     } else {
          if (argc == 3) {
               if (isdigit(argv[1][0]) == true) {
                    npos = 1;
                    fpos = 2;
               } else {
                    npos = 2;
                    fpos = 1;
               }
          } else {
               *debug = true;
               if (isdigit(argv[1][0]) == true) {
                    npos = 1;
                    if (strcmp(argv[2], "-debug") == 0) {
                         fpos = 3;
                    } else {
                         fpos = 2;
                    }
               } else if (isdigit(argv[2][0]) == true) {
                    npos = 2;
                    if (strcmp(argv[1], "-debug") == 0) {
                         fpos = 3;
                    } else {
                         fpos = 1;
                    }
               } else {
                    npos = 3;
                    if (strcmp(argv[1], "-debug") == 0) {
                         fpos = 2;
                    } else {
                         fpos = 1;
                    }
               }
          }
     }
     *n = atoi(argv[npos]);
     ifstream f(argv[fpos]);
     if (f.is_open() == true) {
          string s;
          getline(f, s);
          for (unsigned i = 0; i < s.size(); ++i) {
               if (s[i] == ' ') {
                    ++*nvertex;
               }
          }
          ++*nvertex;
          stringstream ss (stringstream::in | stringstream::out);
          ss << s;
          vector<bool> aux; adj.push_back(aux);
          for (int i = 0; i < *nvertex; ++i) {
               bool e; ss >> e;
               adj[0].push_back(e);
          }
          for (int i = 1; i < *nvertex; ++i) {
               vector<bool> aux; adj.push_back(aux);
               for (int j = 0; j < *nvertex; ++j) {
                    bool e; f >> e;
                    adj[i].push_back(e);
               }
          }
     } else {
          cout << "Erro ao ler o arquivo.\n";
          exit(-1);
     }
}
