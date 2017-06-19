#include "rpitemperaturesensor.h"
#include <iostream>
#include <iterator>
#include <string>
#include <stdexcept>

namespace RADIANCE{
  
  // Reads temperature from temperature sensor file
  // Opens the file, reads the temperature in as ASCII, then closes the file
  bool RPiTemperatureSensor::ReadTemperature(float& temp) {
    // Open the temperature file: temperature_file format is degrees Celcius in ASCII
    std::ifstream temp_file(kLocationStr);

    // Check if the file can be found and read
    if (!temp_file.good()) {
      return false;
    }

    // Read the temperature, close the file and return
    std::string buf((std::istreambuf_iterator<char>(temp_file)),std::istreambuf_iterator<char>());

    // Convert string to float and return actual temperature value
    temp = std::stof(buf)/1000;
    return true;
  }
}
