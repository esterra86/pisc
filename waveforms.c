#include "waveforms.h"




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
