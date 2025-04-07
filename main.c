#include "spi_basic.h"

short sine_LUT[359];
unsigned int i=0;


int  main(){
 	GenerateSineLUT(sine_LUT,359,1000,0);
	while(1){
		DAC_MCP4921_Set_mV(sine_LUT[i]);
		 
		i = (i<360) ? i+1 :0;
	}
}


