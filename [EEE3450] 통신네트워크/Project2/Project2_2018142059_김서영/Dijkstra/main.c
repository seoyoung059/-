#include <stdio.h>
#include <stdlib.h>
#include "dijkstra.h"
#include "graph.h"

pGraph create_Graph_by_file(const char* data_file);
void display_Result(pGraph, int source);

int main(int argc, char** argv)
{
	int source;
	pGraph pG;

	system("clear");

	if (2 != argc)
	{
		printf("Usage: %s <graph data>\n", *argv);
		exit(1);
	}

	pG = create_Graph_by_file(*(argv+1));

	printf("\tinput source (A, B or ...): ");
	source = fgetc(stdin) - 'A';

	Dijkstra(pG, source);

	display_Result(pG, source);

	gDestroy(pG);
}

pGraph create_Graph_by_file(const char* file)
{
	int N, E, weight, e;
	char c, source, sink;
	FILE* fp;

	if (!(fp = fopen(file, "r")))
	{
		printf("Read the graph data error!\n");
		exit(1);
	}
	else
		printf("A file with graph data has been open.\n\n");

	fscanf(fp, "%i %i", &N, &E);
	pGraph pG = gCreate(N);
	printf("   A graph has been created with %i nodes.\n", N);

	e = 0;
	while (EOF != (c = fgetc(fp)))
	{
		if (c >= 'A' && c <= 'Z')
		{
			source = c;
			sink = fgetc(fp);
			fscanf(fp, "%i", &weight);
			gAdd_Edge(pG, (int)(source - 'A'), (int)(sink - 'A'), weight);
			printf("   An edge has been created (%c -> %c, weight: %i)\n", source, sink, weight);
			e++;
		}
	}

	if (e != E)
	{
		printf("# of edges error!\n");
		exit(1);
	}

	printf("\n   => %i nodes & %i edges have been created.\n\n", N, E);

	return pG;
}

void display_Result(pGraph pG, int source)
{
	int i, pre;

	puts("");
	for (i = 0; i < gGet_N(pG); i++)
	{
		if (i == source)
			continue;

		if (-1 == gGet_prenode(pG, i))
		{
			printf("\t%c to %c,    nonexistence\n", source + 'A', i + 'A');
			continue;
		}

		printf("\t%c to %c,    ", source + 'A', i + 'A');
		printf("cost: %2i    path: %c", gGet_cost(pG, i), i + 'A');

		pre = i;
		do {
			pre = gGet_prenode(pG, pre);
			printf(" <- %c", pre + 'A');
		} while (pre != source);
		puts("");
	}
}
