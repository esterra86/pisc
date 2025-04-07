#include "i2c_basic.h"


unsigned char iMainLoopCtr=5;

void Delay(int a){ 
	int iCount;
	int time = a*12000;
	for(iCount =0 ; iCount<time; iCount++){
	};   
}

int  main(){
	I2C_Init();
	while(1){
		PCF8574_Write(iMainLoopCtr);
		iMainLoopCtr = (iMainLoopCtr+1)%256;
		Delay(100);
		}
}


