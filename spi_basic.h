#ifndef SPI_BASIC_H
#define SPI_BASIC_H

#include <LPC213X.H>

#define PIN_SCK0 			(1<<8)  //P04
#define PIN_SDO	 			(1<<10)	//P05
#define PIN_SDI	 			(1<<12)	//P06
#define PIN_CS  			(1<<10)			//P010	//Chip select
#define CONFIG_BITS  	0x3000
#define VREF 					5
#define SPIF 					(1<<7)

typedef struct SPCR_FLAGS{
    unsigned 		 		: 2;
    unsigned BIT_EN : 1;
    unsigned CPHA 	: 1;
    unsigned CPOL 	: 1;
    unsigned MSTR 	: 1;
    unsigned LSBF 	: 1;
    unsigned SPIE		: 1;
    unsigned NR_OF_TRANSFER_BITS : 4;
}tspcr_flags;

typedef union SPCR{
    short full_reg;
    tspcr_flags Bity;
}tspcr;



void DAC_MCP4921_Set(unsigned int uiVoltage);
void DAC_MCP4921_Set_mV(unsigned int uiVoltage);

#endif
