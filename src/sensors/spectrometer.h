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
	  
	//test test!!
	bool CalibTest();

    // Return a spectrum and pixelvalue measurement into the given array
    bool ReadSpectrum(std::array<float,kNumPixels>& spectrum, std::array<float,kNumPixels>& pixelvals);

    // Return spectrometer temperature
    // Returns false if read failed
    bool ReadSpectrometerTemperature(float& temp);
  private:

    // Spectrometer reference handle
    AvsHandle handle_;

    // Spectrometer measurement config
    MeasConfigType meas_config_;

	//Spectrometer device config
	DeviceConfigType dev_config_;
	  
	//Spectrum calibration config
	SpectrumCalibrationType spec_calib_;
	  
	//Irradiance type callibration config
	IrradianceType irrad_type_
	  
	  
    // Converts the voltage into a temperature
    float ConvertVoltageToTemperature(float voltage);
	  
  };

} // namespace RADIANCE
#endif //RADIANCE_SRC_SENSORS_SPECTROMETER_H_
