#pragma once
#include "graph.h"

#ifndef boolean
#define boolean
typedef enum { false, true } bool;
#endif

#define INF 9999

void Dijkstra(pGraph, int source);	// find "least-cost path" and "corresponding cost"
void Initialize(pGraph, int source);	// initilize parameters
bool all_in_S(pGraph);	// check whether or not all nodes are known
int Extract_Min(pGraph);	// extract node with minimum cost
void Relaxation(pGraph, int node, int ith_edge);	// do relaxation of the cost