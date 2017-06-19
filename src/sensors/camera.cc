#include <cstdlib>
#include <iostream>
#include "camera.h"
#include <string>
#include <cstdlib>

namespace RADIANCE{

  // Reads and writes the camera image
  // Calls the system utility 'raspistill' and then copies the images using tee
  // Only takes picture if current time and last picture time differ by 60 seconds
  // Returns true if read succeeds
  // Please do not judge me for writing this garbage
  bool Camera::TakePicture(unsigned int current_time) {
    
    if (last_picture_time_==0) {
      last_picture_time_=current_time;
    }

    if ((current_time-last_picture_time_) >= 60) {
      last_picture_time_=current_time;
      std::cout << "Taking picture" << std::endl; // DEBUG
      
      std::system(("raspistill --width 1920 --height 1080 --nopreview --timeout 250 -o /mnt/mlcdrive1/images/" + std::to_string(current_time) +  "| tee /mnt/mlcdrive2/images/" + std::to_string(current_time)).c_str());
    }
      return true;
  }
}
