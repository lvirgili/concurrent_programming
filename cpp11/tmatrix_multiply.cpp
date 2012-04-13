// It is a quite simple program that uses C++11 threads to multiply
// two squares matrices.
//
// I used this site for the introduction to C++11 threads:
// http://solarianprogrammer.com/2011/12/16/cpp-11-thread-tutorial/
//
// The algorithm itself is in Cormen's algorithms book and I assume
// everywhere that has a magical command to make loops concurrent.
//
// The way to find out the number of cores available was found here:
// http://stackoverflow.com/questions/150355/programmatically-find-the-number-of-cores-on-a-machine
//
// I have no idea the name of the license that this program has. It
// has the license that allows you to do anything with it that does
// not hurt its feelings.

#include <iostream>
#include <thread>
#include <ctime>
using namespace std;

// This variable holds the size of the matrices.
static const int n = 1000;

int a[n][n], b[n][n], c[n][n], d[n][n];

// This is the function that the threads call. Each thread will
// receive a piece of the matrix to multiply.
void multiply_chunk(int start, int end) {
     for (int i = start; i < end; ++i) {
          for (int j = 0; j < n; ++j) {
               d[i][j] = 0;
               for (int k = 0; k < n; ++k) {
                    d[i][j] += a[i][k] * b[k][j];
               }
          }
     }
}

int main() {
     // Initializing the matrices with anything.
     for (int i = 0; i < n; ++i) {
          for (int j = 0; j < n; ++j) {
               a[i][j] = (i + j);
               b[i][j] = (i * j);
          }
     }
     time_t start, end;
     cout << "=== Serial turn ===\n";
     time(&start);
     // The classical way to do it.
     for (int i = 0; i < n; ++i) {
          for (int j = 0; j < n; ++j) {
               c[i][j] = 0;
               for (int k = 0; k < n; ++k) {
                    c[i][j] += a[i][k] * b[k][j];
               }
          }
     }
     time(&end);
     cout << "=== Multiplyed serially in " << difftime(end, start) << " seconds ===\n";

     // This variable holds the number of cores, which is also the
     // number of threads created, counting the main.
     int chunks = sysconf( _SC_NPROCESSORS_ONLN );
     thread t[chunks - 1];
     cout << "=== Threads turn ===\n";
     time(&start);
     // Divide the matrix in chunks and each chunk is processed
     // independently.
     for (int i = 0; i < (chunks - 1); ++i) {
          t[i] = thread(multiply_chunk, i * n / chunks, (i+1) * n / chunks);
     }
     // The main chunk.
     multiply_chunk((chunks - 1) * n / chunks, chunks * n / chunks);
     for (int i = 0; i < chunks - 1; ++i) {
          t[i].join();
     }
     time(&end);
     cout << "=== Threads finished in " << difftime(end, start) << " seconds ===\n";
     return 0;
}
