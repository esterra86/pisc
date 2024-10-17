#include "spi_basic.h"
#include "waveforms.h"

short sine_table[359];
unsigned int i=0;


int  main(){
 	GenerateSineTable(sine_table,360,1000);
	while(1){
		DAC_MCP4921_Set_mV(sine_table[i]);
		 
		i = (i<360) ? i+1 :0;
	}
}


