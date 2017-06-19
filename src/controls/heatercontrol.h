#ifndef RADIANCE_SRC_CONTROLS_HEATERCONTROL_H_
#define RADIANCE_SRC_CONTROLS_HEATERCONTROL_H_

namespace RADIANCE {

  // Class for initializing and commanding the internal heaters
  class HeaterControl {

  public:

    // Initialize the heater with the gpio pin as argument
    HeaterControl(int gpio_pin);

    // Turn the heater on
    void CommandHeaterOn();

    // Turn the heater off
    void CommandHeaterOff();

    // Return heater status
    bool IsHeaterOn();
  private:
    // The heater GPIO pin number, passed into the constructor
    const int gpio_pin_;

    // Current heater status
    bool is_heater_on_;

    // The heater on and off values range between 0 and 255
    const int kGpioOn = 1;
    const int kGpioOff = 0;

  };

}
#endif // RADIANCE_SRC_CONTROLS_HEATERCONTROL_H_
