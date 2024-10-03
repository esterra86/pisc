#include <LPC213X.H>
void DAC_MCP4921_Set(unsigned int uiVoltage);

#define P04_SCK0 			(1<<8)
#define P05_SDO	 			(1<<10)	//MIS0
#define P06_SDI	 			(1<<12)	//M0SI
#define P010_CS  			(1<<10)				//Chip select
#define CONFIG_BITS  	0xc00
#define VREF 					5
#define SPIF 					(1<<7)



int  main(){
	PINSEL0 = PINSEL0 | P04_SCK0 | P05_SDO | P06_SDI;

	IODIR0 = IODIR0 | P010_CS;
	

	S0SPCR |= 0x20;
	S0SPCCR = 0x08;
	
	
	while(1){
		IOCLR0 = IOCLR0 |  P010_CS;
		S0SPDR = 0xcf;
		while(!(S0SPSR & SPIF)){} 
		S0SPDR = 0xff;
		while(!(S0SPSR & SPIF)){}
			
		IOSET0 = IOSET0 |  P010_CS;
	}
}
//00101010

void DAC_MCP4921_Set(unsigned int uiVoltage) {
	
	//unsigned char data_to_send;
	//volatile unsigned char status;
	//volatile unsigned char dummy ;
	uiVoltage =  (uiVoltage << 12)/5;
	
	PINSEL0 = PINSEL0 | P04_SCK0 | P05_SDO | P06_SDI;

	IODIR0 = IODIR0 | P010_CS;
	

	S0SPCR |= 0x0030;
	S0SPCCR = 0x08;
	
	IOCLR0 = IOCLR0 |  P010_CS;
	
	
	//data_to_send = (CONFIG_BITS | ((uiVoltage >> 8) & 0x0F));
	S0SPDR = 0xcfff;
	
	
	while(!(S0SPSR & SPIF)){} 
	//status = S0SPSR;
	//dummy = S0SPDR;
	//data_to_send = uiVoltage & 0xFF;
	S0SPDR =0xf0ff;
		
	
	while(!(S0SPSR & SPIF)){} 
	//status = S0SPSR;
	//dummy = S0SPDR;
	
	IOSET0 |= P010_CS;
}

