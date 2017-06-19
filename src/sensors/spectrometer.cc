#include <chrono>
#include <thread>
#include <stdexcept>
#include "spectrometer.h"
#include <iostream>
#include <algorithm>
#include <cmath>
#include "../systemhaltexception.h"
namespace RADIANCE{

  // Setup and configure the spectrometer
  // The spectrometer is the only mission-critical item. This means that
  // the software (should) still run even if only the spec and one drive
  // is connected. If either all drives are not functioning or the spec
  // cannot be found then the software will restart and try to
  // re-initalize.
  // Start by finding and retrieving the spectrometer handle, then configuring the measurements
  Spectrometer::Spectrometer() {

    // Initialize the library
    // 0 is connected via USB
    // If the spec cannot be found, restart the Pi and try again.
    if (AVS_Init(0)==ERR_SUCCESS) {
      throw std::runtime_error("Could not initialize spectrometer");
    }

    // Parameters for calling AVS_GetList()
    unsigned int byte_set;
    AvsIdentityType a_plist[30];
    AVS_GetList(sizeof(a_plist), // Number of bytes of list data
                    &byte_set, // Number of bytes needed to store information
                    a_plist);    // Pointer to storage of identity information;

    // Retrieve handle
    handle_ = AVS_Activate(&a_plist[0]);

    // Set the ADC to high res mode
    AVS_UseHighResAdc(handle_,1);

    // Configure the measurement
    meas_config_.m_StartPixel      = 0;
    meas_config_.m_StopPixel       = (kNumPixels - 1);
    meas_config_.m_IntegrationTime   = 1.05;
    meas_config_.m_IntegrationDelay    = 0;
    meas_config_.m_NrAverages      = 1;

    // Dark correction
    meas_config_.m_CorDynDark.m_Enable = 0; // Enable dark correction
    meas_config_.m_CorDynDark.m_ForgetPercentage = 0; // Percentage of the new dark value pixels that has to be used


    // Smoothing
    meas_config_.m_Smoothing.m_SmoothPix = 0; // Number of neighbor pixels used for smoothing
    meas_config_.m_Smoothing.m_SmoothModel = 0; // Only one model available(0)

    // Saturation Detection
    meas_config_.m_SaturationDetection = 0; // Saturation detection, 0 is disabled

    // Trigger modes
    meas_config_.m_Trigger.m_Mode    = 0; // Mode, 0 is software
    meas_config_.m_Trigger.m_Source    = 0; // Source, 0 is external trigger
    meas_config_.m_Trigger.m_SourceType  = 0; // Source type, 0 is edge trigger

    // Control settings
    meas_config_.m_Control.m_StrobeControl = 0; // Number of strobe pulses during integration, 0 is no strobe pulses
    meas_config_.m_Control.m_LaserDelay    = 0; // Laser delay since trigger(unit is FPGA clock cycles)
    meas_config_.m_Control.m_LaserWidth    = 0; // Laser pulse width(unit is FPGA clock cycles), 0 is no laser pulse
    meas_config_.m_Control.m_LaserWaveLength = 1; // Peak wavelength of laser(nm), used for Raman spectroscopy
    meas_config_.m_Control.m_StoreToRam      = 0; // Number of spectra to be store to RAM
  }

  // Reads the spectrum with the setup handle. First calls AVS_Measure
  // which starts the read and then waits until a spectrum is
  // ready. Then reads and converts to float array for storage
  // Returns false if read failed
  bool Spectrometer::ReadSpectrum(std::array<float,kNumPixels>& f_spectrum) {

    // Configure the spectrometer with the measurement config
    // If the spectrometer is not found, restart the Pi
    if (AVS_PrepareMeasure(handle_,&meas_config_)!=ERR_SUCCESS) {
      std::cerr << "Could not find spectrometer when attempting to read" << std::endl; //DEBUG
      return false;
    }
    // Signal to start the measurement
    if (AVS_Measure(handle_,0,1)!=ERR_SUCCESS) {
      std::cerr << "Err in Measure" << std::endl; //DEBUG
      return false;
    }

    // Wait for the measurement to complete
    while (AVS_PollScan(handle_)!=1) {
      std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }

    // Avantes library requires double array so use for measuring
    double d_spectrum[kNumPixels];

    // Get spectrum from device
    if (AVS_GetLambda(handle_,d_spectrum)!=ERR_SUCCESS) {
      std::cerr << "Err in GetScopeData" << std::endl; //DEBUG
      return false;
    }

    // Convert spectrum to floats for storage efficiency
    for (int i=0; i < kNumPixels; i++) {
      f_spectrum[i] = (float) d_spectrum[i];
    }

    return true;
  }

  // Returns spectrometer internal temperature
  bool Spectrometer::ReadSpectrometerTemperature(float& temp) {

    float voltage;

    // Get spectrometer thermistor voltage and return error code
    if (AVS_GetAnalogIn(handle_, 0, &voltage)!=ERR_SUCCESS) {
      return false;
    }

    // Need to convert the result to an actual measurement
    temp =  Spectrometer::ConvertVoltageToTemperature(voltage);
    return true;
     
  }
  // Converts the voltage into a temperature using a predefined polynomial
  float Spectrometer::ConvertVoltageToTemperature(float voltage) {
    return 118.69 - 70.361*(voltage)+ 21.02*std::pow(voltage,2) - 3.6443*std::pow(voltage,3) + 0.1993*std::pow(voltage,4);

  }
}

