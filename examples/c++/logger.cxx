#include "upm_logger.hpp"

using namespace upm;

int main()
{

  UPM_LOGGER::LogLevel() = LOG_INFO;

  UPM_LOG(LOG_WARNING) << "a loop with 4 iterations";

  for (int i = 0; i < 4; ++i) {
    UPM_LOG(LOG_DEBUG) << "i = " << i;
  }

  return 0;
}