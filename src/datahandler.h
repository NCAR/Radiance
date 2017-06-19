#ifndef RADIANCE_SRC_DATAHANDLER_H_
#define RADIANCE_SRC_DATAHANDLER_H_

#include <iostream>
#include <fstream>
#include <array>
#include "sensors/spectrometer.h"
#include "sensors/rpitemperaturesensor.h"
#include "sensors/internaltemperaturesensor.h"
#include "sensors/externaltemperaturesensor.h"
#include "sensors/humiditysensor.h"
#include "sensors/attitude.h"
#include "sensors/camera.h"

namespace RADIANCE {
  // DataHandler encapsulates the data transfer process
  // The read functions set the data in the science data struct
  class DataHandler{
    
  private:
  // Takes a value and writes the binary information to given stream
  template <class T> std::ostream& BinaryWrite(std::ostream& stream, const T& value);

  public:
    
    // Reads a measurement from each sensor and places it into the
    // science data struct.
    void ReadSensorData();

    // Writes the science data struct to storage
    void WriteFrameToStorage();

    // Structure type to hold all the sensor data for each frame
    struct frame_data_type {
      unsigned int time_stamp;
      std::array<float,Spectrometer::kNumPixels> spectrum;
      float spectrometer_temperature;
      float rpi_temperature;
      float upper_battery_temperature;
      float lower_battery_temperature;
      float storage_temperature;
      float external_temperature;
      float humidity;
      std::array<float,4> attitude_values;
    };

    // Gets the frame_data struct for other routines
    frame_data_type GetFrameData();

  private:

    // Holds the science data for each frame
    frame_data_type frame_data_;

    // Sensor data members
    attitude radiance_ads;
    Camera camera_;
    ExternalTemperatureSensor external_temperature_sensor_;
    HumiditySensor humidity_sensor_;
    InternalTemperatureSensor lower_battery_temperature_sensor_{"10-000803362138"};
    InternalTemperatureSensor storage_temperature_sensor_{"10-00080336550e"};
    InternalTemperatureSensor upper_battery_temperature_sensor_{"10-00080336329d"}; // Sensor serial number
    RPiTemperatureSensor rpi_temperature_sensor_;
    Spectrometer spectrometer_;

    // Storage data objects for regular data
    // These are kept open for performance
    std::ofstream slc_data_file_{"/mnt/slcdrive/datafile",std::ios::binary|std::ios::app};
    std::ofstream mlc1_data_file_{"/mnt/mlcdrive1/datafile",std::ios::binary|std::ios::app};
    std::ofstream mlc2_data_file_{"/mnt/mlcdrive2/datafile",std::ios::binary|std::ios::app};

    // Writes the frame data to the given file
    void WriteDataToFile(std::ofstream& file);
  };


} // namespace RADIANCE
#endif //RADIANCE_SRC_DATAHANDLER_H_
