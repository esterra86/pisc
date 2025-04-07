#include <LPC21XX.H>
#include "can.h"

//P0.25 RD1
//P0.23 RD2
//P0.24 TD2

#define LED0 (1<<16) 				
#define LED1 (1<<17) 			
#define LED2 (1<<18)
#define LED3 (1<<19)	
#define LED4 (1<<20) 				
#define LED5 (1<<21) 			
#define LED6 (1<<22)
#define LED7 (1<<23)	



//CANMOD
#define CAN2_RD (1 << 14)
#define CAN2_TD (1 << 16)
#define CAN1_RD (1 << 18)

#define MODE_RESET 									(1 << 0)
#define AFMR_AccBP									(1 << 1)
#define CMR_RELEASE_RECEIVE_BUFFER  (1 << 2)
#define CMR_TRANSMISSION_REQUEST 		(1 << 0)
#define SR_RECEIVED_MSG_AVAILABLE   (1 << 0)
#define SR_TX_BUFFER_FREE						(1 << 2)

#define BTR_BRP  										0x1D
#define BTR_TSEG1 									(0xC<<16)
#define BTR_TSEG2 									(0x1<<20)
#define DLC				1
#define DLC_bp  16
#define DLC_bm										0x00010000


void Can1_InitAsTx(void){
	PINSEL1 |= CAN1_RD;
	C1MOD 	|= MODE_RESET; 			
	C1BTR 	|= BTR_BRP|BTR_TSEG1|BTR_TSEG2;
	C1TFI1	&= ~DLC_bm;
	C1TFI1	|= DLC<<DLC_bp;				
	C1MOD 	&= ~MODE_RESET;
}

void Can2_InitAsRx(void){
	PINSEL1 |= CAN2_RD|CAN2_TD;
	C2MOD 	|= MODE_RESET;		
	AFMR  	|= AFMR_AccBP; 	
	C2BTR 	|= BTR_BRP|BTR_TSEG1|BTR_TSEG2;
	C2CMR 	|= CMR_RELEASE_RECEIVE_BUFFER; 
	C2MOD 	&= ~MODE_RESET;								
}

void Can1_SendByte(unsigned char ucByte){
	C1TDA1 = ucByte;									
	C1CMR = CMR_TRANSMISSION_REQUEST; 
}                                   
                                    
unsigned char ucCan1_TxFree(void){  
	return (C1SR & SR_TX_BUFFER_FREE);
};

unsigned char ucCan2_RxReady(void){
	return (C2SR & SR_RECEIVED_MSG_AVAILABLE);
};

unsigned char ucCan2_ReceiveByte(void){
	unsigned char ucResult;
	
	ucResult	=  C2RDA;									
	C2CMR =  CMR_RELEASE_RECEIVE_BUFFER; 
	return ucResult;
}

void Led_Init(void){	
	IO1DIR |= LED0|LED1|LED2|LED3|LED4|LED5|LED6|LED7;
};


void Led_ShowByte(unsigned char ucReceivedByte)
{
	IO1CLR = ~(ucReceivedByte<< 16);
	IO1SET = (ucReceivedByte << 16);
};
