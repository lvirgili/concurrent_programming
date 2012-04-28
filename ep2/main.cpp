#include "ep2_io.hpp"
using namespace std;

int n, nvertex = 0;
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
     return 0;
}
