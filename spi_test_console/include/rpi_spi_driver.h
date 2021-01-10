#ifndef _SPI_CONSOLE_H_
#define _SPI_CONSOLE_H_

#include <stdint.h>
#include <linux/spi/spidev.h>	// Needed for SPI port

#define SPI_SPEED_500K	500000 
#define SPI_SPEED_1M 	1000000
#define SPI_SPEED_5M 	5000000
#define SPI_SPEED_10M 	10000000
#define SPI_CHANN		1
#define SPI_MODE		SPI_MODE_0
#define SPI_BITS_WORD	8

typedef enum _SPI_DEVICE
{
	CS0 = 0, 
	CS1 = 1
} SPI_DEVICE;

class rpi_spi_driver 
{
private:
	SPI_DEVICE spi_device;
	int cs_fd;					// file descriptor for the SPI device
	uint8_t mode;
	uint8_t bitsPerWord;
	unsigned int speed;

public:
	//----- SPI MODE -----
	// SPI_MODE_0 (0,0) 	CPOL = 0, CPHA = 0, Clock idle low, data is clocked in on rising edge, output data (change) on falling edge
	// SPI_MODE_1 (0,1) 	CPOL = 0, CPHA = 1, Clock idle low, data is clocked in on falling edge, output data (change) on rising edge
	// SPI_MODE_2 (1,0) 	CPOL = 1, CPHA = 0, Clock idle high, data is clocked in on falling edge, output data (change) on rising edge
	// SPI_MODE_3 (1,1) 	CPOL = 1, CPHA = 1, Clock idle high, data is clocked in on rising, edge output data (change) on falling edge

	int OpenPort(SPI_DEVICE spi_device, 
		unsigned int spi_speed = SPI_SPEED_1M, 
		uint8_t mode = SPI_MODE_0, 
		uint8_t bitsPerWord = SPI_BITS_WORD);

	int ClosePort();

	//********** SPI WRITE & READ DATA **********
	// TxData and RxData can be the same buffer (read of each byte occurs before write)
	// Length		Max 511 (a C SPI limitation it seems)
	// LeaveCsLow	1=Do not return CS high at end of transfer (you will be making a further call to transfer more data), 0=Set CS high when done
	int WriteAndRead(unsigned char *TxData, unsigned char *RxData, unsigned int Length, uint8_t LeaveCsLow = 0);

	void PrintSetup();
};


#endif