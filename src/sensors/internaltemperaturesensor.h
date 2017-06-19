#include <fcntl.h>
#include <stdlib.h>

#ifndef RADIANCE_SRC_SENSORS_INTERNALTEMPERATURESENSOR_H_
#define RADIANCE_SRC_SENSORS_INTERNALTEMPERATURESENSOR_H_
namespace RADIANCE {

  // Class for initializing and reading from the DS18B20 temperature sensor
  class InternalTemperatureSensor {

  public:
    // Set location based on serial number
    InternalTemperatureSensor(std::string serial_str);

    // Return a temperature measurement
    bool ReadTemperature(float& temp);
  private:

    // Serial number of device, used for opening the sensor file
    const std::string sensor_file_;

    // Path to slave device
    const std::string kLocationStrPrefix{"/sys/bus/w1/drivers/w1_slave_driver/"};
    const std::string kLocationStrPostfix{"/w1_slave"};

  };

}
#endif // RADIANCE_SRC_SENSORS_INTERNALTEMPERATURESENSOR_H_
