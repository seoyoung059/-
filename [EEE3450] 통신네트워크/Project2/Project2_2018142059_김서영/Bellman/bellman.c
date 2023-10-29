#include <assert.h>	// assert
#include "bellman.h"
#include <stdio.h>

bool Bellman_Ford(pGraph pG, int s)
{
	int i, j, k;
	assert(s >= 0);
	assert(s < gGet_N(pG));

	Initialize(pG, s);

	for (i = 0; i < gGet_N(pG) - 1; i++)
	{
		for (j = 0; j < gGet_N(pG); j++)
			for (k = 0; k < gGet_deg(pG, j); k++)
				Relaxation(pG, j, k);
		gRenew_cost(pG);
	}

	for (j = 0; j < gGet_N(pG); j++)
		for (k = 0; k < gGet_deg(pG, j); k++)
			if (Exist_neg_Cycle(pG, j, k))
				return false;

	return true;
}

bool Bellman_Ford_update(pGraph pG, int s, int c)
{
	int l;
	int i, j, k;

	//weight from certain node is changed
	gChange_weight(pG,c);
	for (l = 0; l < gGet_deg(pG,s); l++)
	{
		//reset cost of nodes that are connected to node from source node
		while(gGet_prenode(pG,c)!=s&&gGet_prenode(pG,c)!=-1)
		{
			//c = gGet_prenode(pG,c);
			gSet_newcost(pG,c,INF);
			c = gGet_prenode(pG,c);
		}
		
	}
	gRenew_cost(pG);
	
	for (i = 0; i < gGet_N(pG) -1; i++)
	{
		for (j = 0; j < gGet_N(pG); j++)
			for (k = 0; k < gGet_deg(pG, j); k++)
				Relaxation(pG, j, k);
		gRenew_cost(pG);
	}

	for (j=0; j < gGet_N(pG); j++)
		for (k=0; k < gGet_deg(pG, j); k++)
			if (Exist_neg_Cycle(pG, j, k))
				return false;

	return true;
}

void Initialize(pGraph pG, int s)
{
	int i;

	for (i = 0; i < gGet_N(pG); i++)
	{
		gSet_newcost(pG, i, INF);	//cost of node i = INF
		gSet_prenode(pG, i, -1);	//predecessor of node i = nothing
	}
	gSet_newcost(pG, s, 0);	//cost of source = 0
	gSet_prenode(pG, s, s);	//predecessor of source = source
	gRenew_cost(pG);	//update all costs of graph
}

void Relaxation(pGraph pG, int u, int i)
{
	int cost_u, cost_v, weight_uv;

	cost_u = gGet_oldcost(pG, u);	// cost of u
	cost_v = gGet_oldcost(pG, gGet_sink(pG, u, i));	// cost of v (v = sink of i-th edge of u)
	weight_uv = gGet_weight(pG, u, i);	// weight of directed edge from u to v

	if (INF == cost_u)
		return;

	if (cost_v > cost_u + weight_uv)
	//update the cost from u to i
	{
		gSet_newcost(pG, gGet_sink(pG, u, i), cost_u+weight_uv);
		gSet_prenode(pG, gGet_sink(pG, u, i), u);
	}
}

bool Exist_neg_Cycle(pGraph pG, int u, int i)
{
	int cost_u, cost_v, weight_uv;

	cost_u = gGet_oldcost(pG, u);	//cost of u
	cost_v = gGet_oldcost(pG, gGet_sink(pG, u, i));	//cost of v (v = sink of i-th edge of u)
	weight_uv = gGet_weight(pG, u, i);	//weight of directed edge from u to v

	if (INF == cost_u)
		return false;

	if (cost_v > cost_u + weight_uv)	//cost has correct values (hint. compare costs)
		return true;

	return false;
}

