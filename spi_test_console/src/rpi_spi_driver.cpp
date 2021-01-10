#include <fcntl.h>				//Needed for SPI port
#include <sys/ioctl.h>			//Needed for SPI port
#include <linux/spi/spidev.h>	//Needed for SPI port
#include <unistd.h>			//Needed for SPI port
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <iostream>
#include <unistd.h>
#include <cstring>

#include <rpi_spi_driver.h>

using namespace std;

void rpi_spi_driver::PrintSetup()
{
	cout << "rpi_spi_driver:" << endl;
	cout << "  device  : " << this->spi_device << endl;
	cout << "  mode    : " << (int)this->mode << endl;
	cout << "  speed   : " << this->speed << endl;
	cout << "  bits/w  : " << (int)this->bitsPerWord << endl;
	cout << "  cs_fd   : " << this->cs_fd << endl << endl;
}

//********** SPI OPEN PORT **********
//spi_device	0=CS0, 1=CS1
int rpi_spi_driver::OpenPort(SPI_DEVICE spi_device, 
	unsigned int spi_speed,
	uint8_t mode, 
	uint8_t bitsPerWord)
{
	int status_value = -1;

	this->spi_device = spi_device;
	this->mode = mode;
    this->bitsPerWord = bitsPerWord;
    this->speed = spi_speed;		//default: 1000000 = 1MHz (1uS per bit) 


    if (spi_device == SPI_DEVICE::CS1)
    {
    	this->cs_fd = open("/dev/spidev0.1", O_RDWR);
    }
    else{
    	this->cs_fd = open("/dev/spidev0.0", O_RDWR);
    }

    if (this->cs_fd < 0)
    {
        perror("Error - Could not open SPI device");
        exit(1);
    }

    status_value = ioctl(this->cs_fd, SPI_IOC_WR_MODE, &this->mode);
    if(status_value < 0)
    {
        perror("Could not set SPIMode (WR)...ioctl fail");
        exit(1);
    }

    status_value = ioctl(this->cs_fd, SPI_IOC_RD_MODE, &this->mode);
    if(status_value < 0)
    {
      perror("Could not set SPIMode (RD)...ioctl fail");
      exit(1);
    }

    status_value = ioctl(this->cs_fd, SPI_IOC_WR_BITS_PER_WORD, &this->bitsPerWord);
    if(status_value < 0)
    {
      perror("Could not set SPI bitsPerWord (WR)...ioctl fail");
      exit(1);
    }

    status_value = ioctl(this->cs_fd, SPI_IOC_RD_BITS_PER_WORD, &this->bitsPerWord);
    if(status_value < 0)
    {
      perror("Could not set SPI bitsPerWord(RD)...ioctl fail");
      exit(1);
    }

    status_value = ioctl(this->cs_fd, SPI_IOC_WR_MAX_SPEED_HZ, &this->speed);
    if(status_value < 0)
    {
      perror("Could not set SPI speed (WR)...ioctl fail");
      exit(1);
    }

    status_value = ioctl(this->cs_fd, SPI_IOC_RD_MAX_SPEED_HZ, &this->speed);
    if(status_value < 0)
    {
      perror("Could not set SPI speed (RD)...ioctl fail");
      exit(1);
    }
    return(status_value);
}



//********** SPI CLOSE PORT **********
int rpi_spi_driver::ClosePort()
{
	int status_value = -1;

    status_value = close(this->cs_fd);
    if(status_value < 0)
    {
    	perror("Error - Could not close SPI device");
    	exit(1);
    }
    return status_value;
}



//********** SPI WRITE & READ DATA **********
//TxData and RxData can be the same buffer (read of each byte occurs before write)
//Length		Max 511 (a C SPI limitation it seems)
//LeaveCsLow	1=Do not return CS high at end of transfer (you will be making a further call to transfer more data), 0=Set CS high when done
int rpi_spi_driver::WriteAndRead(unsigned char *TxData, unsigned char *RxData, unsigned int Length, uint8_t LeaveCsLow)
{
	struct spi_ioc_transfer spi;
	int status_value = -1;

	spi.tx_buf = (unsigned long)TxData;		//transmit from "data"
	spi.rx_buf = (unsigned long)RxData;		//receive into "data"
	spi.len = Length;
	spi.delay_usecs = 0;
	spi.speed_hz = this->speed;
	spi.bits_per_word = this->bitsPerWord;
	spi.cs_change = LeaveCsLow;						//0=Set CS high after a transfer, 1=leave CS set low
	spi.pad = 0;
	spi.rx_nbits = this->bitsPerWord;
	spi.tx_nbits = this->bitsPerWord;

	status_value = ioctl(this->cs_fd, SPI_IOC_MESSAGE(1), &spi);

	if(status_value < 0)
	{
		perror("Error - Problem transmitting spi data..ioctl");
		//exit(1);
	}

	return status_value;
}