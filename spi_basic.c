#include "spi_basic.h"
#include  <cmath>

tspcr my_spcr; 
void DAC_MCP4921_Set(unsigned int uiVoltage) {
	
	PINSEL0 |= PINSEL0 | PIN_SCK0 | PIN_SDO | PIN_SDI;

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


void  GenerateSineLUT(short *sine_table, unsigned int points, unsigned int ampl_mv, unsigned int dc_offset )
{
	unsigned int i;
	float  rad;
	for(i =0;  i < points; i++)
	{
			rad = (float)i*2* M_PI / points + dc_offset;
		  sine_table[i] = (sinf(rad) + 1.0f)*ampl_mv;
		//sine_table[i] = sin(rad)*ampl_mv;
	}
};
