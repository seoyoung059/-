#include <malloc.h>	// malloc, realloc, free
#include <assert.h>	// assert
#include "graph.h"	// typdef, function prototypes

struct graph {
	int N;	// # of nodes
	int E;	// # of edges
	pNode node[1];	// node[i] = i-th node
};

struct node {
	int oldcost;	// cost before relaxation
	int newcost;	// cost after relaxation
	int prenode;	// predecessor in Dijkstra path
	int deg;		// # of out-degree edges
	pEdge edge[1];	// edge[i] = i-th edge
};

struct edge {
	int sink;	// sink node edge
	int weight;	// weight of edge
};

pGraph gCreate(int n)
{
	pGraph pG;
	int i;

	// allocate memory: graph
	pG = (pGraph)malloc(sizeof(struct graph) + sizeof(pNode) * (n - 1));
	assert(pG);

	pG->N = n;
	pG->E = 0;

	for (i = 0; i < n; i++) {
		// allocate memory: nodes
		pG->node[i] = (pNode)malloc(sizeof(struct node) - sizeof(pEdge));
		assert(pG->node[i]);
		pG->node[i]->deg = 0;
	}
	return pG;
}

void gDestroy(pGraph pG)
{
	int i, j;
	for (i = 0; i < pG->N; i++) {	// loop for nodes
		for (j = 0; j < pG->node[i]->deg; j++)	// loop for edges
			free(pG->node[i]->edge[j]);	// deallocate memory: edges
		free(pG->node[i]);	// deallocate memory: nodes
	}
	free(pG);	// deallocate memory: graph
}

void gAdd_Edge(pGraph pG, int u, int v, int c)
{
	assert(u >= 0);
	assert(u < pG->N);
	assert(v >= 0);
	assert(v < pG->N);

	// expand memory: + edge
	pG->node[u] = (pNode)realloc(pG->node[u], sizeof(struct node) + sizeof(pEdge) * (pG->node[u]->deg));

	// allocate memory: edge
	pG->node[u]->edge[pG->node[u]->deg] = (pEdge)malloc(sizeof(struct edge));
	pG->node[u]->edge[pG->node[u]->deg]->sink = v;
	pG->node[u]->edge[pG->node[u]->deg]->weight = c;

	// count +1
	pG->node[u]->deg++;
	pG->E++;
}

int gGet_N(pGraph pG)
{
	return pG->N;
}

int gGet_E(pGraph pG)
{
	return pG->E;
}

int gGet_oldcost(pGraph pG, int node)
{
	assert(node >= 0);
	assert(node < pG->N);

	return pG->node[node]->oldcost;
}

int gGet_prenode(pGraph pG, int node)
{
	assert(node >= 0);
	assert(node < pG->N);

	return pG->node[node]->prenode;
}

int gGet_deg(pGraph pG, int node)
{
	assert(node >= 0);
	assert(node < pG->N);

	return pG->node[node]->deg;
}

int gGet_sink(pGraph pG, int node, int i)
{
	assert(node >= 0);
	assert(node < pG->N);
	assert(i >= 0);
	assert(i < pG->node[node]->deg);

	return pG->node[node]->edge[i]->sink;
}

int gGet_weight(pGraph pG, int node, int i)
{
	assert(node >= 0);
	assert(node < pG->N);
	assert(i >= 0);
	assert(i < pG->node[node]->deg);

	return pG->node[node]->edge[i]->weight;
}

void gSet_newcost(pGraph pG, int node, int D)
{
	assert(node >= 0);
	assert(node < pG->N);
	pG->node[node]->newcost = D;
}

void gSet_prenode(pGraph pG, int node, int predecessor)
{
	pG->node[node]->prenode = predecessor;
}

void gRenew_cost(pGraph pG)
{
	int i;
	for (i = 0; i < pG->N; i++)
		pG->node[i]->oldcost = pG->node[i]->newcost;
}

void gChange_weight(pGraph pG, int node)
{
	int i;
	for (i = 0; i < pG->node[node]->deg; i++)
		pG->node[node]->edge[i]->weight = 100;
}
