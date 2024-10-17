#include "waveforms.h"
#include  <cmath>



void  GenerateSineTable(short *sine_table, unsigned int points, unsigned int ampl_mv )
{
	unsigned int i;
	float  rad;
	for(i =0;  i < points; i++)
	{
			rad = (float)i*2* M_PI / points;
		  sine_table[i] = (sinf(rad) + 1.0f)*ampl_mv;
	}
};
