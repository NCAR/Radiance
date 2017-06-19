#include <stdio.h>
#include <sys/types.h>
#include <chrono>
#include <limits>
#include <iostream>
#include <thread>
#include <stdexcept>
#include "../include/avaspec/avaspec.h"
#include "systemhaltexception.h"
#include "datahandler.h"
#include "microcontroller.h"
#include "controls/heatercontrol.h"

namespace RADIANCE {

  // Reads a measurement from each sensor and places it into the
  // science data struct.
  // In general, if housekeep sensors cannot be read return high heater temperature
  // If science instruments cannot be read return zero
  void DataHandler::ReadSensorData() {

    // Read timestamp measurement
    // This timestamp represents seconds since Unix epoch
    std::chrono::seconds ms = std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now().time_since_epoch());
    frame_data_.time_stamp = ms.count();
    std::cout << "Timestamp: " << frame_data_.time_stamp << std::endl;

    // Read main instrument(spectrometer)
    // If the spectrometer cannot be read from, throw an exception to restart the system
    if (!spectrometer_.ReadSpectrum(frame_data_.spectrum)) {
      throw SystemHaltException();
    }

    // Take a picture, if necessary
    // The time stamp is necessary to know how long ago a picture was taken
      camera_.TakePicture(frame_data_.time_stamp);

    // Read temperature sensors
    // These sensors are used for heater control. Overheating is a concern, so 
    // if the sensors are not responding, assume sensor is already at desired temperature
    if (!spectrometer_.ReadSpectrometerTemperature(frame_data_.spectrometer_temperature)) {
      frame_data_.spectrometer_temperature = Microcontroller::GetMaxHeaterTemp();
    }

    if (!rpi_temperature_sensor_.ReadTemperature(frame_data_.rpi_temperature)) {
      frame_data_.rpi_temperature = Microcontroller::GetMaxHeaterTemp();
    }

    if (!upper_battery_temperature_sensor_.ReadTemperature(frame_data_.upper_battery_temperature)) {
      frame_data_.upper_battery_temperature = Microcontroller::GetMaxHeaterTemp();
    }

    if (!lower_battery_temperature_sensor_.ReadTemperature(frame_data_.lower_battery_temperature)) {
      frame_data_.lower_battery_temperature = Microcontroller::GetMaxHeaterTemp();
    }

    if (!storage_temperature_sensor_.ReadTemperature(frame_data_.storage_temperature)) {
      frame_data_.storage_temperature = Microcontroller::GetMaxHeaterTemp();
    }

    // Read environmental sensors
    // These sensors are not used in heating calculations so return 0 if they cannot be read
    if (!external_temperature_sensor_.ReadTemperature(frame_data_.external_temperature)) {
      frame_data_.external_temperature = 0;
    }

    if (!humidity_sensor_.ReadHumidity(frame_data_.humidity)) {
      frame_data_.humidity = 0;
    }

    for(int i = 0; i < 4; ++i){
    	frame_data_.attitude_values[i] = radiance_ads.ads_read(i+1);
    }
    
  }

  // Writes the frame data to a csv file
  void DataHandler::WriteFrameToStorage() {

    // Make sure at least one data file can be written to
    // If not, restart the system
    if (!slc_data_file_.good() && !mlc1_data_file_.good() && !mlc2_data_file_.good()) {
      throw SystemHaltException();
    }

    // Writes the data(measurements) to all three drives every second
    WriteDataToFile(slc_data_file_);
    WriteDataToFile(mlc1_data_file_);
    WriteDataToFile(mlc2_data_file_);
  }

  // Gets the frame_data struct for other routines
  DataHandler::frame_data_type DataHandler::GetFrameData() {return frame_data_;}

  // Writes the frame data to the given file
  // Inputs:
  // file: The ofstream object to write to. If the file cannot be found/written to do nothing
  void DataHandler::WriteDataToFile(std::ofstream& file) {

    // Check that the data file is OK
    // If not OK, do nothing
    if (file.good()) {
    
      // Write timestamp of measurement
      DataHandler::BinaryWrite(file,frame_data_.time_stamp);

      // Write the spectrometer measurements
      for (auto& i : frame_data_.spectrum) {
        DataHandler::BinaryWrite(file,i);
      }
      // Write the engineering/housekeeping measurements to the given file
      DataHandler::BinaryWrite(file,frame_data_.spectrometer_temperature);
      DataHandler::BinaryWrite(file,frame_data_.rpi_temperature);
      DataHandler::BinaryWrite(file,frame_data_.upper_battery_temperature);
      DataHandler::BinaryWrite(file,frame_data_.lower_battery_temperature);
      DataHandler::BinaryWrite(file,frame_data_.storage_temperature);
      DataHandler::BinaryWrite(file,frame_data_.external_temperature);
      DataHandler::BinaryWrite(file,frame_data_.humidity);

      // Write the attitude measurements
      for (auto& i : frame_data_.attitude_values) {
        DataHandler::BinaryWrite(file,i);
      }

      file.flush();
    }
  }

  // Takes a value and writes the binary information to given stream
  template <class T> std::ostream& DataHandler::BinaryWrite(std::ostream& stream, const T& value) {
    return stream.write(reinterpret_cast<const char*>(&value), sizeof(T));
  }

}
