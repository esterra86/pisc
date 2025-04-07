/*
CPHA - clock phase - determines when data is sampled relative to the clock edge.
	the rising or falling clock edge is used to sample and/or shift the data
		= 0: Data is sampled on the rising edge of the clock.
		= 1: Data is sampled on the falling edge of the clock.
CPOL - Clock Polarity) defines the idle state of the clock line
	idle state is defined as the period when CS is high and transitioning to low
	at the start of the transmission and when CS is low and transitioning to high at the end of the transmission.
		= 1: Clock is low when idle.
		= 0: Clock is high when idle.                                                                                                                                                                                                                                                                 
MSTR -  master  slave  select -  configures the device as either a master or a slave in SPI communication.
		= 1: Configures the device as the master.
		= 0: Configures the device as the slave.
LSBF -  LSB  first
		= 0: (MSB) first.
		= 1:  (LSB) first.
		
SPCR - SPI Control Register
SPSR - SPI Status Register
SPDR - SPI Data Register
SPCCR - SPI Clock Counter Register. This register controls the
frequency of a master’s SCK.
*/

/*
SCK0:P0.4
SDO: P0.5
SDI: P0.6

CS - chip select requires an active-low signal to enable serial clock and data functions - select subnode
*/

#include <LPC213X.H>
void DAC_MCP4921_Set(unsigned int uiVoltage);

#define P04_SCK0 			0x004
#define P05_SDO	 			0x001	//MIS0
#define P06_SDI	 			0x0004	//M0SI
#define P010_CS  			(1<<(10-1))				//Chip select
#define CONFIG_BITS  	0b0011
#define VREF 					5
#define SPIF 					(1<<7)


int  main(){
	
	while(1){DAC_MCP4921_Set(2);}
}

void DAC_MCP4921_Set(unsigned int uiVoltage) {
	
	unsigned char data_to_send;
	uiVoltage =  (uiVoltage << 13)/5;
	//---------inicjalizacja--------
	//ustawic funkcje odpowiednich pinów na SPI (patrz Pin Connect Block),
	PINSEL0 = PINSEL0 | P04_SCK0 | P05_SDO | P06_SDI;
	//ustawic pin P0.10 (uzywany dalej jako Chip Select, CS) na wyjsciowy,
	IO0DIR = IO0DIR | P010_CS;
	
	//ustawic tryb pracy SPI0 na „master”, bity cpha i cpol odpowiednio do wspólpracy z przetwornikiem
	//CPOL = 0 (low when idle) CPHA=0 
	//sampled on the rising edge shifted at hte faaling edge
	//LSBF=0,MSTR=1,no spi interrupts?
	S0SPCR |= 0x0c0;
	//ustawic czestotliwosc zegara SPI0 na 1/8 czestotliwosci taktowania peryferiów (pclk)
	S0SPCCR = 0x8;
	
	//-------transmisja---------------
	//ustawic odpowiednia wartosc na linii CS
	IO0CLR |= P010_CS;
	
	// 15  | 14  | 13 | 12   | 11:0
	// A/B | BUF | GA | SHDN | D11:D0
	//zainicjowac wyslanie starszego bajtu sterowania
	data_to_send = (CONFIG_BITS<<12) | ((uiVoltage >> 8) & 0x0F);
	S0SPDR =data_to_send;
	
	//poczekac na zakonczanie transmisji,
	while(!(S0SPSR & SPIF)){} 
		
	//zainicjowac wyslanie mlodszego bajtu sterowania,
	data_to_send = uiVoltage & 0xFF;
	S0SPDR =data_to_send;
		
	//poczekac na zakonczanie transmisji,
	while(!(S0SPSR & SPIF)){} 
	//ustawic odpowiednia wartosc na linii CS.
	IOSET0 |= P010_CS;
}
