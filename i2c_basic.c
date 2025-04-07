#include "i2c_basic.h"
#include <LPC213X.H>


#define  VIC_I2C0_CHANNEL_NR 9
#define mIRQ_SLOT_ENABLE 0x00000020


#define SDA0_PIN					0x40
#define SCL0_PIN					0x10
	
	
#define SLAVE_ADDR				0x40	//PCF8574A:  0111 A2 A1 A0, PCF8574:0100  A2 A1 A0
	
#define READ_BIT 					(1<<0)
	
#define CLEAR_ALL_FLAGS		0x6c
#define EN_I2C						(1<<6)
	
#define SCL_HIGH_TIME			0x50
#define SCL_LOW_TIME			0x50
	
#define I2C_START					(1<<5)
#define I2C_STOP					(1<<4)
#define I2C_CLEAR_INTERRUPT (1<<3)

#define START_CONDITION    0x08
#define SLA_TRANSMITTED_ACK_RECEIVED			 0x18
#define DATA_TRANSMITTED_ACK_RECEIVED			 0x28
	


unsigned char ucDataGlobal;

__irq void I2C_Interrupt(void)
{
	switch (I2C0STAT)
    {
        case (START_CONDITION):  //A START condition has been transmitted.
            I2C0CONCLR = I2C_START;  
            I2C0DAT = SLAVE_ADDR; 
            break;

        case (SLA_TRANSMITTED_ACK_RECEIVED):  // SLA+W has been transmitted; ACK has been received.
            I2C0DAT =ucDataGlobal;    
            break;

				case (DATA_TRANSMITTED_ACK_RECEIVED): // Data sent, Ack
				I2C0CONSET = I2C_STOP; // Stop condition
				break;
        default:
            break;
    }
	I2C0CONCLR=I2C_CLEAR_INTERRUPT;
	VICVectAddr = 0;
};


void I2C_Init(void)
{
	VICIntEnable |= (0x1 << VIC_I2C0_CHANNEL_NR);
	VICVectCntl11 = mIRQ_SLOT_ENABLE | VIC_I2C0_CHANNEL_NR;
	VICVectAddr11 = (unsigned long)I2C_Interrupt;
	
	
	PINSEL0 = PINSEL0 | SDA0_PIN | SCL0_PIN;
	
	I2C0CONCLR=CLEAR_ALL_FLAGS;
	I2C0CONSET=EN_I2C; 
	I2C0SCLH=SCL_HIGH_TIME;
	I2C0SCLL =SCL_LOW_TIME;
	
	
	
};
void PCF8574_Write(unsigned char ucData)
{
		ucDataGlobal = ucData;
		I2C0CONSET = I2C_START;
	
};
