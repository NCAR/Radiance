#ifndef RADIANCE_SRC_SENSORS_HUMIDITYSENSOR_H_
#define RADIANCE_SRC_SENSORS_HUMIDITYSENSOR_H_
namespace RADIANCE {

  // Class for interacting with the TE HPP804B130 humidity sensor
  class HumiditySensor {


    // Callback function for each edge
    static void EdgeCallbackFunction(int gpio, int level, uint32_t tick);

  public:

    // Initialize the sensor and start the ability to read
    HumiditySensor();

    // Return a humidity measurement
    // Returns false if the read failed
    bool ReadHumidity(float& humidity);
    
  private:
    // Convert a measurement from the humidity sensor to a humidity measurement
    float ConvertFrequencyToHumidity(float freq);

    // Gpio pin to use
    const int kGpioPin = 17;

    // Pulse period in microseconds
    const int kPulseDelay = 20;
  };

}
#endif // RADIANCE_SRC_SENSORS_HUMIDITYSENSOR_H_
