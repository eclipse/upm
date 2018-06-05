#pragma once

#include <sstream>
#include <fstream>
#include <string>
#include <chrono>
#include <thread>
#include <mutex>

/* Helper macro for logger utility class. */
#define UPM_LOG(log_level) \
  if ( log_level < UPM_LOGGER::LogLevel() ) \
    ; \
  else \
    UPM_LOGGER().log(log_level) << __FILE__ << " " \
                                << __FUNCTION__ << " " \
                                << __LINE__ << ": "

#define LOG_FILE "/var/log/upm.log"

namespace upm {

  enum UpmLogLevel {
      LOG_ERROR,
      LOG_WARNING,
      LOG_INFO,
      LOG_DEBUG,
      NA
  };

  static const std::string logLevelNames[] = {
    "ERROR",
    "WARNING",
    "INFO",
    "DEBUG",
    ""
  };

  class UPM_LOGGER {
  public:
    UPM_LOGGER();
    virtual ~UPM_LOGGER();
    std::ostringstream& log(UpmLogLevel level = LOG_ERROR);
    static UpmLogLevel& LogLevel();

    static std::ofstream logStream;
    static std::mutex logMutex;
  protected:
    std::ostringstream os;
  private:
    UPM_LOGGER(const UPM_LOGGER&);
    UPM_LOGGER& operator=(const UPM_LOGGER&);
  private:
    // Track logLevel for this particular instance of UPM_LOGGER
    UpmLogLevel logLevel;

  public:
    static const std::string& getLogLevelName(UpmLogLevel level)
    {
      if (level < LOG_ERROR || level >= NA) {
        return NULL;
      }

      return logLevelNames[level];
    }

  public:
    static void write(std::ostringstream& os)
    {


      std::lock_guard<std::mutex> lock(logMutex);

      logStream.open(LOG_FILE, std::ios_base::app);
      logStream << os.str();
      logStream.flush();
    }
  };

  UPM_LOGGER::UPM_LOGGER() {}


  UpmLogLevel& UPM_LOGGER::LogLevel()
  {
    static UpmLogLevel reportingLevel = LOG_DEBUG;

    return reportingLevel;
  }

  std::ostringstream& UPM_LOGGER::log(UpmLogLevel level)
  {
    using namespace std::chrono;

    os << "UPM - " << duration_cast<nanoseconds>
      (system_clock::now().time_since_epoch()).count();
    os << " " << getLogLevelName(level) << " ";

    logLevel = level;

    return os;
  }

  UPM_LOGGER::~UPM_LOGGER()
  {
    os << std::endl;

    write(os);
  }
}
