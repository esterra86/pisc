#ifndef I2C_BASIC_H
#define I2C_BASIC_H

void I2C_Init(void);
void PCF8574_Write(unsigned char ucData);
__irq void I2C_Interrupt(void);

#endif
