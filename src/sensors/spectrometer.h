#ifndef RADIANCE_SRC_SENSORS_SPECTROMETER_H_
#define RADIANCE_SRC_SENSORS_SPECTROMETER_H_
#include "../../include/avaspec/avaspec.h"

namespace RADIANCE {
  // Encapsulates the spectrometer configuration and reading
  class Spectrometer {

  public:
    // Setup and configure the spectrometer
    Spectrometer();

    // Number of spectrometer elements
    static const int kNumPixels = 2048;

    // Return a spectrum measurement into the given array
    bool ReadSpectrum(std::array<float,kNumPixels>& spectrum);

    // Return spectrometer temperature
    // Returns false if read failed
    bool ReadSpectrometerTemperature(float& temp);
  private:

    // Spectrometer reference handle
    AvsHandle handle_;

    // Spectrometer measurement config
    MeasConfigType meas_config_;

    // Converts the voltage into a temperature
    float ConvertVoltageToTemperature(float voltage);

  };

} // namespace RADIANCE
#endif //RADIANCE_SRC_SENSORS_SPECTROMETER_H_
