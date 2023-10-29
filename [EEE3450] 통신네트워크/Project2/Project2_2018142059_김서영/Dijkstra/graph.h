#pragma once

#ifndef boolean
#define boolean
typedef enum { false, true } bool;
#endif

typedef struct graph* pGraph;
typedef struct node* pNode;
typedef struct edge* pEdge;

pGraph gCreate(int n);	// create graph with n nodes
void gDestroy(pGraph);	// destroy graph
void gAdd_Edge(pGraph, int source, int sink, int weight);	// add edge from source to sink with weight

int gGet_N(pGraph);	// get N (# of nodes)
int gGet_E(pGraph);	// get E (# of edges)
bool gGet_in_S(pGraph, int node);	// get in_S of node (true: node is in S, false: node is not in S)
int gGet_cost(pGraph, int node);	// get cost of node
int gGet_prenode(pGraph pG, int node);	// get prenode of node (in Dijkstra path)
int gGet_deg(pGraph, int node);	// get deg of node (# of out-degree edges)
int gGet_sink(pGraph, int node, int i);	// get sink of i-th edge of node
int gGet_weight(pGraph, int node, int i);	// get weight of i-th edge of node

void gSet_in_S(pGraph, int node, bool val);	// set in_S of node = val
void gSet_cost(pGraph, int node, int val);	// set cost of node = val
void gSet_prenode(pGraph, int node, int val);	// set prenode of node = val
