#ifndef RADIANCE_SRC_MICROCONTROLLER_H_
#define RADIANCE_SRC_MICROCONTROLLER_H_

#include "datahandler.h"
#include "controls/heatercontrol.h"

namespace RADIANCE {
  // Controls the RADIANCE software. Encapsulates the command
  // functions of the Rasperry Pi 3.
  class Microcontroller {
  public:

    // Setup and configure sensors
    Microcontroller();

    // Step one frame
    void UpdateFrameCounter();

    // Start the system (infinite) loop
    void StartLoop();

    // Process the measured data and then write the heater output
    void SetThermalControl(DataHandler::frame_data_type frame_data);

    // Heater temp getters
    static float GetMaxHeaterTemp();
    static float GetMinHeaterTemp();
    

  private:
    // Minimum and maximum heater temperatures
    static constexpr float kMinHeaterTemp = 1;
    static constexpr float kMaxHeaterTemp = 3;

    // Used to handle input/output of data from reading the sensors to storage
    DataHandler data_handler_;

    // Initialize the heater controls. 
    HeaterControl spectrometer_heater_{24}; // GPIO pin number
    HeaterControl battery_heater_{23};      // GPIO pin number
  };


} // namespace RADIANCE
#endif //RADIANCE_SRC_MICROCONTROLLER_H_
