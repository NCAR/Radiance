#include <pigpio.h>
#include <iostream>
#include "heatercontrol.h"

namespace RADIANCE{

  // Initialize the heater, the argument is the GPIO pin. The heater
  // control is actually a PWM signal to a mosfet controlling the
  // heater. Therefore, turning the PWM signal on and off turns the
  // heater on and off.
  HeaterControl::HeaterControl(int gpio_pin):gpio_pin_(gpio_pin) {
    gpioInitialise();
    std::cout << gpioSetMode(gpio_pin_,PI_OUTPUT) << std::endl;
    gpioWrite(gpio_pin_,kGpioOff);
    is_heater_on_ = false;
  }

  // Turn the heater on
  void HeaterControl::CommandHeaterOn() {
    std::cout << "Turning heater on" << std::endl; //DEBUg
    std::cout << gpioWrite(gpio_pin_,kGpioOn) << std::endl;
    is_heater_on_ = true;
  }

  // Turn the heater off
  void HeaterControl::CommandHeaterOff() {
    gpioWrite(gpio_pin_,kGpioOff);
    is_heater_on_ = false;
  }

  // Return heater status
  bool HeaterControl::IsHeaterOn() {return is_heater_on_;}
}
