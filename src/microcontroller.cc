#include <math.h>
#include <chrono>
#include <iostream>
#include <thread>
#include <stdexcept>
#include <pigpio.h>
#include "microcontroller.h"
#include "datahandler.h"
#include "systemhaltexception.h"
#include "controls/heatercontrol.h"

namespace RADIANCE {

  // Setup and configure sensors by calling Data Handler instance
  Microcontroller::Microcontroller() {
    // Set terminate handler to custom restart function
    // This will restart the pi if an unknown exception occurs
    // std::set_terminate(SystemHaltException::RestartSystem);
  }

  // Sets heater output based on the information in frame_data
  // The thermal algorithm is a dead zone between the minimum and maximum heater temperature(kMinHeaterTemp and kMaxHeaterTemp)
  void Microcontroller::SetThermalControl(DataHandler::frame_data_type frame_data) {

    // Spectrometer heating
    if (frame_data.spectrometer_temperature <= kMinHeaterTemp && !spectrometer_heater_.IsHeaterOn()){
      spectrometer_heater_.CommandHeaterOn();
    } else if (frame_data.spectrometer_temperature >= kMaxHeaterTemp && spectrometer_heater_.IsHeaterOn()) {
      spectrometer_heater_.CommandHeaterOff();
    }

    // Battery heating
    // First average the two battery temperatures
    float avg_battery_temperature = (frame_data.upper_battery_temperature + frame_data.lower_battery_temperature)/2;
    if (avg_battery_temperature <= kMinHeaterTemp && !battery_heater_.IsHeaterOn()){
      battery_heater_.CommandHeaterOn();
    } else if (avg_battery_temperature >= kMaxHeaterTemp && battery_heater_.IsHeaterOn()) {
      battery_heater_.CommandHeaterOff();
    }

  }

  // Heater temp getters
  float Microcontroller::GetMaxHeaterTemp() {return kMaxHeaterTemp;}
  float Microcontroller::GetMinHeaterTemp() {return kMinHeaterTemp;}

  // Start the system (infinite) loop 
  // System process is the following: Read -> Control Temperature -> Write -> Sleep(if necessary); 
  void Microcontroller::StartLoop() {

    std::chrono::high_resolution_clock::time_point begin,end;

    // System loop
    while (true) {

      // Start the clock
      begin = std::chrono::high_resolution_clock::now();

      // Read all sensors
      data_handler_.ReadSensorData();

      // Update the heater output
      Microcontroller::SetThermalControl(data_handler_.GetFrameData());

      // Write processed data to storage
      data_handler_.WriteFrameToStorage();

      // Calculate time taken(for heartbeat information)
      end = std::chrono::high_resolution_clock::now();
      std::chrono::duration<float> fs = end - begin;
      std::chrono::milliseconds ms = std::chrono::duration_cast<std::chrono::milliseconds>(fs);
      std::cout << "Milliseconds taken: " << ms.count() << std::endl; // DEBUG

      // Sleep, if necessary
      // This reduces the chance that too much data will be taken on the storage
      if (ms < std::chrono::milliseconds(1000)) {
        std::this_thread::sleep_for(std::chrono::milliseconds(1000)-ms);
      }

    }
  }
}
