# Protocols for Inter-System Communication

This repository contains a set of functions written for the **LPC21xx** microcontroller to support the following communication protocols:

- **CAN**
- **SPI**
- **I2C**

The implementations for each protocol can be found in their respective branches.

---

## SPI

On this board, SPI is used to communicate with the **MCP4921 DAC**.

- `DAC_MCP4921_Set`: Initializes the SPI module and sends 2 bytes to the DAC inputs.  
- `DAC_MCP4921_Set_mV`: Accepts a value in millivolts (mV) to be set on the DAC output. It converts the input voltage to the appropriate value for the DAC and passes it to `DAC_MCP4921_Set`.

---

## I2C

I2C is used to communicate with the **PCF8574 I/O Expander**.

- `I2C_Interrupt`: Interrupt handler for the I2C module.  
- `I2C_Init`: Initializes the I2C module and its interrupt configuration.  
- `PCF8574_Write`: Sets the data to be transmitted over I2C and sets the flag to initiate the transmission.

---

## CAN

- `Can1_InitAsTx`: Initializes **CAN1** as a transmitter.  
- `Can2_InitAsRx`: Initializes **CAN2** as a receiver.  
- `ucCan1_TxFree`: Checks the status of the transmit buffer.  
- `ucCan2_RxReady`: Checks if the receive buffer has received any data.  
- `Can1_SendByte`: Sends a byte to the transmit buffer and sets the **transmit request** flag.  
- `ucCan2_ReceiveByte`: Reads a byte from the receive buffer and sets a flag to release the buffer.
