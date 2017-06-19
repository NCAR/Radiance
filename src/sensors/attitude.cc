#include "attitude.h"
#include <bitset>

// Read current from one photodiode
double attitude::ads_read(int pdiode){

	// Initialize message struct and spi device
	struct spi_ioc_transfer spi;
	memset(&spi,0,sizeof(spi));

	// Open spi device
	int spifd;
	spifd = this->spiOpen();

	// Initialize data register
	unsigned char data[2];
	data[0] = 0;
	data[1] = 0;

	// Misc variables and result
	int retVal = -1;
	unsigned short dn;
	double current = 0.0;
	double rf;


	// Construct message
	spi.tx_buf 				= (unsigned long)data;
	spi.rx_buf 				= (unsigned long)data;
	spi.len 				= this->len;
	spi.delay_usecs 			= this->delay;
	spi.speed_hz	 			= this->speed;
	spi.bits_per_word 			= this->bitsPerWord;
	spi.cs_change 				= this->cs;


//	std::cout << sizeof(spi.bits_per_word) << "\n";

	// MESSAGE SEND
		// Set necessary CS pin low to make corresponding ADC listen
		// Also set feedback resistance
		if(pdiode == 1){
			gpioWrite(8, 0);
			rf = this->rf1;
		}
		else if(pdiode == 2){
			gpioWrite(7, 0);
			rf = this->rf2;
		}
		else if(pdiode == 3){
			gpioWrite(20, 0);
			rf = this->rf3;
		}
		else if(pdiode == 4){
			gpioWrite(21, 0);
			rf = this->rf4;
		}


//		std::cout << "spifd = " << spifd << "\n";
//		std::cout << "spi.tx_buf = " << spi.tx_buf << "\n";
//		std::cout << "spi.rx_buf = " << spi.rx_buf << "\n";
//		std::cout << "spi.speed_hz = " << spi.speed_hz << "\n";
//		std::cout << "spi.bits_per_word = " << (int)spi.bits_per_word << "\n";
//		std::cout << "spi.cs_change = " << (int)spi.cs_change << "\n";
//		std::cout << "spi.len = " << spi.len << "\n";

		// Wait 4 ms for conversion
		usleep(4000);

		// Send message
		retVal = ioctl(spifd, SPI_IOC_MESSAGE(1), &spi);

		// Set necessary CS pin high again
		if(pdiode == 1){
			gpioWrite(8, 1);
		}
		else if(pdiode == 2){
			gpioWrite(7, 1);
		}
		else if(pdiode == 3){
			gpioWrite(20, 1);
		}
		else if(pdiode == 4){
			gpioWrite(21, 1);
		}


	if(retVal < 0){
		perror("ERROR - problem transmitting spi message");
		exit(1);
	}

//	std::cout << "RESULT " << std::bitset<8>((int)data[0]) << " " << std::bitset<8>((int)data[1]) << "\n";

	// Get DN
	dn = (data[0] << 8) | data[1];

	current = dn;

	// Close spi device
	retVal = spiClose(spifd);
	if(retVal < 0){
		perror("ERROR - problem closing spi device");
		exit(1);
	}

	return current;

}

// Open spi device
int attitude::spiOpen(){

	int statusVal = -1;
	int spifd;

	// Open spidev device
	spifd = open("/dev/spidev0.0", O_RDWR);
	if(spifd < 0){
		perror("could not open SPI device");
		exit(1);
	}

	// Enable write mode on spidev
	statusVal = ioctl(spifd, SPI_IOC_WR_MODE, &(this->mode));
	if(statusVal < 0){
		perror("Could not set SPIMode (WR) with ioctl");
		exit(1);
	}

	// Enable read mode on spidev
	statusVal = ioctl(spifd, SPI_IOC_RD_MODE, &(this->mode));
	if(statusVal < 0){
		perror("Could not set SPIMode (RD) with ioctl");
		exit(1);
	}

	// Set SPI bits per word (WR)
	statusVal = ioctl(spifd, SPI_IOC_WR_BITS_PER_WORD, &(this->bitsPerWord));
	if(statusVal < 0){
		perror("Could not set SPI bits per word (WR) with ioctl");
		exit(1);
	}

	// Set SPI bits per word (RD)
	statusVal = ioctl(spifd, SPI_IOC_RD_BITS_PER_WORD, &(this->bitsPerWord));
	if(statusVal < 0){
		perror("Could not set SPI bits per word (RD) with ioctl");
		exit(1);
	}

	// Set SPI max write speed
	statusVal = ioctl(spifd, SPI_IOC_WR_MAX_SPEED_HZ, &(this->speed));
	if(statusVal < 0){
		perror("Could not set SPI max write speed with ioctl");
		exit(1);
	}

	// Set SPI max read speed
	statusVal = ioctl(spifd, SPI_IOC_RD_MAX_SPEED_HZ, &(this->speed));
	if(statusVal < 0){
		perror("Could not set SPI max read speed with ioctl");
		exit(1);
	}

	return spifd;

}


// Close spi device
int attitude::spiClose(int spifd){

	int statusVal = -1;

	statusVal = close(spifd);

	if (statusVal < 0){
		perror("Could not close spi device");
		exit(1);
	}

	return statusVal;

}
