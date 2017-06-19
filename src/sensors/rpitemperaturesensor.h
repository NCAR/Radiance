#ifndef RADIANCE_SRC_SENSORS_RPITEMPERATURESENSOR_H_
#define RADIANCE_SRC_SENSORS_RPITEMPERATURESENSOR_H_

#include <fstream>
namespace RADIANCE {

  // Class for initializing and reading from the RPi temperature sensor
  class RPiTemperatureSensor {

  public:

    // Return a temperature measurement
    bool ReadTemperature(float& temp);
  private:
    // Location of temperature sensor file
    const std::string kLocationStr{"/sys/class/thermal/thermal_zone0/temp"};
  };

}
#endif // RADIANCE_SRC_SENSORS_RPITEMPERATURESENSOR_H_
