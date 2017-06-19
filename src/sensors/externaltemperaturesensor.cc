#include <pigpio.h>
#include <iostream>
#include <memory>
#include "externaltemperaturesensor.h"

namespace RADIANCE{
  // Initializes the temperature sensor by setting the file handle to the I2C address
  ExternalTemperatureSensor::ExternalTemperatureSensor() {
    gpioInitialise();
    file_handle_ = i2cOpen(1,kI2cAddress,0);
  }

  // Read temperature from sensor
  // Create buffer and read two bytes from register
  // Success error code is number of bytes read(2)
  bool ExternalTemperatureSensor::ReadTemperature(float& temp) {
    char buf[2];

    // Check if measurement failed
    if (i2cReadI2CBlockData(file_handle_,kTempRegister,buf,2)==2) {
      temp = ConvertBlockDataToTemperature(buf);
      return true;
    } else {
      return false;
    }

  }

  // Converts two's complement block data to temperature measurement
  float ExternalTemperatureSensor::ConvertBlockDataToTemperature(char buf[2]) {
    // Combines high and low measurements
    int counts = buf[0] << 4 | buf[1] >> 4;

    // Check if temperature is negative
    // If negative, take complement and add 1
    if (buf[0] & 0x10000000) {
      counts = ~counts+1;
    }

    return counts*kCountsToCelsius;
  }
}
