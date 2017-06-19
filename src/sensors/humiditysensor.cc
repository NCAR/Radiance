#include <cmath>
#include <pigpio.h>
#include <stdexcept>
#include <iostream>
#include "humiditysensor.h"

namespace RADIANCE{

  // Set to one if reset is necessary
  static int reset_counter=1;

  // Pulse track counter
  static int pulse_counter=1;
  
  // Callback function for each edge
  void HumiditySensor::EdgeCallbackFunction(int gpio, int level, uint32_t tick)
  {
    std::cout << "got callback" << std::endl; //DEBUG
    if (reset_counter)
      {
        reset_counter = 0;
        pulse_counter = 0;
      }

    // only record low to high edges
    if (level == 1)
      pulse_counter++;
  }
  
  // Initializes humidity sensor and prepares it for measurement
  HumiditySensor::HumiditySensor() {
    gpioInitialise();

    int wave_id, mode;
    gpioPulse_t pulse[2];
    int kPulseDelay = 20;
    int g_mask = 1;

    gpioWaveClear();

    pulse[0].gpioOn  = g_mask;
    pulse[0].gpioOff = 0;
    pulse[0].usDelay = kPulseDelay;

    pulse[1].gpioOn  = 0;
    pulse[1].gpioOff = g_mask;
    pulse[1].usDelay = kPulseDelay;

    gpioWaveAddGeneric(2, pulse);

    wave_id = gpioWaveCreate();

    gpioSetAlertFunc(kGpioPin, EdgeCallbackFunction);
    gpioSetMode(kGpioPin, PI_INPUT);
  }

  // Reads a humidity measurement. This requires finding the frequency
  // of the wave and then converting that measurement to percent
  // relative humidity
  // Returns false if the read failed
  bool HumiditySensor::ReadHumidity(float& humidity){
    reset_counter = 1;

    // Check if any pulses are detected. 
    // If not, throw an error
    if (pulse_counter<1) {
      return false;
    }

    float period = pulse_counter/kPulseDelay;
    float frequency = 1/period;

    humidity = ConvertFrequencyToHumidity(frequency);
    return true;
  }

  // Converts frequency to humidity using a polynomial fit equation
  float HumiditySensor::ConvertFrequencyToHumidity(float freq) {
    return 1.2158e-8*std::pow(freq,3)-2.193e-4*std::pow(freq,2)+1.2094*freq-1.8698e3;
  }

}
