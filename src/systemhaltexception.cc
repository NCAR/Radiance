#include <unistd.h>
#include <sys/reboot.h>
#include "systemhaltexception.h"
namespace RADIANCE {

  const char* SystemHaltException::what() const throw(){
    SystemHaltException::RestartSystem();
    return "";
  }
  // Error handler function; first syncs the file system and then restarts the pi
  void SystemHaltException::RestartSystem() {
    sync();
    reboot(LINUX_REBOOT_CMD_RESTART);

  }

}
