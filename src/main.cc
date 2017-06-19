#include <pigpio.h>
#include "microcontroller.h"

int main() {
  // Create object and start the system loop
  RADIANCE::Microcontroller raspberry_pi;
  raspberry_pi.StartLoop();

}

