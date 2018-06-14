#include <thread>
#include <vector>

#include "upm_logger.hpp"

using namespace upm;

void print()
{
  UPM_LOG(LOG_DEBUG) << "Thread " << std::this_thread::get_id() << ": running loop with 3 iterations";

  for (int i = 0; i < 4; ++i) {
    UPM_LOG(LOG_DEBUG) << std::this_thread::get_id() << ": i = " << i;
  }
}

int main()
{
  std::vector<std::thread> threads;

  UPM_LOGGER::LogLevel() = LOG_ERROR;

  //UPM_LOGGER::outputFile() = "test.log";

  UPM_LOG(LOG_WARNING) << "Testing the upm logger 1 " << "4";

  // Launching 5 threads
  for (int i = 0; i < 3; ++i) {
    threads.push_back(std::thread(print));
  }

  for (auto& thread : threads) {
    thread.join();
  }

  return 0;
}