/************************* (C) COPYRIGHT 2010 ROBOTIS **************************
* File Name          : main.c
* Author             : danceww
* Version            : V0.0.1
* Date               : 08/23/2010
* Description        : Main program body
*******************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x_lib.h"
#include "bt_hal.h"
#include "bt410.h"
#include "dynamixel.h"
#include "dxl_hal.h"
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/

#define P_GOAL_POSITION_L		30
#define P_GOAL_POSITION_H		31
#define P_PRESENT_POSITION_L	36
#define P_PRESENT_POSITION_H	37
#define P_MOVING				46

#define PORT_LED_AUX			GPIOB
#define PORT_LED_MANAGE			GPIOB
#define PORT_LED_PROGRAM		GPIOB
#define PORT_LED_PLAY			GPIOB
#define PORT_LED_POWER			GPIOC
#define PORT_LED_TX				GPIOC
#define PORT_LED_RX				GPIOC
#define PORT_BT_TXD			GPIOC
#define PORT_BT_RXD			GPIOD
#define PORT_BT_RESET		GPIOA
#define PORT_ENABLE_TXD			GPIOB
#define PORT_ENABLE_RXD			GPIOB
#define PORT_DXL_TXD			GPIOB
#define PORT_DXL_RXD			GPIOB

#define PIN_LED_AUX				GPIO_Pin_12
#define PIN_LED_MANAGE			GPIO_Pin_13
#define PIN_LED_PROGRAM			GPIO_Pin_14
#define PIN_LED_PLAY			GPIO_Pin_15
#define PIN_LED_POWER			GPIO_Pin_13
#define PIN_LED_TX				GPIO_Pin_14
#define PIN_LED_RX				GPIO_Pin_15
#define PIN_BT_TXD			GPIO_Pin_12
#define PIN_BT_RXD			GPIO_Pin_2
#define PIN_PC_TXD				GPIO_Pin_10
#define PIN_PC_RXD              GPIO_Pin_11
#define PIN_BT_RESET		GPIO_Pin_12
#define PIN_ENABLE_TXD			GPIO_Pin_4
#define PIN_ENABLE_RXD			GPIO_Pin_5
#define PIN_DXL_TXD				GPIO_Pin_6
#define PIN_DXL_RXD				GPIO_Pin_7

#define word                    u16
#define byte                    u8

#define USART_DXL				0
#define USART_BT		 		1
#define USART_PC			    2

#define PACKET_DATA0    		2
#define INVERSE_PACKET_DATA0 	3
#define PACKET_DATA1    		4
#define INVERSE_PACKET_DATA1 	5
#define PACKET_LENGTH 			6

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
vu32                            gwTimingDelay;
volatile byte                   gbPacketWritePointer = 0; // PC, Wireless
volatile byte                   gbPacketReadPointer = 0;
volatile byte                   gbPacketPointer = 0;
volatile byte                   gbpPacketDataBuffer[16+1+16];
volatile byte                   gbpPacket[PACKET_LENGTH+2];
volatile byte                   gbNewPacket;
volatile word                   gwBTRxData;

u32                             Baudrate_BT = 57600;
vu32                            gwTimingDelay;
word                            RcvData;

volatile byte                   gbpRxInterruptBuffer[256]; // dxl buffer
volatile byte                   gbRxBufferWritePointer,gbRxBufferReadPointer;
volatile vu32                   gwTimingDelay,gw1msCounter;
u32                             Baudrate_DXL = 	1000000;
u32                             Baudrate_PC = 57600;
vu16                            CCR1_Val = 100; 		// 1ms
vu32                            capture = 0;

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
void RCC_Configuration(void);
void NVIC_Configuration(void);
void GPIO_Configuration(void);
void SysTick_Configuration(void);
void RxD2Interrupt(void);
void __ISR_DELAY(void);
void EnableBT(void);
void DisableBT(void);
byte CheckNewArrive(void);
void mDelay(u32);
void PrintCommStatus(int);
void PrintErrorCode(void);
void TxDByte_BT(byte);
byte RxDByte_BT(void);
void TxDByte_DXL(byte);
byte RxDByte_DXL(void);
void TxDString(byte*);
void TxDWord16(word);
void TxDByte16(byte);
void TxDByte_PC(byte);

/*******************************************************************************
* Function Name  : main
* Description    : Main program
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/

volatile vu32 ms = 0;

int main(void)
{
	/* System Clocks Configuration */
	RCC_Configuration();

	/* NVIC configuration */
	NVIC_Configuration();

	/* Configure the GPIO ports */
	GPIO_Configuration();

	SysTick_Configuration();

	Timer_Configuration();

	GPIO_ResetBits(PORT_LED_POWER, PIN_LED_POWER);
	dxl_initialize( 0, 1 );	//init dynamicxel
	bt_initialize(0);		//init bt communication
	//USART_Configuration(USART_PC, 57600);
	mDelay(200);

	int tog = 1;
	int rxtog = 0;

	dxl_write_word( 3, P_GOAL_POSITION_L, 412 );
	dxl_write_word( 4, P_GOAL_POSITION_L, 612 );
	dxl_write_word( 5, P_GOAL_POSITION_L, 412 );
	dxl_write_word( 6, P_GOAL_POSITION_L, 612 );
	dxl_write_word( 7, P_GOAL_POSITION_L, 512 );
	dxl_write_word( 8, P_GOAL_POSITION_L, 512 );
	dxl_write_word( 9, P_GOAL_POSITION_L, 412 );
	dxl_write_word( 10, P_GOAL_POSITION_L, 612 );
	dxl_write_word( 11, P_GOAL_POSITION_L, 612 );
	dxl_write_word( 12, P_GOAL_POSITION_L, 412 );
	dxl_write_word( 13, P_GOAL_POSITION_L, 412 );
	dxl_write_word( 14, P_GOAL_POSITION_L, 612 );

	//Motor Names
	#define LFrontLeg		3
	#define RMidLeg 		8
	#define LBackLeg		11

    #define LFrontKnee		5
	#define RMidKnee		10
	#define LBackKnee		13

	#define RFrontLeg		4
	#define LMidLeg			7
	#define RBackLeg		12

    #define RFrontKnee		6
	#define LMidKnee		9
	#define RBackKnee		2

	//Standing Motor Positions
	#define LFrontLeg_stand		412
	#define RMidLeg_stand		512
	#define LBackLeg_stand		612

	#define RFrontLeg_stand		612
	#define LMidLeg_stand		512
	#define RBackLeg_stand		412

	#define LKnees_stand		412					//412 Task1:412 Task2: 512
	#define RKnees_stand		612					//612 Task1:612 Task2: 512

	//Leg Forward Positions
	#define delta				25
	#define LFrontLeg_forward	(412 - delta)		//412
	#define RMidLeg_forward		(512 + delta)		//512
	#define LBackLeg_forward	(612 - delta*0.5)	//612

	#define RFrontLeg_forward	(612 + delta)		//612
	#define LMidLeg_forward		(512 - delta)		//512
	#define RBackLeg_forward	(412 + delta*0.5)	//412

	//Leg Backward Positions
	#define LFrontLeg_backward	(412 + delta) 		//412
	#define RMidLeg_backward 	(512 - delta)		//512
	#define LBackLeg_backward	(612 + delta*1.5)	//612

	#define RFrontLeg_backward	(612 - delta)		//612
	#define LMidLeg_backward	(512 + delta)		//512
	#define RBackLeg_backward	(412 - delta*1.5)	//412

	//Knee Lift Positions
	#define LKnees_lift			(412 - 65)			//412 Task1:412 Task2: 512
	#define RKnees_lift 		(612 + 65)			//612 Task1:612 Task2: 512

	//Knee Down Positions
	#define LKnees_down			412					//412 Task1:412 Task2: 512
	#define RKnees_down			612					//612 Task1:612 Task2: 512
	#define LKnees_downer		(412 + 25)			//412 Task1:412 Task2: 512
	#define RKnees_downer		(612 - 25)			//612 Task1:612 Task2: 512

	int clock = 0;
	#define timing 1

	while(1)
	{
		if(bt_rx_check() == 1)
		{
			RcvData = bt_rx_data();
			if(rxtog){
				rxtog = 0;
				GPIO_ResetBits(PORT_LED_RX, PIN_LED_RX);
			}else{
				rxtog = 1;
				GPIO_SetBits(PORT_LED_RX, PIN_LED_RX);
			} //toggle RX LED when transmit BT well
			if(RcvData == 1){//when SendData is 1 **standard position

				dxl_write_word( LFrontLeg, P_GOAL_POSITION_L, LFrontLeg_stand );
				dxl_write_word( RMidLeg, P_GOAL_POSITION_L, RMidLeg_stand );
				dxl_write_word( LBackLeg, P_GOAL_POSITION_L, LBackLeg_stand );

				dxl_write_word( LFrontKnee, P_GOAL_POSITION_L, LKnees_stand );
				dxl_write_word( RMidKnee, P_GOAL_POSITION_L, RKnees_stand );
				dxl_write_word( LBackKnee, P_GOAL_POSITION_L, LKnees_stand );

				dxl_write_word( RFrontLeg, P_GOAL_POSITION_L, RFrontLeg_stand );
				dxl_write_word( LMidLeg, P_GOAL_POSITION_L, LMidLeg_stand );
				dxl_write_word( RBackLeg, P_GOAL_POSITION_L, RBackLeg_stand );

				dxl_write_word( RFrontKnee, P_GOAL_POSITION_L, RKnees_stand );
				dxl_write_word( LMidKnee, P_GOAL_POSITION_L, LKnees_stand );
				dxl_write_word( RBackKnee, P_GOAL_POSITION_L, RKnees_stand );
			}
			else if(RcvData == 2){//Forwards Movement
				int run = 1;

				//Inbetweening
				dxl_write_word( LFrontLeg, P_GOAL_POSITION_L, LFrontLeg_stand );
				dxl_write_word( RMidLeg, P_GOAL_POSITION_L, RMidLeg_stand );
				dxl_write_word( LBackLeg, P_GOAL_POSITION_L, LBackLeg_stand );

				dxl_write_word( LFrontKnee, P_GOAL_POSITION_L, LKnees_stand );
				dxl_write_word( RMidKnee, P_GOAL_POSITION_L, RKnees_stand );
				dxl_write_word( LBackKnee, P_GOAL_POSITION_L, LKnees_stand );

				dxl_write_word( RFrontLeg, P_GOAL_POSITION_L, RFrontLeg_stand );
				dxl_write_word( LMidLeg, P_GOAL_POSITION_L, LMidLeg_stand );
				dxl_write_word( RBackLeg, P_GOAL_POSITION_L, RBackLeg_stand );

				dxl_write_word( RFrontKnee, P_GOAL_POSITION_L, RKnees_stand );
				dxl_write_word( LMidKnee, P_GOAL_POSITION_L, LKnees_stand );
				dxl_write_word( RBackKnee, P_GOAL_POSITION_L, RKnees_stand );

				while(run){
					if(bt_rx_check() == 1) {
						break;
					}
					if(ms >= 45){ //ms increase every 1ms task1: 45 task 2: 75
						ms = 0;	//RESET ms
						if(clock > timing*8){ //10=1sec 20=2sec
							clock = 0;
						}else{
							clock = clock + 1;
						}
					}
					if(clock == timing*0){
						//Left Knees Lift
						dxl_write_word( LFrontKnee, P_GOAL_POSITION_L, LKnees_lift );
						dxl_write_word( RMidKnee, P_GOAL_POSITION_L, RKnees_lift );
						dxl_write_word( LBackKnee, P_GOAL_POSITION_L, LKnees_lift );
						//Right Knees Down
						dxl_write_word( RFrontKnee, P_GOAL_POSITION_L, RKnees_down );
						dxl_write_word( LMidKnee, P_GOAL_POSITION_L, LKnees_down );
						dxl_write_word( RBackKnee, P_GOAL_POSITION_L, RKnees_downer );
						//Left Legs Forward
						dxl_write_word( LFrontLeg, P_GOAL_POSITION_L, LFrontLeg_forward );
						dxl_write_word( RMidLeg, P_GOAL_POSITION_L, RMidLeg_forward );
						dxl_write_word( LBackLeg, P_GOAL_POSITION_L, LBackLeg_forward );
						//Right Legs Backward
						dxl_write_word( RFrontLeg, P_GOAL_POSITION_L, RFrontLeg_backward );
						dxl_write_word( LMidLeg, P_GOAL_POSITION_L, LMidLeg_backward );
						dxl_write_word( RBackLeg, P_GOAL_POSITION_L, RBackLeg_backward );

					} else if(clock == timing * 1){
						//Left Knees Down
						dxl_write_word( LFrontKnee, P_GOAL_POSITION_L, LKnees_down );
						dxl_write_word( RMidKnee, P_GOAL_POSITION_L, RKnees_down );
						dxl_write_word( LBackKnee, P_GOAL_POSITION_L, LKnees_downer );
						//Right Knees Down
						dxl_write_word( RFrontKnee, P_GOAL_POSITION_L, RKnees_down );
						dxl_write_word( LMidKnee, P_GOAL_POSITION_L, LKnees_down );
						dxl_write_word( RBackKnee, P_GOAL_POSITION_L, RKnees_downer );
						//Left Legs Forward
						dxl_write_word( LFrontLeg, P_GOAL_POSITION_L, LFrontLeg_forward );
						dxl_write_word( RMidLeg, P_GOAL_POSITION_L, RMidLeg_forward );
						dxl_write_word( LBackLeg, P_GOAL_POSITION_L, LBackLeg_forward );
						//Right Legs Backward
						dxl_write_word( RFrontLeg, P_GOAL_POSITION_L, RFrontLeg_backward );
						dxl_write_word( LMidLeg, P_GOAL_POSITION_L, LMidLeg_backward );
						dxl_write_word( RBackLeg, P_GOAL_POSITION_L, RBackLeg_backward );

					} else if(clock == timing * 3){
						//Left Knees Down
						dxl_write_word( LFrontKnee, P_GOAL_POSITION_L, LKnees_down );
						dxl_write_word( RMidKnee, P_GOAL_POSITION_L, RKnees_down );
						dxl_write_word( LBackKnee, P_GOAL_POSITION_L, LKnees_downer );
						//Right Knees Lift
						dxl_write_word( RFrontKnee, P_GOAL_POSITION_L, RKnees_lift );
						dxl_write_word( LMidKnee, P_GOAL_POSITION_L, LKnees_lift );
						dxl_write_word( RBackKnee, P_GOAL_POSITION_L, RKnees_lift );
						//Left Legs Forward
						dxl_write_word( LFrontLeg, P_GOAL_POSITION_L, LFrontLeg_forward );
						dxl_write_word( RMidLeg, P_GOAL_POSITION_L, RMidLeg_forward );
						dxl_write_word( LBackLeg, P_GOAL_POSITION_L, LBackLeg_forward );

						//Right Legs Backward
						dxl_write_word( RFrontLeg, P_GOAL_POSITION_L, RFrontLeg_backward );
						dxl_write_word( LMidLeg, P_GOAL_POSITION_L, LMidLeg_backward );
						dxl_write_word( RBackLeg, P_GOAL_POSITION_L, RBackLeg_backward );

					} else if(clock == timing * 4){
						//Left Knees Down
						dxl_write_word( LFrontKnee, P_GOAL_POSITION_L, LKnees_down );
						dxl_write_word( RMidKnee, P_GOAL_POSITION_L, RKnees_down );
						dxl_write_word( LBackKnee, P_GOAL_POSITION_L, LKnees_downer );
						//Right Knees Lift
						dxl_write_word( RFrontKnee, P_GOAL_POSITION_L, RKnees_lift );
						dxl_write_word( LMidKnee, P_GOAL_POSITION_L, LKnees_lift );
						dxl_write_word( RBackKnee, P_GOAL_POSITION_L, RKnees_lift );
						//Left Legs Backward
						dxl_write_word( LFrontLeg, P_GOAL_POSITION_L, LFrontLeg_backward );
						dxl_write_word( RMidLeg, P_GOAL_POSITION_L, RMidLeg_backward );
						dxl_write_word( LBackLeg, P_GOAL_POSITION_L, LBackLeg_backward );

						//Right Legs Forward
						dxl_write_word( RFrontLeg, P_GOAL_POSITION_L, RFrontLeg_forward );
						dxl_write_word( LMidLeg, P_GOAL_POSITION_L, LMidLeg_forward );
						dxl_write_word( RBackLeg, P_GOAL_POSITION_L, RBackLeg_forward );

					} else if(clock == timing * 5){
						//Left Knees Down
						dxl_write_word( LFrontKnee, P_GOAL_POSITION_L, LKnees_down );
						dxl_write_word( RMidKnee, P_GOAL_POSITION_L, RKnees_down );
						dxl_write_word( LBackKnee, P_GOAL_POSITION_L, LKnees_downer );
						//Right Knees down
						dxl_write_word( RFrontKnee, P_GOAL_POSITION_L, RKnees_down );
						dxl_write_word( LMidKnee, P_GOAL_POSITION_L, LKnees_down );
						dxl_write_word( RBackKnee, P_GOAL_POSITION_L, RKnees_downer );
						//Left Legs Backward
						dxl_write_word( LFrontLeg, P_GOAL_POSITION_L, LFrontLeg_backward );
						dxl_write_word( RMidLeg, P_GOAL_POSITION_L, RMidLeg_backward );
						dxl_write_word( LBackLeg, P_GOAL_POSITION_L, LBackLeg_backward );

						//Right Legs Forward
						dxl_write_word( RFrontLeg, P_GOAL_POSITION_L, RFrontLeg_forward );
						dxl_write_word( LMidLeg, P_GOAL_POSITION_L, LMidLeg_forward );
						dxl_write_word( RBackLeg, P_GOAL_POSITION_L, RBackLeg_forward );

					} else if(clock == timing * 7){
						//Left Knees Lift
						dxl_write_word( LFrontKnee, P_GOAL_POSITION_L, LKnees_lift );
						dxl_write_word( RMidKnee, P_GOAL_POSITION_L, RKnees_lift );
						dxl_write_word( LBackKnee, P_GOAL_POSITION_L, LKnees_lift );
						//Right Knees Down
						dxl_write_word( RFrontKnee, P_GOAL_POSITION_L, RKnees_down );
						dxl_write_word( LMidKnee, P_GOAL_POSITION_L, LKnees_down );
						dxl_write_word( RBackKnee, P_GOAL_POSITION_L, RKnees_downer );
						//Left Legs Backward
						dxl_write_word( LFrontLeg, P_GOAL_POSITION_L, LFrontLeg_backward );
						dxl_write_word( RMidLeg, P_GOAL_POSITION_L, RMidLeg_backward );
						dxl_write_word( LBackLeg, P_GOAL_POSITION_L, LBackLeg_backward );
						//Right Legs Forward
						dxl_write_word( RFrontLeg, P_GOAL_POSITION_L, RFrontLeg_forward );
						dxl_write_word( LMidLeg, P_GOAL_POSITION_L, LMidLeg_forward );
						dxl_write_word( RBackLeg, P_GOAL_POSITION_L, RBackLeg_forward );

					}
				}
				//Inbetweening
				dxl_write_word( LFrontLeg, P_GOAL_POSITION_L, LFrontLeg_stand );
				dxl_write_word( RMidLeg, P_GOAL_POSITION_L, RMidLeg_stand );
				dxl_write_word( LBackLeg, P_GOAL_POSITION_L, LBackLeg_stand );

				dxl_write_word( LFrontKnee, P_GOAL_POSITION_L, LKnees_stand );
				dxl_write_word( RMidKnee, P_GOAL_POSITION_L, RKnees_stand );
				dxl_write_word( LBackKnee, P_GOAL_POSITION_L, LKnees_stand );

				dxl_write_word( RFrontLeg, P_GOAL_POSITION_L, RFrontLeg_stand );
				dxl_write_word( LMidLeg, P_GOAL_POSITION_L, LMidLeg_stand );
				dxl_write_word( RBackLeg, P_GOAL_POSITION_L, RBackLeg_stand );

				dxl_write_word( RFrontKnee, P_GOAL_POSITION_L, RKnees_stand );
				dxl_write_word( LMidKnee, P_GOAL_POSITION_L, LKnees_stand );
				dxl_write_word( RBackKnee, P_GOAL_POSITION_L, RKnees_stand );
			}
			else if(RcvData == 3){//Backwards Movement
				int run = 1;

				//Inbetweening
				dxl_write_word( LFrontLeg, P_GOAL_POSITION_L, LFrontLeg_stand );
				dxl_write_word( RMidLeg, P_GOAL_POSITION_L, RMidLeg_stand );
				dxl_write_word( LBackLeg, P_GOAL_POSITION_L, LBackLeg_stand );

				dxl_write_word( LFrontKnee, P_GOAL_POSITION_L, LKnees_stand );
				dxl_write_word( RMidKnee, P_GOAL_POSITION_L, RKnees_stand );
				dxl_write_word( LBackKnee, P_GOAL_POSITION_L, LKnees_stand );

				dxl_write_word( RFrontLeg, P_GOAL_POSITION_L, RFrontLeg_stand );
				dxl_write_word( LMidLeg, P_GOAL_POSITION_L, LMidLeg_stand );
				dxl_write_word( RBackLeg, P_GOAL_POSITION_L, RBackLeg_stand );

				dxl_write_word( RFrontKnee, P_GOAL_POSITION_L, RKnees_stand );
				dxl_write_word( LMidKnee, P_GOAL_POSITION_L, LKnees_stand );
				dxl_write_word( RBackKnee, P_GOAL_POSITION_L, RKnees_stand );

				while(run){
					if(bt_rx_check() == 1) { break; }
					if(ms >= 45){ //ms increase every 1ms task 1: 45 task 2: 75
						ms = 0;	//RESET ms
						if(clock > timing*8){ //10=1sec 20=2sec
							clock = 0;
						}else{
							clock = clock + 1;
						}
					}
					if(clock == timing*0){
						//Left Knees down
						dxl_write_word( LFrontKnee, P_GOAL_POSITION_L, LKnees_down );
						dxl_write_word( RMidKnee, P_GOAL_POSITION_L, RKnees_down );
						dxl_write_word( LBackKnee, P_GOAL_POSITION_L, LKnees_downer );
						//Right Knees lift
						dxl_write_word( RFrontKnee, P_GOAL_POSITION_L, RKnees_lift );
						dxl_write_word( LMidKnee, P_GOAL_POSITION_L, LKnees_lift );
						dxl_write_word( RBackKnee, P_GOAL_POSITION_L, RKnees_lift );
						//Left Legs Forward
						dxl_write_word( LFrontLeg, P_GOAL_POSITION_L, LFrontLeg_forward );
						dxl_write_word( RMidLeg, P_GOAL_POSITION_L, RMidLeg_forward );
						dxl_write_word( LBackLeg, P_GOAL_POSITION_L, LBackLeg_forward );

						//Right Legs Backward
						dxl_write_word( RFrontLeg, P_GOAL_POSITION_L, RFrontLeg_backward );
						dxl_write_word( LMidLeg, P_GOAL_POSITION_L, LMidLeg_backward );
						dxl_write_word( RBackLeg, P_GOAL_POSITION_L, RBackLeg_backward );


					} else if(clock == timing*1){
						//Left Knees Down
						dxl_write_word( LFrontKnee, P_GOAL_POSITION_L, LKnees_down );
						dxl_write_word( RMidKnee, P_GOAL_POSITION_L, RKnees_down );
						dxl_write_word( LBackKnee, P_GOAL_POSITION_L, LKnees_downer );
						//Right Knees Down
						dxl_write_word( RFrontKnee, P_GOAL_POSITION_L, RKnees_down );
						dxl_write_word( LMidKnee, P_GOAL_POSITION_L, LKnees_down );
						dxl_write_word( RBackKnee, P_GOAL_POSITION_L, RKnees_downer );
						//Left Legs Forward
						dxl_write_word( LFrontLeg, P_GOAL_POSITION_L, LFrontLeg_forward );
						dxl_write_word( RMidLeg, P_GOAL_POSITION_L, RMidLeg_forward );
						dxl_write_word( LBackLeg, P_GOAL_POSITION_L, LBackLeg_forward );

						//Right Legs Backward
						dxl_write_word( RFrontLeg, P_GOAL_POSITION_L, RFrontLeg_backward );
						dxl_write_word( LMidLeg, P_GOAL_POSITION_L, LMidLeg_backward );
						dxl_write_word( RBackLeg, P_GOAL_POSITION_L, RBackLeg_backward );

					} else if(clock == timing*3){
						//Left Knees Lift
						dxl_write_word( LFrontKnee, P_GOAL_POSITION_L, LKnees_lift );
						dxl_write_word( RMidKnee, P_GOAL_POSITION_L, RKnees_lift );
						dxl_write_word( LBackKnee, P_GOAL_POSITION_L, LKnees_lift );
						//Right Knees down
						dxl_write_word( RFrontKnee, P_GOAL_POSITION_L, RKnees_down );
						dxl_write_word( LMidKnee, P_GOAL_POSITION_L, LKnees_down );
						dxl_write_word( RBackKnee, P_GOAL_POSITION_L, RKnees_downer );
						//Left Legs Forward
						dxl_write_word( LFrontLeg, P_GOAL_POSITION_L, LFrontLeg_forward );
						dxl_write_word( RMidLeg, P_GOAL_POSITION_L, RMidLeg_forward );
						dxl_write_word( LBackLeg, P_GOAL_POSITION_L, LBackLeg_forward );

						//Right Legs Backward
						dxl_write_word( RFrontLeg, P_GOAL_POSITION_L, RFrontLeg_backward );
						dxl_write_word( LMidLeg, P_GOAL_POSITION_L, LMidLeg_backward );
						dxl_write_word( RBackLeg, P_GOAL_POSITION_L, RBackLeg_backward );

					} else if(clock == timing*4){
						//Left Knees lift
						dxl_write_word( LFrontKnee, P_GOAL_POSITION_L, LKnees_lift );
						dxl_write_word( RMidKnee, P_GOAL_POSITION_L, RKnees_lift );
						dxl_write_word( LBackKnee, P_GOAL_POSITION_L, LKnees_lift );
						//Right Knees down
						dxl_write_word( RFrontKnee, P_GOAL_POSITION_L, RKnees_down );
						dxl_write_word( LMidKnee, P_GOAL_POSITION_L, LKnees_down );
						dxl_write_word( RBackKnee, P_GOAL_POSITION_L, RKnees_downer );
						//Left Legs Backward
						dxl_write_word( LFrontLeg, P_GOAL_POSITION_L, LFrontLeg_backward );
						dxl_write_word( RMidLeg, P_GOAL_POSITION_L, RMidLeg_backward );
						dxl_write_word( LBackLeg, P_GOAL_POSITION_L, LBackLeg_backward );

						//Right Legs Forward
						dxl_write_word( RFrontLeg, P_GOAL_POSITION_L, RFrontLeg_forward );
						dxl_write_word( LMidLeg, P_GOAL_POSITION_L, LMidLeg_forward );
						dxl_write_word( RBackLeg, P_GOAL_POSITION_L, RBackLeg_forward );
					} else if(clock == timing*5){
						//Left Knees Down
						dxl_write_word( LFrontKnee, P_GOAL_POSITION_L, LKnees_down );
						dxl_write_word( RMidKnee, P_GOAL_POSITION_L, RKnees_down );
						dxl_write_word( LBackKnee, P_GOAL_POSITION_L, LKnees_downer );
						//Right Knees down
						dxl_write_word( RFrontKnee, P_GOAL_POSITION_L, RKnees_down );
						dxl_write_word( LMidKnee, P_GOAL_POSITION_L, LKnees_down );
						dxl_write_word( RBackKnee, P_GOAL_POSITION_L, RKnees_downer );
						//Left Legs Backward
						dxl_write_word( LFrontLeg, P_GOAL_POSITION_L, LFrontLeg_backward );
						dxl_write_word( RMidLeg, P_GOAL_POSITION_L, RMidLeg_backward );
						dxl_write_word( LBackLeg, P_GOAL_POSITION_L, LBackLeg_backward );

						//Right Legs Forward
						dxl_write_word( RFrontLeg, P_GOAL_POSITION_L, RFrontLeg_forward );
						dxl_write_word( LMidLeg, P_GOAL_POSITION_L, LMidLeg_forward );
						dxl_write_word( RBackLeg, P_GOAL_POSITION_L, RBackLeg_forward );

					} else if(clock == timing*7){
						//Left Knees down
						dxl_write_word( LFrontKnee, P_GOAL_POSITION_L, LKnees_down );
						dxl_write_word( RMidKnee, P_GOAL_POSITION_L, RKnees_down );
						dxl_write_word( LBackKnee, P_GOAL_POSITION_L, LKnees_downer );
						//Right Knees lift
						dxl_write_word( RFrontKnee, P_GOAL_POSITION_L, RKnees_lift );
						dxl_write_word( LMidKnee, P_GOAL_POSITION_L, LKnees_lift );
						dxl_write_word( RBackKnee, P_GOAL_POSITION_L, RKnees_lift );
						//Left Legs Backward
						dxl_write_word( LFrontLeg, P_GOAL_POSITION_L, LFrontLeg_backward );
						dxl_write_word( RMidLeg, P_GOAL_POSITION_L, RMidLeg_backward );
						dxl_write_word( LBackLeg, P_GOAL_POSITION_L, LBackLeg_backward );

						//Right Legs Forward
						dxl_write_word( RFrontLeg, P_GOAL_POSITION_L, RFrontLeg_forward );
						dxl_write_word( LMidLeg, P_GOAL_POSITION_L, LMidLeg_forward );
						dxl_write_word( RBackLeg, P_GOAL_POSITION_L, RBackLeg_forward );


					}
				}
				//Inbetweening
				dxl_write_word( LFrontLeg, P_GOAL_POSITION_L, LFrontLeg_stand );
				dxl_write_word( RMidLeg, P_GOAL_POSITION_L, RMidLeg_stand );
				dxl_write_word( LBackLeg, P_GOAL_POSITION_L, LBackLeg_stand );

				dxl_write_word( LFrontKnee, P_GOAL_POSITION_L, LKnees_stand );
				dxl_write_word( RMidKnee, P_GOAL_POSITION_L, RKnees_stand );
				dxl_write_word( LBackKnee, P_GOAL_POSITION_L, LKnees_stand );

				dxl_write_word( RFrontLeg, P_GOAL_POSITION_L, RFrontLeg_stand );
				dxl_write_word( LMidLeg, P_GOAL_POSITION_L, LMidLeg_stand );
				dxl_write_word( RBackLeg, P_GOAL_POSITION_L, RBackLeg_stand );

				dxl_write_word( RFrontKnee, P_GOAL_POSITION_L, RKnees_stand );
				dxl_write_word( LMidKnee, P_GOAL_POSITION_L, LKnees_stand );
				dxl_write_word( RBackKnee, P_GOAL_POSITION_L, RKnees_stand );
			}
			///NOTE DECREASING TIMING INCREMENT BY ONE
			else if(RcvData == 4){//when SendData is 4 //turn clockwise
				int run = 1;
				//Inbetweening
				dxl_write_word( LFrontLeg, P_GOAL_POSITION_L, LFrontLeg_stand );
				dxl_write_word( RMidLeg, P_GOAL_POSITION_L, RMidLeg_stand );
				dxl_write_word( LBackLeg, P_GOAL_POSITION_L, LBackLeg_stand );

				dxl_write_word( LFrontKnee, P_GOAL_POSITION_L, LKnees_stand );
				dxl_write_word( RMidKnee, P_GOAL_POSITION_L, RKnees_stand );
				dxl_write_word( LBackKnee, P_GOAL_POSITION_L, LKnees_stand );

				dxl_write_word( RFrontLeg, P_GOAL_POSITION_L, RFrontLeg_stand );
				dxl_write_word( LMidLeg, P_GOAL_POSITION_L, LMidLeg_stand );
				dxl_write_word( RBackLeg, P_GOAL_POSITION_L, RBackLeg_stand );

				dxl_write_word( RFrontKnee, P_GOAL_POSITION_L, RKnees_stand );
				dxl_write_word( LMidKnee, P_GOAL_POSITION_L, LKnees_stand );
				dxl_write_word( RBackKnee, P_GOAL_POSITION_L, RKnees_stand );
				while(run){
					if(bt_rx_check() == 1) { break; }
					if(ms >= 35){ //ms increase every 1ms
						ms = 0;	//RESET ms
						if(clock >= timing*6){ //10=1sec 20=2sec
							clock = 0;
						}else{
							clock = clock +1;
						}
					}
					if(clock == timing*0){
						//Left Legs Forward
						dxl_write_word( LFrontLeg, P_GOAL_POSITION_L, LFrontLeg_forward );
						dxl_write_word( RMidLeg, P_GOAL_POSITION_L, RMidLeg_backward );
						dxl_write_word( LBackLeg, P_GOAL_POSITION_L, LBackLeg_forward );
						//Left Knees Lift
						dxl_write_word( LFrontKnee, P_GOAL_POSITION_L, LKnees_lift );
						dxl_write_word( RMidKnee, P_GOAL_POSITION_L, RKnees_lift );
						dxl_write_word( LBackKnee, P_GOAL_POSITION_L, LKnees_lift );
						//Right Legs Forward
						dxl_write_word( RFrontLeg, P_GOAL_POSITION_L, RFrontLeg_forward );
						dxl_write_word( LMidLeg, P_GOAL_POSITION_L, LMidLeg_backward );
						dxl_write_word( RBackLeg, P_GOAL_POSITION_L, RBackLeg_forward );
						//Right Knees Down
						dxl_write_word( RFrontKnee, P_GOAL_POSITION_L, RKnees_down );
						dxl_write_word( LMidKnee, P_GOAL_POSITION_L, LKnees_down );
						dxl_write_word( RBackKnee, P_GOAL_POSITION_L, RKnees_downer );

					} else if(clock == timing*1){
						//Left Legs Forward
						dxl_write_word( LFrontLeg, P_GOAL_POSITION_L, LFrontLeg_forward );
						dxl_write_word( RMidLeg, P_GOAL_POSITION_L, RMidLeg_backward );
						dxl_write_word( LBackLeg, P_GOAL_POSITION_L, LBackLeg_forward );
						//Left Knees Down
						dxl_write_word( LFrontKnee, P_GOAL_POSITION_L, LKnees_down );
						dxl_write_word( RMidKnee, P_GOAL_POSITION_L, RKnees_down );
						dxl_write_word( LBackKnee, P_GOAL_POSITION_L, LKnees_downer );
						//Right Legs Forward
						dxl_write_word( RFrontLeg, P_GOAL_POSITION_L, RFrontLeg_forward );
						dxl_write_word( LMidLeg, P_GOAL_POSITION_L, LMidLeg_backward );
						dxl_write_word( RBackLeg, P_GOAL_POSITION_L, RBackLeg_forward );
						//Right Knees Down
						dxl_write_word( RFrontKnee, P_GOAL_POSITION_L, RKnees_down );
						dxl_write_word( LMidKnee, P_GOAL_POSITION_L, LKnees_down );
						dxl_write_word( RBackKnee, P_GOAL_POSITION_L, RKnees_downer );
					} else if(clock == timing*2){
						//3
						//Left Legs Forward
						dxl_write_word( LFrontLeg, P_GOAL_POSITION_L, LFrontLeg_forward );
						dxl_write_word( RMidLeg, P_GOAL_POSITION_L, RMidLeg_backward );
						dxl_write_word( LBackLeg, P_GOAL_POSITION_L, LBackLeg_forward );
						//Left Knees Down
						dxl_write_word( LFrontKnee, P_GOAL_POSITION_L, LKnees_down );
						dxl_write_word( RMidKnee, P_GOAL_POSITION_L, RKnees_down );
						dxl_write_word( LBackKnee, P_GOAL_POSITION_L, LKnees_downer );
						//Right Legs Forward
						dxl_write_word( RFrontLeg, P_GOAL_POSITION_L, RFrontLeg_forward );
						dxl_write_word( LMidLeg, P_GOAL_POSITION_L, LMidLeg_backward );
						dxl_write_word( RBackLeg, P_GOAL_POSITION_L, RBackLeg_forward );
						//Right Knees Lift
						dxl_write_word( RFrontKnee, P_GOAL_POSITION_L, RKnees_lift );
						dxl_write_word( LMidKnee, P_GOAL_POSITION_L, LKnees_lift );
						dxl_write_word( RBackKnee, P_GOAL_POSITION_L, RKnees_lift );
					} else if(clock == timing*3){
						//4
						//Left Legs Backward
						dxl_write_word( LFrontLeg, P_GOAL_POSITION_L, LFrontLeg_backward );
						dxl_write_word( RMidLeg, P_GOAL_POSITION_L, RMidLeg_forward );
						dxl_write_word( LBackLeg, P_GOAL_POSITION_L, LBackLeg_backward );
						//Left Knees Down
						dxl_write_word( LFrontKnee, P_GOAL_POSITION_L, LKnees_down );
						dxl_write_word( RMidKnee, P_GOAL_POSITION_L, RKnees_down );
						dxl_write_word( LBackKnee, P_GOAL_POSITION_L, LKnees_downer );
						//Right Legs Backward
						dxl_write_word( RFrontLeg, P_GOAL_POSITION_L, RFrontLeg_backward );
						dxl_write_word( LMidLeg, P_GOAL_POSITION_L, LMidLeg_forward );
						dxl_write_word( RBackLeg, P_GOAL_POSITION_L, RBackLeg_backward );
						//Right Knees Lift
						dxl_write_word( RFrontKnee, P_GOAL_POSITION_L, RKnees_lift );
						dxl_write_word( LMidKnee, P_GOAL_POSITION_L, LKnees_lift );
						dxl_write_word( RBackKnee, P_GOAL_POSITION_L, RKnees_lift );
					} else if(clock == timing*4){
						//5
						//Left Legs Backward
						dxl_write_word( LFrontLeg, P_GOAL_POSITION_L, LFrontLeg_backward );
						dxl_write_word( RMidLeg, P_GOAL_POSITION_L, RMidLeg_forward );
						dxl_write_word( LBackLeg, P_GOAL_POSITION_L, LBackLeg_backward );
						//Left Knees Down
						dxl_write_word( LFrontKnee, P_GOAL_POSITION_L, LKnees_down );
						dxl_write_word( RMidKnee, P_GOAL_POSITION_L, RKnees_down );
						dxl_write_word( LBackKnee, P_GOAL_POSITION_L, LKnees_downer );
						//Right Legs backward
						dxl_write_word( RFrontLeg, P_GOAL_POSITION_L, RFrontLeg_backward );
						dxl_write_word( LMidLeg, P_GOAL_POSITION_L, LMidLeg_forward );
						dxl_write_word( RBackLeg, P_GOAL_POSITION_L, RBackLeg_backward );
						//Right Knees down
						dxl_write_word( RFrontKnee, P_GOAL_POSITION_L, RKnees_down );
						dxl_write_word( LMidKnee, P_GOAL_POSITION_L, LKnees_down );
						dxl_write_word( RBackKnee, P_GOAL_POSITION_L, RKnees_downer );
					} else if(clock == timing*5){
						//6
						//Left Legs Backward
						dxl_write_word( LFrontLeg, P_GOAL_POSITION_L, LFrontLeg_backward );
						dxl_write_word( RMidLeg, P_GOAL_POSITION_L, RMidLeg_forward );
						dxl_write_word( LBackLeg, P_GOAL_POSITION_L, LBackLeg_backward );
						//Left Knees Lift
						dxl_write_word( LFrontKnee, P_GOAL_POSITION_L, LKnees_lift );
						dxl_write_word( RMidKnee, P_GOAL_POSITION_L, RKnees_lift );
						dxl_write_word( LBackKnee, P_GOAL_POSITION_L, LKnees_lift );
						//Right Legs backward
						dxl_write_word( RFrontLeg, P_GOAL_POSITION_L, RFrontLeg_backward );
						dxl_write_word( LMidLeg, P_GOAL_POSITION_L, LMidLeg_forward );
						dxl_write_word( RBackLeg, P_GOAL_POSITION_L, RBackLeg_backward );
						//Right Knees Down
						dxl_write_word( RFrontKnee, P_GOAL_POSITION_L, RKnees_down );
						dxl_write_word( LMidKnee, P_GOAL_POSITION_L, LKnees_down );
						dxl_write_word( RBackKnee, P_GOAL_POSITION_L, RKnees_downer );

					}
				}
				//Inbetweening
				dxl_write_word( LFrontLeg, P_GOAL_POSITION_L, LFrontLeg_stand );
				dxl_write_word( RMidLeg, P_GOAL_POSITION_L, RMidLeg_stand );
				dxl_write_word( LBackLeg, P_GOAL_POSITION_L, LBackLeg_stand );

				dxl_write_word( LFrontKnee, P_GOAL_POSITION_L, LKnees_stand );
				dxl_write_word( RMidKnee, P_GOAL_POSITION_L, RKnees_stand );
				dxl_write_word( LBackKnee, P_GOAL_POSITION_L, LKnees_stand );

				dxl_write_word( RFrontLeg, P_GOAL_POSITION_L, RFrontLeg_stand );
				dxl_write_word( LMidLeg, P_GOAL_POSITION_L, LMidLeg_stand );
				dxl_write_word( RBackLeg, P_GOAL_POSITION_L, RBackLeg_stand );

				dxl_write_word( RFrontKnee, P_GOAL_POSITION_L, RKnees_stand );
				dxl_write_word( LMidKnee, P_GOAL_POSITION_L, LKnees_stand );
				dxl_write_word( RBackKnee, P_GOAL_POSITION_L, RKnees_stand );
			} //end
			else if(RcvData == 5){//when SendData is 4 //turn counterclockwise
				int run = 1;
				//Inbetweening
				dxl_write_word( LFrontLeg, P_GOAL_POSITION_L, LFrontLeg_stand );
				dxl_write_word( RMidLeg, P_GOAL_POSITION_L, RMidLeg_stand );
				dxl_write_word( LBackLeg, P_GOAL_POSITION_L, LBackLeg_stand );

				dxl_write_word( LFrontKnee, P_GOAL_POSITION_L, LKnees_stand );
				dxl_write_word( RMidKnee, P_GOAL_POSITION_L, RKnees_stand );
				dxl_write_word( LBackKnee, P_GOAL_POSITION_L, LKnees_stand );

				dxl_write_word( RFrontLeg, P_GOAL_POSITION_L, RFrontLeg_stand );
				dxl_write_word( LMidLeg, P_GOAL_POSITION_L, LMidLeg_stand );
				dxl_write_word( RBackLeg, P_GOAL_POSITION_L, RBackLeg_stand );

				dxl_write_word( RFrontKnee, P_GOAL_POSITION_L, RKnees_stand );
				dxl_write_word( LMidKnee, P_GOAL_POSITION_L, LKnees_stand );
				dxl_write_word( RBackKnee, P_GOAL_POSITION_L, RKnees_stand );

				while(run){
					if(bt_rx_check() == 1) { break; }
					if(ms >= 35){ //ms increase every 1ms
						ms = 0;	//RESET ms
						if(clock >= timing*6){ //10=1sec 20=2sec
							clock = 0;
						}else{
							clock = clock +1;
						}
					}
					if(clock == timing*0){
						//Left Legs backward
						dxl_write_word( LFrontLeg, P_GOAL_POSITION_L, LFrontLeg_backward );
						dxl_write_word( RMidLeg, P_GOAL_POSITION_L, RMidLeg_forward );
						dxl_write_word( LBackLeg, P_GOAL_POSITION_L, LBackLeg_backward );
						//Left Knees Lift
						dxl_write_word( LFrontKnee, P_GOAL_POSITION_L, LKnees_lift );
						dxl_write_word( RMidKnee, P_GOAL_POSITION_L, RKnees_lift );
						dxl_write_word( LBackKnee, P_GOAL_POSITION_L, LKnees_lift );
						//Right Legs backward
						dxl_write_word( RFrontLeg, P_GOAL_POSITION_L, RFrontLeg_backward );
						dxl_write_word( LMidLeg, P_GOAL_POSITION_L, LMidLeg_forward );
						dxl_write_word( RBackLeg, P_GOAL_POSITION_L, RBackLeg_backward );
						//Right Knees Down
						dxl_write_word( RFrontKnee, P_GOAL_POSITION_L, RKnees_down );
						dxl_write_word( LMidKnee, P_GOAL_POSITION_L, LKnees_down );
						dxl_write_word( RBackKnee, P_GOAL_POSITION_L, RKnees_downer );

					} else if(clock == timing*1){
						//Left Legs backward
						dxl_write_word( LFrontLeg, P_GOAL_POSITION_L, LFrontLeg_backward );
						dxl_write_word( RMidLeg, P_GOAL_POSITION_L, RMidLeg_forward );
						dxl_write_word( LBackLeg, P_GOAL_POSITION_L, LBackLeg_backward );
						//Left Knees Down
						dxl_write_word( LFrontKnee, P_GOAL_POSITION_L, LKnees_down );
						dxl_write_word( RMidKnee, P_GOAL_POSITION_L, RKnees_down );
						dxl_write_word( LBackKnee, P_GOAL_POSITION_L, LKnees_downer );
						//Right Legs backward
						dxl_write_word( RFrontLeg, P_GOAL_POSITION_L, RFrontLeg_backward );
						dxl_write_word( LMidLeg, P_GOAL_POSITION_L, LMidLeg_forward );
						dxl_write_word( RBackLeg, P_GOAL_POSITION_L, RBackLeg_backward );
						//Right Knees Down
						dxl_write_word( RFrontKnee, P_GOAL_POSITION_L, RKnees_down );
						dxl_write_word( LMidKnee, P_GOAL_POSITION_L, LKnees_down );
						dxl_write_word( RBackKnee, P_GOAL_POSITION_L, RKnees_downer );
					} else if(clock == timing*2){
						//3
						//Left Legs backward
						dxl_write_word( LFrontLeg, P_GOAL_POSITION_L, LFrontLeg_backward );
						dxl_write_word( RMidLeg, P_GOAL_POSITION_L, RMidLeg_forward );
						dxl_write_word( LBackLeg, P_GOAL_POSITION_L, LBackLeg_backward );
						//Left Knees Down
						dxl_write_word( LFrontKnee, P_GOAL_POSITION_L, LKnees_down );
						dxl_write_word( RMidKnee, P_GOAL_POSITION_L, RKnees_down );
						dxl_write_word( LBackKnee, P_GOAL_POSITION_L, LKnees_downer );
						//Right Legs backward
						dxl_write_word( RFrontLeg, P_GOAL_POSITION_L, RFrontLeg_backward );
						dxl_write_word( LMidLeg, P_GOAL_POSITION_L, LMidLeg_forward );
						dxl_write_word( RBackLeg, P_GOAL_POSITION_L, RBackLeg_backward );
						//Right Knees Lift
						dxl_write_word( RFrontKnee, P_GOAL_POSITION_L, RKnees_lift );
						dxl_write_word( LMidKnee, P_GOAL_POSITION_L, LKnees_lift );
						dxl_write_word( RBackKnee, P_GOAL_POSITION_L, RKnees_lift );
					} else if(clock == timing*3){
						//4
						//Left Legs forward
						dxl_write_word( LFrontLeg, P_GOAL_POSITION_L, LFrontLeg_forward );
						dxl_write_word( RMidLeg, P_GOAL_POSITION_L, RMidLeg_backward );
						dxl_write_word( LBackLeg, P_GOAL_POSITION_L, LBackLeg_forward );
						//Left Knees Down
						dxl_write_word( LFrontKnee, P_GOAL_POSITION_L, LKnees_down );
						dxl_write_word( RMidKnee, P_GOAL_POSITION_L, RKnees_down );
						dxl_write_word( LBackKnee, P_GOAL_POSITION_L, LKnees_downer );
						//Right Legs forward
						dxl_write_word( RFrontLeg, P_GOAL_POSITION_L, RFrontLeg_forward );
						dxl_write_word( LMidLeg, P_GOAL_POSITION_L, LMidLeg_backward );
						dxl_write_word( RBackLeg, P_GOAL_POSITION_L, RBackLeg_forward );
						//Right Knees Lift
						dxl_write_word( RFrontKnee, P_GOAL_POSITION_L, RKnees_lift );
						dxl_write_word( LMidKnee, P_GOAL_POSITION_L, LKnees_lift );
						dxl_write_word( RBackKnee, P_GOAL_POSITION_L, RKnees_lift );
					} else if(clock == timing*4){
						//4
						//Left Legs forward
						dxl_write_word( LFrontLeg, P_GOAL_POSITION_L, LFrontLeg_forward );
						dxl_write_word( RMidLeg, P_GOAL_POSITION_L, RMidLeg_backward );
						dxl_write_word( LBackLeg, P_GOAL_POSITION_L, LBackLeg_forward );
						//Left Knees Down
						dxl_write_word( LFrontKnee, P_GOAL_POSITION_L, LKnees_down );
						dxl_write_word( RMidKnee, P_GOAL_POSITION_L, RKnees_down );
						dxl_write_word( LBackKnee, P_GOAL_POSITION_L, LKnees_downer );
						//Right Legs forward
						dxl_write_word( RFrontLeg, P_GOAL_POSITION_L, RFrontLeg_forward );
						dxl_write_word( LMidLeg, P_GOAL_POSITION_L, LMidLeg_backward );
						dxl_write_word( RBackLeg, P_GOAL_POSITION_L, RBackLeg_forward );
						//Right Knees down
						dxl_write_word( RFrontKnee, P_GOAL_POSITION_L, RKnees_down );
						dxl_write_word( LMidKnee, P_GOAL_POSITION_L, LKnees_down );
						dxl_write_word( RBackKnee, P_GOAL_POSITION_L, RKnees_downer );
					} else if(clock == 5){
						//5
						//Left Legs forward
						dxl_write_word( LFrontLeg, P_GOAL_POSITION_L, LFrontLeg_forward );
						dxl_write_word( RMidLeg, P_GOAL_POSITION_L, RMidLeg_backward );
						dxl_write_word( LBackLeg, P_GOAL_POSITION_L, LBackLeg_forward );
						//Left Knees Lift
						dxl_write_word( LFrontKnee, P_GOAL_POSITION_L, LKnees_lift );
						dxl_write_word( RMidKnee, P_GOAL_POSITION_L, RKnees_lift );
						dxl_write_word( LBackKnee, P_GOAL_POSITION_L, LKnees_lift );
						//Right Legs forward
						dxl_write_word( RFrontLeg, P_GOAL_POSITION_L, RFrontLeg_forward );
						dxl_write_word( LMidLeg, P_GOAL_POSITION_L, LMidLeg_backward );
						dxl_write_word( RBackLeg, P_GOAL_POSITION_L, RBackLeg_forward );
						//Right Knees Down
						dxl_write_word( RFrontKnee, P_GOAL_POSITION_L, RKnees_down );
						dxl_write_word( LMidKnee, P_GOAL_POSITION_L, LKnees_down );
						dxl_write_word( RBackKnee, P_GOAL_POSITION_L, RKnees_downer );

					}
				}
				//Inbetweening
				dxl_write_word( LFrontLeg, P_GOAL_POSITION_L, LFrontLeg_stand );
				dxl_write_word( RMidLeg, P_GOAL_POSITION_L, RMidLeg_stand );
				dxl_write_word( LBackLeg, P_GOAL_POSITION_L, LBackLeg_stand );

				dxl_write_word( LFrontKnee, P_GOAL_POSITION_L, LKnees_stand );
				dxl_write_word( RMidKnee, P_GOAL_POSITION_L, RKnees_stand );
				dxl_write_word( LBackKnee, P_GOAL_POSITION_L, LKnees_stand );

				dxl_write_word( RFrontLeg, P_GOAL_POSITION_L, RFrontLeg_stand );
				dxl_write_word( LMidLeg, P_GOAL_POSITION_L, LMidLeg_stand );
				dxl_write_word( RBackLeg, P_GOAL_POSITION_L, RBackLeg_stand );

				dxl_write_word( RFrontKnee, P_GOAL_POSITION_L, RKnees_stand );
				dxl_write_word( LMidKnee, P_GOAL_POSITION_L, LKnees_stand );
				dxl_write_word( RBackKnee, P_GOAL_POSITION_L, RKnees_stand );
			}
		}
		if(ms >= 100){ //ms increase every 1ms
			if(tog){
				GPIO_ResetBits(PORT_LED_PLAY, PIN_LED_PLAY); //blinking
				tog = 0;
			}else{
				GPIO_SetBits(PORT_LED_PLAY, PIN_LED_PLAY);	//blinking
				tog = 1;
			}
			ms = 0;	//RESET ms
			if(clock >= 20){ //10=1sec 20=2sec
				clock = 0;
			}else{
				clock = clock +1;
			}
		}
	}
	return 0;
}

/*******************************************************************************
* Function Name  : RCC_Configuration
* Description    : Configures the different system clocks.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void RCC_Configuration(void)
{
	ErrorStatus HSEStartUpStatus;
	/* RCC system reset(for debug purpose) */
	RCC_DeInit();

	/* Enable HSE */
	RCC_HSEConfig(RCC_HSE_ON);

	/* Wait till HSE is ready */
	HSEStartUpStatus = RCC_WaitForHSEStartUp();

	if(HSEStartUpStatus == SUCCESS)
	{
		/* Enable Prefetch Buffer */
		FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable);

		/* Flash 2 wait state */
		FLASH_SetLatency(FLASH_Latency_2);

		/* HCLK = SYSCLK */
		RCC_HCLKConfig(RCC_SYSCLK_Div1);

		/* PCLK2 = HCLK */
		RCC_PCLK2Config(RCC_HCLK_Div1);

		/* PCLK1 = HCLK/2 */
		RCC_PCLK1Config(RCC_HCLK_Div2);

		/* PLLCLK = 8MHz * 9 = 72 MHz */
		RCC_PLLConfig(RCC_PLLSource_HSE_Div1, RCC_PLLMul_9);

		/* Enable PLL */
		RCC_PLLCmd(ENABLE);

		/* Wait till PLL is ready */
		while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET)
		{
		}

		/* Select PLL as system clock source */
		RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);

		/* Wait till PLL is used as system clock source */
		while(RCC_GetSYSCLKSource() != 0x08)
		{
		}
	}

	/* Enable peripheral clocks --------------------------------------------------*/

	/* Enable GPIOB and GPIOC clocks */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1 | RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC, ENABLE);

	/* Enable UART5 and USART3 clocks */
	RCC_APB1PeriphClockCmd ( RCC_APB1Periph_UART5 | RCC_APB1Periph_USART3 | RCC_APB1Periph_TIM2, ENABLE);

	PWR_BackupAccessCmd(ENABLE);
}


/*******************************************************************************
* Function Name  : NVIC_Configuration
* Description    : Configures Vector Table base location.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void NVIC_Configuration(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;

	#ifdef  VECT_TAB_RAM
		// Set the Vector Table base location at 0x20000000
		NVIC_SetVectorTable(NVIC_VectTab_RAM, 0x0);
	#else  // VECT_TAB_FLASH
		// Set the Vector Table base location at 0x08003000
		NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x3000);
	#endif

	// Configure the NVIC Preemption Priority Bits
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);

	NVIC_InitStructure.NVIC_IRQChannel = UART5_IRQChannel;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	// Enable the USART1 Interrupt
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQChannel;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	// Enable the TIM2 Interrupt
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQChannel;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

/*******************************************************************************
* Function Name  : GPIO_Configuration
* Description    : Configures the different GPIO ports.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void GPIO_Configuration(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_StructInit(&GPIO_InitStructure);

	// PORTA CONFIG
	GPIO_InitStructure.GPIO_Pin = 	PIN_BT_RESET;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	// PORTB CONFIG
	GPIO_InitStructure.GPIO_Pin = 	 PIN_LED_AUX | PIN_LED_MANAGE | PIN_LED_PROGRAM |
									PIN_LED_PLAY | PIN_ENABLE_RXD | PIN_ENABLE_TXD;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = PIN_DXL_RXD | PIN_PC_RXD;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = PIN_DXL_TXD | PIN_PC_TXD;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	// PORTC CONFIG
	GPIO_InitStructure.GPIO_Pin = 	 PIN_LED_POWER | PIN_LED_TX | PIN_LED_RX;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOC, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin =  PIN_BT_TXD;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	//GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOC, &GPIO_InitStructure);

	// PORTD CONFIG
	GPIO_InitStructure.GPIO_Pin = PIN_BT_RXD;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOD, &GPIO_InitStructure);

	GPIO_PinRemapConfig( GPIO_Remap_USART1, ENABLE);
	GPIO_PinRemapConfig( GPIO_Remap_SWJ_Disable, ENABLE);

	GPIO_ResetBits(PORT_ENABLE_TXD, PIN_ENABLE_TXD);	// TX Disable
	GPIO_SetBits(PORT_ENABLE_RXD, PIN_ENABLE_RXD);	// RX Enable
}

void USART1_Configuration(u32 baudrate)
{
	USART_Configuration(USART_DXL, baudrate);
}

void SysTick_Configuration(void)
{
	  /* SysTick end of count event each 1ms with input clock equal to 9MHz (HCLK/8, default) */
	  SysTick_SetReload(9000);

	  /* Enable SysTick interrupt */
	  SysTick_ITConfig(ENABLE);
}

void __ISR_DELAY(void)
{
	if (gwTimingDelay != 0x00)
		gwTimingDelay--;
}

void mDelay(u32 nTime)
{
	  /* Enable the SysTick Counter */
	  SysTick_CounterCmd(SysTick_Counter_Enable);

	  gwTimingDelay = nTime;

	  while(gwTimingDelay != 0);

	  /* Disable SysTick Counter */
	  SysTick_CounterCmd(SysTick_Counter_Disable);
	  /* Clear SysTick Counter */
	  SysTick_CounterCmd(SysTick_Counter_Clear);
}

void RxD2Interrupt(void)
{
	if(USART_GetITStatus(UART5, USART_IT_RXNE) != RESET)
	{
		word temp;
		temp = USART_ReceiveData(UART5);

		gbpPacketDataBuffer[gbPacketWritePointer] = temp;
		gbPacketWritePointer++;
		gbPacketWritePointer = gbPacketWritePointer & 0x1F;
		//TxDByte_BT(temp);	//echo
	}
}

/*__interrupt*/
void RxD1Interrupt(void)
{
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
		gbpRxInterruptBuffer[gbRxBufferWritePointer++] = USART_ReceiveData(USART1);
}

void USART_Configuration(u8 PORT, u32 baudrate)
{
	USART_InitTypeDef USART_InitStructure;

	USART_StructInit(&USART_InitStructure);

	USART_InitStructure.USART_BaudRate = baudrate;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No ;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

	if( PORT == USART_BT )
	{
		USART_DeInit(UART5);
		mDelay(10);
		/* Configure the UART5 */
		USART_Init(UART5, &USART_InitStructure);

		/* Enable UART5 Receive and Transmit interrupts */
		USART_ITConfig(UART5, USART_IT_RXNE, ENABLE);

		/* Enable the UART5 */
		USART_Cmd(UART5, ENABLE);
	}
	if( PORT == USART_DXL )
	{
		USART_DeInit(USART1);
		mDelay(10);
		/* Configure the USART1 */
		USART_Init(USART1, &USART_InitStructure);

		/* Enable USART1 Receive and Transmit interrupts */
		USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
		//USART_ITConfig(USART1, USART_IT_TC, ENABLE);

		/* Enable the USART1 */
		USART_Cmd(USART1, ENABLE);
	}
	else if( PORT == USART_PC )
	{

		USART_DeInit(USART3);
		mDelay(10);
		/* Configure the USART3 */
		USART_Init(USART3, &USART_InitStructure);

		/* Enable USART3 Receive and Transmit interrupts */
		//USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);
		//USART_ITConfig(USART3, USART_IT_TC, ENABLE);

		/* Enable the USART3 */
		USART_Cmd(USART3, ENABLE);
	}
}

void Timer_Configuration(void)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;

	TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
	TIM_OCStructInit(&TIM_OCInitStructure);

	TIM_DeInit(TIM2);

	/* Time base configuration */
	TIM_TimeBaseStructure.TIM_Period = 65535;
	TIM_TimeBaseStructure.TIM_Prescaler = 0;
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;

	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);

	/* Prescaler configuration */
	TIM_PrescalerConfig(TIM2, 722, TIM_PSCReloadMode_Immediate);

	/* Output Compare Timing Mode configuration: Channel1 */
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_Timing;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Disable;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OCInitStructure.TIM_Pulse = CCR1_Val ;

	TIM_OC1Init(TIM2, &TIM_OCInitStructure);
	TIM_OC1PreloadConfig(TIM2, TIM_OCPreload_Disable);

	/* TIM IT enable */
	TIM_ITConfig(TIM2, TIM_IT_CC1, ENABLE);

	/* TIM2 enable counter */
	TIM_Cmd(TIM2, ENABLE);
}

void TimerInterrupt_1ms(void) //OLLO CONTROL
{
	if (TIM_GetITStatus(TIM2, TIM_IT_CC1) != RESET) // 1ms//
	{
		TIM_ClearITPendingBit(TIM2, TIM_IT_CC1);

		capture = TIM_GetCapture1(TIM2);
		TIM_SetCompare1(TIM2, capture + CCR1_Val);

		if(gw1msCounter > 0)
			gw1msCounter--;
		ms++;
	}
}

void DisableUSART1(void)
{
	USART_Cmd(USART1, DISABLE);
}

void EnableBT(void)
{
	USART_Configuration(USART_BT, 57600);
	GPIO_ResetBits(PORT_BT_RESET, PIN_BT_RESET);
}

void DisableBT(void)
{
	USART_Cmd(UART5, DISABLE);
	GPIO_SetBits(PORT_BT_RESET, PIN_BT_RESET);
}

void TxDByte_BT(byte bTxdData)
{
	USART_SendData(UART5,bTxdData);
	while( USART_GetFlagStatus(UART5, USART_FLAG_TC)==RESET );
}

byte RxDByte_BT(void)
{
	byte bTemp;

	while(1)
	{
		if(gbPacketReadPointer != gbPacketWritePointer) break;
	}

	bTemp = gbpPacketDataBuffer[gbPacketReadPointer];
	gbPacketReadPointer++;

	return bTemp;
}

void TxDByte_DXL(byte bTxdData)
{
	GPIO_ResetBits(PORT_ENABLE_RXD, PIN_ENABLE_RXD);	// RX Disable
	GPIO_SetBits(PORT_ENABLE_TXD, PIN_ENABLE_TXD);	// TX Enable

	USART_SendData(USART1,bTxdData);
	while( USART_GetFlagStatus(USART1, USART_FLAG_TC)==RESET );

	GPIO_ResetBits(PORT_ENABLE_TXD, PIN_ENABLE_TXD);	// TX Disable
	GPIO_SetBits(PORT_ENABLE_RXD, PIN_ENABLE_RXD);	// RX Enable
}

byte RxDByte_DXL(void)
{
	byte bTemp;

	while(1)
	{
		if(gbRxBufferReadPointer != gbRxBufferWritePointer) break;
	}

	bTemp = gbpRxInterruptBuffer[gbRxBufferReadPointer];
	gbRxBufferReadPointer++;

	return bTemp;
}

void ClearBuffer256(void)
{
	gbRxBufferReadPointer = gbRxBufferWritePointer = 0;
}

void StartDiscount(s32 StartTime)
{
	gw1msCounter = StartTime;
}

u8 CheckTimeOut(void)
{
	// Check timeout
	// Return: 0 is false, 1 is true(timeout occurred)

	if(gw1msCounter == 0)
		return 1;
	else
		return 0;
}


byte CheckNewArrive(void)
{
	if(gbPacketReadPointer != gbPacketWritePointer)
		return 1;
	else
		return 0;
}

void TxDString(byte *bData)
{
	while (*bData)
		TxDByte_PC(*bData++);
}

void TxDWord16(word wSentData)
{
	TxDByte16((wSentData >> 8) & 0xff);
	TxDByte16(wSentData & 0xff);
}

void TxDByte16(byte bSentData)
{
	byte bTmp;

	bTmp = ((byte) (bSentData >> 4) & 0x0f) + (byte) '0';
	if (bTmp > '9')
		bTmp += 7;
	TxDByte_PC(bTmp);
	bTmp = (byte) (bSentData & 0x0f) + (byte) '0';
	if (bTmp > '9')
		bTmp += 7;
	TxDByte_PC(bTmp);
}

void TxDByte_PC(byte bTxdData)
{
	USART_SendData(USART3,bTxdData);
	while( USART_GetFlagStatus(USART3, USART_FLAG_TC)==RESET );
}
