#ifndef RADIANCE_SRC_SENSORS_CAMERA_H_
#define RADIANCE_SRC_SENSORS_CAMERA_H_

namespace RADIANCE {
  // Encapsulates the camera configuration and reading
  class Camera {

  public:

    // Reads and writes the camera image
    bool TakePicture(unsigned int current_time);

  private:
    // Timestamp of last picture
    // If 60s has passed, take new picture and update timestamp
    unsigned int last_picture_time_=0;
  };
  
} // namespace RADIANCE
#endif //RADIANCE_SRC_SENSORS_CAMERA_H_
