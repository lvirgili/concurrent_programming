#include "ep2_io.hpp"
#include "ep2_graph.hpp"
using namespace std;

int n, nvertex = 0;
int nthreads = sysconf(_SC_NPROCESSORS_ONLN);
bool debug = false;
vector<vector<bool> > adj;

int main(int argc, char **argv) {
     read_input(argc, argv, &n, &nvertex, &debug, adj);
     cout << "n: " << n << endl << "nvertex: " << nvertex << endl << "debug: " << debug << endl;
     for (int i = 0; i < nvertex; ++i) {
          for (int j = 0; j < nvertex; ++j) {
               cout << adj[i][j] << ' ';
          }
          cout << endl;
     }
     if (nthreads == 1) {
          nthreads = 2;
     }
     cout << nthreads << endl;
     init(nthreads);
     thread *t;
     t = new thread[nthreads];
     for (int i = 0; i < nthreads; ++i) {
          t[i] = thread(test, i, nthreads);
     }
     for (int i = 0; i < nthreads; ++i) {
          t[i].join();
     }
     return 0;
}
