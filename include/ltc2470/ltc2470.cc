#include "ltc2470.h"

//spiOpen() - called by constructor. Opens spidev device and sets up interface
// Not called by user.
int ltc2470::spiOpen(std::string devspi){
	int statusVal = -1;
	// Open spidev device
	this->spifd = open(devspi.c_str(), O_RDWR);
	if(this->spifd < 0){
		perror("could not open SPI device");
		exit(1);
	}
	// Enable write mode on spidev
	statusVal = ioctl(this->spifd, SPI_IOC_WR_MODE, &(this->mode));
	if(statusVal < 0){
		perror("Could not set SPIMode (WR) with ioctl");
		exit(1);
	}
	// Enable read mode on spidev
	statusVal = ioctl(this->spifd, SPI_IOC_RD_MODE, &(this->mode));
	if(statusVal < 0){
		perror("Could not set SPIMode (RD) with ioctl");
		exit(1);
	}
	// Set SPI bits per word (WR)
	statusVal = ioctl(this->spifd, SPI_IOC_WR_BITS_PER_WORD, &(this->bitsPerWord));
	if(statusVal < 0){
		perror("Could not set SPI bits per word (WR) with ioctl");
		exit(1);
	}
	// Set SPI bits per word (RD)
	statusVal = ioctl(this->spifd, SPI_IOC_RD_BITS_PER_WORD, &(this->bitsPerWord));
	if(statusVal < 0){
		perror("Could not set SPI bits per word (RD) with ioctl");
		exit(1);
	}
	// Set SPI max write speed
	statusVal = ioctl(this->spifd, SPI_IOC_WR_MAX_SPEED_HZ, &(this->speed));
	if(statusVal < 0){
		perror("Could not set SPI max write speed with ioctl");
		exit(1);
	}
	// Set SPI max read speed
	statusVal = ioctl(this->spifd, SPI_IOC_RD_MAX_SPEED_HZ, &(this->speed));
	if(statusVal < 0){
		perror("Could not set SPI max read speed with ioctl");
		exit(1);
	}
	return statusVal;
}
//spiClose() - called by destructor. Self-explanatory.
// Not called by user.
int ltc2470::spiClose(){
	int statusVal = -1;
	// Close file
	statusVal = close(this->spifd);
	if(statusVal < 0){
		perror("Could not close SPI device");
		exit(1);
	}
	return statusVal;
}
//spiWriteRead() - function to write data to SPI device and read shifted data
// No distinction in software between reading and writing. For ADS, write
// control data and junk to shift measurement to SPI register
// INPUTS - data: pointer to data array of desired size
//			length: number of words in data array
int ltc2470::spiWriteRead(unsigned char *data, int length){
		// Struct to define a single transfer. See spidev.h
		struct spi_ioc_transfer spi[length];
		int i = 0;
		int retVal = -1;

		// One SPI transfer per byte (defined by length)
		for (i = 0; i < length; i++) {
			//Hacky fix for ioctl:invalid argument error
    		//See http://www.raspberry-projects.com/pi/programming-in-c/spi/using-the-spi-interface
    		memset(&spi[i],0,sizeof(spi[i]));

    		// Set value in "spi" structure created above
    		// tx_buf and rx_buf are pointers to buffers for data
    		// (data+i) allows for multi-byte transfers
    		spi[i].tx_buf 			= (unsigned long)(data + i);
    		spi[i].rx_buf			= (unsigned long)(data + i);
    		// Length of tx and rx buffers, bytes
    		// Should be constant at 1 byte (for char array *data)
    		spi[i].len 				= sizeof(*(data + i));
    		// Delay after transmission to deselect device
    		spi[i].delay_usecs 		= 0;
    		// Override device's bitrate (temporary, be careful)
    		spi[i].speed_hz 		= this->speed;
    		// Override device's word size (temporary)
    		spi[i].bits_per_word 	= this->bitsPerWord;
    		// Set following to true to deselect device before next transfer
    		spi[i].cs_change 		= 0;	
		}

		// ioctl wizardry to send transfer message structure "spi"
    	retVal = ioctl(this->spifd, SPI_IOC_MESSAGE(length), &spi);

    	// Check for error and return
    	if(retVal < 0){
    		perror("Problem transmitting data with ioctl");
    		exit(1);
    	}

    	return retVal;
}
//ltc2470() - default constructor. Not recommended for our device
ltc2470::ltc2470(){
		// Mode sets clock polarity and phase. Don't muck with this
		this->mode = SPI_MODE_0;
		this->bitsPerWord = 8;
		this->speed = 1000000;	// 1 MHz
		// File descriptor for SPI device. Gets set by spiOpen
		this->spifd = -1;

		// Call spiOpen to configure device
		// As arg, give location of spi device on RPi
		this->spiOpen(std::string("/dev/spidev0.0"));
}
//ltc2470(...) - overloaded constructor. Good!
// INPUTS - devspi: string with location of spidev0.0, 0.1
//			spiMode: 0,1,2, or 3: sets clock polarity and phase
//			spiSpeed: bit rate in Hz
//			spiBitsPerWord: number of bits per word
ltc2470::ltc2470(std::string devspi, unsigned char spiMode, unsigned int spiSpeed, unsigned char spiBitsPerWord){
		// Mode sets clock polarity and phase. Don't muck with this
		this->mode = spiMode;
		this->bitsPerWord = spiBitsPerWord;
		this->speed = spiSpeed; // Hz
		// File descriptor for SPI device. Gets set by spiOpen
		this->spifd = -1;

		// Call spiOpen to configure device
		// As arg, give location of spi device on RPi
		this->spiOpen(std::string(devspi));
}
//~ltc2470() - destructor
ltc2470::~ltc2470(){
	// Call private close function
	this->spiClose();
}
