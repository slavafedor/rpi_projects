#include <stdio.h>
#include <string.h>
#include <iostream>
#include <errno.h>
#include <signal.h>
#include <unistd.h>

#include <wiringPiSPI.h>
#include <rpi_spi_driver.h>

using namespace std;

#define SPI_SPEED SPI_SPEED_500K

// channel is the wiringPi name for the chip select (or chip enable) pin.
// Set this to 0 or 1, depending on how it's connected.
static const int CHANNEL = SPI_CHANN;


void my_handler(int s)
{
	printf("Caught signal %d\n",s);
	exit(1); 
}

void init_signal()
{
	struct sigaction sigIntHandler;

	sigIntHandler.sa_handler = my_handler;
	sigemptyset(&sigIntHandler.sa_mask);
	sigIntHandler.sa_flags = 0;
	sigaction(SIGINT, &sigIntHandler, NULL);
}

int main()
{
	int i = 0;
	
	init_signal();

	cout << "spi_console" << endl;
	cout << "  channel : " << SPI_CHANN << endl;
	cout << "  mode    : " << SPI_MODE << endl;
	cout << "  speed   : " << SPI_SPEED << endl;

	int result;
	unsigned char buffer[100];

	memset(buffer, 0, 100);

	cout << "Initializing" << endl ;

	// Configure the interface.
	// CHANNEL insicates chip select,
	// 500000 indicates bus speed.
/*	
	int fd;
	fd = wiringPiSPISetupMode(CHANNEL, SPI_SPEED, SPI_MODE);

	cout << "Init result: " << fd << endl;


	while(i < 10)
	{
		++i &= 0xff;
		buffer[0] = i;
		result = wiringPiSPIDataRW(CHANNEL, buffer, 2);
		if(result)
		{
			printf("0x%x %d %s\n", result, i, buffer);
		}
		sleep(1);
	}
*/
	rpi_spi_driver rp_spi;

	rp_spi.OpenPort(SPI_DEVICE::CS1 /*,SPI_SPEED_500K, SPI_MODE_0, 8*/);
	rp_spi.PrintSetup();

	while(1)
	{
		++i &= 0xff;
		buffer[0] = i;
		result = rp_spi.WriteAndRead(buffer, &buffer[5], 2, 1);
		if(result)
		{
			printf("0x%x %d %s %s\n", result, i, buffer, &buffer[5]);
		}
		sleep(1);
	}

	rp_spi.ClosePort();

	return 0;
}