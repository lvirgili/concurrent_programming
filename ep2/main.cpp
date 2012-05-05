#include "ep2_io.hpp"
#include "ep2_graph.hpp"
using namespace std;

int n, nvertex = 0;
// Essa linha calcula o numero de threads.
int nthreads = sysconf(_SC_NPROCESSORS_ONLN);
bool debug = false;
// Vetor que armazena a matriz de adjacencias.
vector<vector<bool> > adj;

// A main nao faz nada, so chama as funcoes que leem a entrada e as
// threads e fica esperando todas acabarem.
int main(int argc, char **argv) {
     read_input(argc, argv, &n, &nvertex, &debug, adj);
     if (nthreads == 1) {
          nthreads = 2;
     }
     init(nthreads, nvertex);
     vector<thread> t;
     for (int i = 0; i < nthreads; ++i) {
          t.push_back(thread(gen_paths, n, i, nvertex, nthreads, adj, debug));
     }
     for (int i = 0; i < nthreads; ++i) {
          t[i].join();
     }
     return 0;
}
