#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>
#include "random.h"
#define MINI_Slot 1  //msec
#define Slot 1  //sec
#define Frame_LENGTH Slot //1 frame duration = 1 sec
#define SIMTIME 100

typedef int64_t Clock;
typedef enum {
	IDLE,
	TRANSMIT,
	COLLISION,
} State;
typedef enum {
	NOTRANSMIT,
	TRANSMITTED,
	NOCOLLISION
} Trans;

int rand_num();
double rnd();
void main()
{   int N;
	int frame_obtained;
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
	Trans* transmissions;
	State channel;
	Clock txEndTime;
	Clock tick;
	fp = fopen("Result(Slotted_Des).txt", "w");
	
	N=4;  // initial # of nodes
	G=1.0; //initial offered load for unit time=1sec 

		totalRate=G/Slot; //expected number of transmitted frames for 1msec = offered load for 1msec
		//fprintf(fp, "N=%i\n", N);
		//fprintf(fp, "Offered load= Expected value of Transmitted Frame/sec\n");
		//fprintf(fp, "Throughput= # of success/sec\n");
		//fprintf(fp, "(Theoretical Offered load Theoretical throughput), (Experimental Offered load Experimental throughput) \n");

			p=sqrt((totalRate)/N);  //probability that a node will transmit a frame 
			q = 0.03;
			channel  = IDLE;
			total_frame_transmitted = 0;
			suc = 0;
			txEndTime = -1;
			tick = 0;
			transmissions = (Trans*)malloc(sizeof(Trans)*N);

			while(tick < SIMTIME)	//Time slots
			{
				frame_transmitted = 0;
				transmitted_without_collision = 0;

				for(int i=0; i < N; i++)
				{
					//Fill the codes (frame_transmitted)//
					//Arrival: Frame obtained when prob < p
					frame_obtained = 0;
					printf("Time slot %ld: (Arrival) ",tick);
					if(rnd() >= p) printf("No Frame Obtained at node %d (prob >= p)\n",i);
					else
					{
						frame_obtained++;
						printf("Fresh Frame obtained at node %d (prob < p)\n",i);
					}

					//Transmission: Frame transmitted when prob <p
					printf("Time slot %ld: (Transmission) ", tick);
					if(frame_obtained==0) 
					{
						printf("node %d has no Frame to transmit\n",i);
						transmissions[i]=NOTRANSMIT;
					}
					else if(rnd()<p) 
					{
						printf("Transmit at node %d (prob < p)",i);
						total_frame_transmitted++;
						frame_transmitted++;
						transmissions[i]=TRANSMITTED;


						//Frame can be transmitted without collision when prob < q
						if(rnd()<q)
						{
							transmitted_without_collision ++;
							transmissions[i]=NOCOLLISION;
						}
						printf("\n");
					}
					else 
					{
						printf("No Transmit at node %d (prob >= p)\n",i);
						transmissions[i]=NOTRANSMIT;
					}

					if(channel == IDLE && frame_transmitted==1)
					{
						channel = TRANSMIT;
					}

					else if(frame_transmitted - transmitted_without_collision > 1)
					{
						channel = COLLISION;
					}
				}

				//Channel
				if(frame_transmitted==0) printf("Time slot %ld: (Channel) No Transmit !!\n",tick);	//No Transmit
				else if(frame_transmitted==1)	//1 Transmit, Success!
				{
					suc+=frame_transmitted;
					printf("Time slot %ld: (Channel) Transmit Success !!\n",tick);
				}
				else if(frame_transmitted>1 && transmitted_without_collision==0)	//Many Frame, Collision!
				{
					printf("Time slot %ld: (Channel) Collision !!\n",tick);
				}
				else							//Many Frame, but no collision in some nodes! 
				{
					for(int i=0; i < N; i++)
					{
						if(frame_transmitted - transmitted_without_collision == 1 && transmissions[i]!=NOTRANSMIT)
						{
							suc++;
							printf("Time slot %ld: (Channel): Many Frames, but no collision in %d node !!\n",tick,i);
						}
						else if(transmissions[i]==NOCOLLISION)
						{
							suc++;
							printf("Time slot %ld: (Channel): Many Frames, but no collision in %d node !!\n",tick,i);
						}	
					}
				}

				tick++;
				printf("\n");
			}

			printf("\n");
	// 		fprintf(fp, "(%.6f %.8f)\t(%.8f %.8f)\n",
	// 				totalRate * Slot,
	// 				totalRate * Slot * (q + exp(-1 * totalRate * Slot)),
	// 				(double)(total_frame_transmitted) / SIMTIME * Slot,
	// 				(double)(suc) / SIMTIME * Slot);

	
	// fclose(fp);
}

double rnd()
{
	r_seed = fmod(A * r_seed, M);
	return (r_seed * 4.656612875e-10);
}

