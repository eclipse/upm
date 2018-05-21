#include "upm_logger.hpp"

using namespace upm;

int main()
{

  UPM_LOGGER::getLogLevel() = LOG_ERROR;

  UPM_LOGGER().get(LOG_WARNING) << "a loop with 4 iterations";

  for (int i = 0; i < 4; ++i) {
    UPM_LOGGER().get(LOG_DEBUG) << "i = " << i;
  }

  return 0;
}