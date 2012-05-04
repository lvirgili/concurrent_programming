#ifndef GRAPH_H
#define GRAPH_H

#include <thread>
#include <iostream>
#include <atomic>
#include <vector>
#include <mutex>
#include <cmath>
using namespace std;

void init(int nthreads, int nvertex);
void test(int tid, int nthreads, bool debug);

#endif
