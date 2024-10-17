#include "spi_basic.h"

tspcr my_spcr; //‘extern’ keyword is used for a variable when we declare the variable in one file and define it in another file.
							//the extern keyword is used to extend the visibility of variables/functions.
void DAC_MCP4921_Set(unsigned int uiVoltage) {
	
	PINSEL0 = PINSEL0 | PIN_SCK0 | PIN_SDO | PIN_SDI;

	IODIR0 = IODIR0 | PIN_CS;
	
	
	my_spcr.Bity.BIT_EN=1;
	my_spcr.Bity.CPHA=0;
	my_spcr.Bity.CPOL=0;
	my_spcr.Bity.MSTR=1;
	my_spcr.Bity.LSBF=0;
	

	S0SPCR = my_spcr.full_reg;
	S0SPCCR = 0x08;

	
	IOCLR0 = IOCLR0 |  PIN_CS;
	
	S0SPDR =  CONFIG_BITS|uiVoltage;
	while(!(S0SPSR & SPIF)){}  

	IOSET0 |= PIN_CS;
		
}

void DAC_MCP4921_Set_mV(unsigned int uiVoltage) {
	uiVoltage=uiVoltage*4095/3000;
	DAC_MCP4921_Set(uiVoltage);
}
