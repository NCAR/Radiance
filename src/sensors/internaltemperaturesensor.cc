#include <fstream>
#include <string.h>
#include <unistd.h>
#include <iostream>
#include <stdexcept>
#include "internaltemperaturesensor.h"

namespace RADIANCE{

  // Set sensor location based on serial number
  InternalTemperatureSensor::InternalTemperatureSensor(std::string sensor_file):sensor_file_(sensor_file) {
  }

  // Reads temperature from temperature file
  // Opens the sensor file, filter to the temperature and covert to celcius
  // Returns false if read failed
  bool InternalTemperatureSensor::ReadTemperature(float& temp) {

    // Open the file and save the file handle
    std::ifstream temp_file(kLocationStrPrefix + sensor_file_ + kLocationStrPostfix);

    // If the file cannot be read from return error
    if (!temp_file.good()) {
      return false;
    }

    // Read the file into a buffer
    std::string buf((std::istreambuf_iterator<char>(temp_file)),
                 std::istreambuf_iterator<char>());

    // Check if the file has the standard format
    int match = buf.find("t=");

    // If the format string cannot be found return error
    if (match == std::string::npos) {
      return false;
    }
    // Cut the string to just the temperature information
    // The temperature information stats after 't=' so add 2
    std::string temp_str(buf.substr(match+2));

    // Convert the string to float and convert to actual value
    temp = std::stof(temp_str)/1000;
    return true;
  }
}
