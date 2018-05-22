#pragma once

#include <sstream>
#include <string>
#include <chrono>
#include <stdio.h>

/* Helper macro for logger utility class. */
#define UPM_LOG(log_level) \
  if ( log_level < UPM_LOGGER::LogLevel() ) \
    ; \
  else \
    UPM_LOGGER().log(log_level) << __FILE__ << " " \
                                << __FUNCTION__ << " " \
                                << __LINE__ << ": "

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
    fprintf(stderr, "%s", os.str().c_str());
    fflush(stderr);
  }
}
