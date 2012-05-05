#ifndef GRAPH_H
#define GRAPH_H

#include <thread>
#include <iostream>
#include <atomic>
#include <vector>
#include <mutex>
#include <cmath>
#include <algorithm>
using namespace std;

void init(int nthreads, int nvertex);
void test(int tid, int nthreads, bool debug);
void gen_paths(int n, int tid, int nvertex, int nthreads, vector<vector<bool> > adj, bool debug);

#endif
