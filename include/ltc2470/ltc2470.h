#ifndef LTC2470_H
#define LTC2470_H

#include <iostream>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/spi/spidev.h>
#include <unistd.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Class for initializing and reading from LTC2470 SPI ADCs for photodiode measurements
// NOTE: this class is treated as a single SPI device (one MISO line, one MOSI line, etc)
// Selecting which ADC to read is handled in ads.cc by driving CS pins low
class ltc2470{

public:
	// Constructor declarations
	ltc2470();
	ltc2470(std::string devspi, unsigned char spiMode, unsigned int spiSpeed, unsigned char spiBitsPerWord);
	// Destructor declaration
	~ltc2470();

	// Write/read function declaration
	int spiWriteRead(unsigned char *data, int length);

private:
	// Properties of spidev objects. More info in ltc2470.cc
	unsigned char mode;
	unsigned char bitsPerWord;
	unsigned int speed;
	int spifd;

	// Private function declarations for opening and closing system files
	int spiOpen(std::string devspi);
	int spiClose();

};


#endif
