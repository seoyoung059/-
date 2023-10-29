#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>
#include "random.h"
#define MINI_Slot 1  //msec
#define Slot 1  //sec
#define Frame_LENGTH Slot //1 frame duration = 1 sec
#define SIMTIME 1000000

typedef int64_t Clock;
typedef enum {
	IDLE,
	TRANSMIT,
	COLLISION,
} State;

int rand_num();
double rnd();
void main()
{   int N;
	int frame_transmitted;
	int suc;
	int total_frame_transmitted;
	double G;
	double eachRate;
	double totalRate;
	double p;
	FILE *fp;
	State channel;
	Clock txEndTime;
	Clock tick;
	fp = fopen("Result(Slotted_1).txt", "w");
	
	N=2;  // initial # of nodes
	G=0.015625; //initial offered load for unit time=1sec 
	while(N <= 16)
	{
		totalRate=G/Slot; //expected number of transmitted frames for 1msec = offered load for 1msec
		fprintf(fp, "N=%i\n", N);
		fprintf(fp, "Offered load= Expected value of Transmitted Frame/sec\n");
		fprintf(fp, "Throughput= # of success/sec\n");
		fprintf(fp, "(Theoretical Offered load Theoretical throughput), (Experimental Offered load Experimental throughput) \n");

		while(totalRate <= 4.0/ Slot)
		{   p=sqrt((totalRate)/N);  //probability that a node will transmit a frame 
			channel  = IDLE;
			total_frame_transmitted = 0;
			suc = 0;
			txEndTime = -1;
			tick = 0;

			printf("Offered load(Frame/sec) = %.6f/sec, N = %d\n",
					totalRate * Slot, N);

			while(tick < SIMTIME)	//Time slots
			{
				for(int i=0; i < N; i++)
				{
					frame_transmitted=0;

					//Fill the codes (frame_transmitted)//
					//Arrival
					if(rnd() >= p) continue;

					//Transmission
					if(rnd()<p) 
					{
						total_frame_transmitted++;
						frame_transmitted++;
					}

					if(channel == IDLE && frame_transmitted!=0)
					{
						channel = TRANSMIT;
					}

					else if(frame_transmitted!=0)
					{
						channel = COLLISION;
					}
				}

				//Channel
				if(channel==TRANSMIT)
				{
					suc++;
				}
				channel = IDLE;
				

				if(tick % (SIMTIME/100) == 0)
				{
					printf("Simulation %6.2f%% Completed\r", tick / (double)SIMTIME * 100.0 + 1);
					fflush(stdout);
				}

				tick++;
			}

			printf("\n");
			fprintf(fp, "(%.6f %.8f)\t(%.8f %.8f)\n",
					totalRate * Slot,
					totalRate * Slot * exp(-1 * totalRate * Slot),
					(double)(total_frame_transmitted) / SIMTIME * Slot,
					(double)(suc) / SIMTIME * Slot);

			totalRate *= 2;
		}
		N *= 2;
	}
	fclose(fp);
}

double rnd()
{
	r_seed = fmod(A * r_seed, M);
	return (r_seed * 4.656612875e-10);
}

