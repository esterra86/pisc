void Can1_InitAsTx(void);
void Can2_InitAsRx(void);
void Can1_SendByte(unsigned char ucByte);
unsigned char ucCan1_TxFree(void);
unsigned char ucCan2_RxReady(void);
unsigned char ucCan2_ReceiveByte(void); 
void Led_Init(void);
void Led_ShowByte(unsigned char ucReceivedByte);
