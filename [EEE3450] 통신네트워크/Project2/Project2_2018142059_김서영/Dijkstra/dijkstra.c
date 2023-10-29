#include <assert.h>	// assert
#include "dijkstra.h"

void Dijkstra(pGraph pG, int s)
{
	int i, u;
	assert(s >= 0);
	assert(s < gGet_N(pG));

	Initialize(pG, s);

	while (!all_in_S(pG))
	{
		u = Extract_Min(pG);	// extract node with minimum cost among ones not in S

		gSet_in_S(pG, u, true);	// input to S node extracted

		for (i = 0; i < gGet_deg(pG, u); i++)	// loop for relaxation
			// pass if sink of i-th edge is not in S
			if (!gGet_in_S(pG, gGet_sink(pG, u, i)))
				Relaxation(pG, u, i);
	}
}

void Initialize(pGraph pG, int s)
{
	int i;

	for (i = 0; i < gGet_N(pG); i++)
	{
		gSet_in_S(pG, i, false);	//node i is not in the set S
		gSet_cost(pG, i, INF);	//cost of node i = INF
		gSet_prenode(pG, i, -1);	//predecessor of i = nothing
	}
	gSet_cost(pG, s, 0);	// cost of source = 0
	gSet_prenode(pG, s, s);	// predecessor of source = source
}

bool all_in_S(pGraph pG)
{
	int i;
	for (i = 0; i < gGet_N(pG); i++)
		if (!gGet_in_S(pG,i))	//when node i is not in S
			return false;
	return true;
}

int Extract_Min(pGraph pG)
{
	int i, min;

	// find the first node not in S
	i = 0;
	while (gGet_in_S(pG,i) && gGet_in_S(pG, i++));	//check minimum value in S
	min = i;

	for (; i < gGet_N(pG); i++)
	{
		// pass for node in S
		if (gGet_in_S(pG,i))
			continue;

		// renew min when node with less cost is found
		if (gGet_cost(pG, i) < gGet_cost(pG,min))
			min = i;
	}
	return min;
}

void Relaxation(pGraph pG, int u, int i)
{
	int cost_u, cost_v, weight_uv;
	double updated_weight;

	cost_u = gGet_cost(pG, u);	// cost of u
	cost_v = gGet_cost(pG, gGet_sink(pG, u, i));	// cost of v (v = sink of i-th edge of u)
	weight_uv = gGet_weight(pG, u, i);	// weight of directed edge from u to v

	if (INF == cost_u)
		return;

	if (cost_v > cost_u + weight_uv)	//update cost from u to i
	{
		gSet_cost(pG, gGet_sink(pG,u,i), cost_u+weight_uv);	//update cost as new cost
		gSet_prenode(pG, gGet_sink(pG,u,i), u);	//Update predecessor of node u
	}
}
