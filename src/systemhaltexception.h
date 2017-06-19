#ifndef RADIANCE_SRC_SYSTEMHALTEXCEPTION_H_
#define RADIANCE_SRC_SYSTEMHALTEXCEPTION_H_
#define LINUX_REBOOT_CMD_RESTART 0x1234567
#include <exception>


namespace RADIANCE {
  class SystemHaltException: public std::exception {
  public:

    virtual const char* what() const throw();
    static void RestartSystem();
  };

} // namespace RADIANCE
#endif //RADIANCE_SRC_SYSTEMHALTEXCEPTION_H_
