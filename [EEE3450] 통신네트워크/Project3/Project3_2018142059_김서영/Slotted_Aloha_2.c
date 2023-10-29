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
	int transmitted_without_collision;
	int suc;
	int total_frame_transmitted;
	double G;
	double eachRate;
	double totalRate;
	double p;
	double q;
	FILE *fp;
	State channel;
	Clock txEndTime;
	Clock tick;
	fp = fopen("Result(Slotted_2).txt", "w");
	
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
			q = 0.03;
			channel  = IDLE;
			total_frame_transmitted = 0;
			suc = 0;
			txEndTime = -1;
			tick = 0;

			printf("Offered load(Frame/sec) = %.6f/sec, N = %d\n",
					totalRate * Slot, N);

			while(tick < SIMTIME)	//Time slots
			{
				frame_transmitted = 0;
				transmitted_without_collision = 0;

				for(int i=0; i < N; i++)
				{
					//Fill the codes (frame_transmitted)//
					//Arrival: Frame obtained when prob < p
					if(rnd() >= p) continue;

					//Transmission: Frame transmitted when prob <p
					if(rnd()<p) 
					{
						total_frame_transmitted++;
						frame_transmitted++;

						//Frame can be transmitted without collision when prob < q
						if(rnd()<q) transmitted_without_collision ++;
					}

					if(channel == IDLE && frame_transmitted==1)
					{
						channel = TRANSMIT;

					}

					else if(frame_transmitted-transmitted_without_collision > 1)
					{
						channel = COLLISION;
					}
				}

				//Channel
				if(channel==TRANSMIT)
				{
					suc+=frame_transmitted;
				}
				else if(channel==COLLISION)
				{
					suc+=transmitted_without_collision;
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
					totalRate * Slot * (q+exp(-1 * totalRate * Slot)),
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

