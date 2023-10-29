#pragma once
#include "graph.h"

#ifndef boolean
#define boolean
typedef enum { false, true } bool;
#endif

#define INF 9999

bool Bellman_Ford(pGraph, int source);	// find "least-cost path" and "corresponding cost"
bool Bellman_Ford_update(pGraph, int source, int cut);
void Initialize(pGraph, int source);	// initilize parameters
void Relaxation(pGraph, int node, int ith_edge);	// do relaxation of the cost
bool Exist_neg_Cycle(pGraph, int node, int ith_edge);	//
