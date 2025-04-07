/*
halfduplex,  master-slave
SCL -  serial  clock line
SDA - serial  data line
nie ma minimalnej  predkosci,  max: 100kb/s(standard), 400kb/s(szybki),  3.4Mb/s(High speed)
'0' stan  dominujacyl, '1' stan recesywny
bit start | data | bit stop

start, stop -  tylko master
dane - master i slave
paczki  po 8bitów
Pierwszy  bajt  po  START  zawsze  master
0-6:  adres lave'a 8: bit  kierunkowy
bit  kierunkowy: 0-z  mastera, 1-z slave'a
je[li dane  od  slav'e a to ack(ostatni  bit) ='1'

SCL0:P0.2
SDA0:P0.3
*/

/*
I2C block may operate as a master, a slave, or both. 
slave mode -- the I2C hardware looks for its own slave address and the general call address. --  if  it  finds  one  of  them --> interrupt request
Gdy  procesor chce  zostac masterem -->  hardware  hardware waits until the bus is free to enter  master  mode
If bus arbitration is lost in the master mode --> block switches to the slave mode immediately and can detect its own slave address in the same serial transfer.

Master TRasmitter  mode:	(set I2EN,  clear SI) R/W:0
		enter  mode when   the STA bit sets
		--> START condition is send --> SI bit is set (status code in I2STAT is 0x08)
		status code is used to vector to a state service routine (ssr) 
		ssr  -->loads the slave address and W(rite) bit to the I2DAT --> clear the SI bit. 
		The STA bit should be cleared after  writing the slave address.
		
		(slave adress, R/W bit -- transmitted)&(ACK -- received) --> SI is set again
		--> possible status codes: master mode ( 0x18, 0x20, or 0x38 ), slave mode (0x68, 0x78, or 0xB0)

Master Receiver mode
	--> START condition is send --> ssr  -->loads the slave address and W/R bit to the I2DAT --> clear the SI bit.
	-->(slave adress, R/W bit -- transmitted)&(ACK -- received) --> SI is set again
	--> Status Register will show status code
	possible status codes: master mode ( 0x40, 0x48, or 0x38 ), slave mode (0x68, 0x78, or 0xB0)
	
Slave Receiver mode
	init: write to I2ADR, write I2CONSET ob?10001??
	--> it waits until it is addressed --> mode = (W/R) ? Receiver : Transmitter
	(slave address, R/W -- received) --> , the SI bit is set, valid status code can be read from I2STAT
	
Slave Transmitter mode
	The first byte is received and handled as in the slave receiver mode
	Serial data is transmitted via SDA while the serial clock is input through SCL
	waits for being addressed --> gis addressed --> interrupt is requested
	(When the microcontrollers wishes to become the bus master, the
	hardware waits until the bus is free before the master mode is entered so that a possible
	slave action is not interrupted)
	(bus arbitration is lost --> interface switches to the slave mode immediately 
	and can detect its own slave address in the same serial transfer.)
*/
	
/*
	INPUT FILTER: Input signals are synchronized with the internal clock, and spikes shorter than three
	clocks are filtered out.
	
	ADDRES REG - I2ADDR: LSB(enable general call adres recognition) 1-7: address bits
	COMPARATOR: 7 bit slave address with its own slave address(I2ADDR)
	SHIFT REG -I2DAT: contains a byte of serial data to be transmitted or a byte which has just been received
										 always shifted from right to left
										 the first bit to be transmitted is the MSB (bit 7)
										 the first bit of received data is located at the MSB of I2DAT
										 While data is being shifted out, data on the bus is simultaneously shifted in;
	Serial Clock Generator:
						provides clock pulses when I2C block is in the master  mode
						output clock frequency and duty cycle is programmable via the I2C Clock Control Registers
	 Control register I2CONSET and I2CONCLR:
					used to set and clear bits in control register
					I2C control register contains bits used to control 
						I2C block functions: start, restart of a serial transfer,
						termination of a serial transfer, bit rate, address recognition, acknowledgment.
						
	Status Decoder:	
		takes all of the internal status bits and compresses them into a 5 bit code
		code is unique for each I2C bus status
		this code may be used to generate vector addresses for fast processing of the various service routines.
		Each service routine processes a particular bus status. 
		
	Status Register:
		The 5 bit status code is latched into the 5 MSBs of the status reg
		when the SI(serial interrupt) flag is set (by hardware) and remains stable until
		SI flag is cleared (by software).
		3 LSBs ar always zero
		o. If the status code is used as a vector to service routines, then the routines are displaced by eight address locations
*/


/*
I2CONSET   -- control setting of bits in the I2CON register (that controls operation of the I2C interface).
						7:x 6:I2EN 5:STA 4:STO 3:SI 2:AA 1-0:x



*/
#include "i2c_basic.h"
#include <LPC213X.H>


#define  VIC_I2C0_CHANNEL_NR 9
#define mIRQ_SLOT_ENABLE 0x00000020


#define SDA0_PIN				0x40
#define SCL0_PIN				0x10


#define SLAVE_ADDR			0x70 //PCF8574A:  0111 A2 A1 A0, PCF8574:0100  A2 A1 A0

#define WRITE_BIT				0x00
#define READ_BIT 				0x01

unsigned char ucDataGlobal;

__irq void I2C_Interrupt(void)
{
	switch (I2C0STAT)   // Read result code and switch to next action
    {
        case (0x08):  //A START condition has been transmitted.
            I2C0CONCLR = 0x20;   // Clear start bit
            I2C0DAT = SLAVE_ADDR | WRITE_BIT;  // Send address and write bit
            break;

        case (0x18):  // SLA+W has been transmitted; ACK has been received.
            I2C0DAT =ucDataGlobal;    // Data byte will be transmitted; ACK bit will be received.
            break;

       // case (0x20):  //  SLA+W has been  transmitted; NOT ACK has been received.
       //     I2C0DAT = SLAVE_ADDR | WRITE_BIT;  //Data byte will be transmitted; ACK bit will be received.
       //     break;

       // case (0x28):  // Data byte in I2DAT has been transmitted;ACK has been received.
       //     I2C0CONSET = 0x10;    // STOP condition will be transmitted; STO flag will be reset.
       //     break;

        default:
            break;
    }
	//I2C0CONCLR=0x08;
	VICVectAddr = 0;
};


void I2C_Init(void)
{
	VICIntEnable |= (0x1 << VIC_I2C0_CHANNEL_NR);
	VICVectCntl11 = mIRQ_SLOT_ENABLE | VIC_I2C0_CHANNEL_NR;
	VICVectAddr11 = (unsigned long)I2C_Interrupt;
	
	
	PINSEL0 = PINSEL0 | SDA0_PIN | SCL0_PIN;
	
	I2C0CONCLR=0x6c; //clearing flags
	I2C0CONSET=0x40; //enabling I2C (0x40),  start(0x20)
	
	I2C0SCLH=0x80;
	I2C0SCLL =0x80;
	
	
	
};
void PCF8574_Write(unsigned char ucData)
{
		I2C0CONSET = 0x20;
		ucDataGlobal = ucData;
	
};
