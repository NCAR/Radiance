#ifndef RADIANCE_SRC_SENSORS_EXTERNALTEMPERATURESENSOR_H_
#define RADIANCE_SRC_SENSORS_EXTERNALTEMPERATURESENSOR_H_

namespace RADIANCE {

  // Class for initializing and reading from the TMP102 temperature sensor
  class ExternalTemperatureSensor {
public:
    // Initialize the temperature sensor and get it ready to read temperature
    ExternalTemperatureSensor();

    // Return a temperature measurement
    // Returns false if read failed
    bool ReadTemperature(float& temp);
private:

    // Convert i2c data to temperature
    float ConvertBlockDataToTemperature(char* buf);

    // Linux file handler
    int file_handle_;

    // I2C software address
    const int kI2cAddress = 0x48;

    // Temperature register number
    const int kTempRegister = 0;

    // Number of LSBs per Celsius
    const float kCountsToCelsius = 0.0625;
  };

}
#endif // RADIANCE_SRC_SENSORS_EXTERNALTEMPERATURESENSOR_H_
