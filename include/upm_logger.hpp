#pragma once

#include <sstream>
#include <string>
#include <chrono>
#include <stdio.h>

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
    std::ostringstream& get(UpmLogLevel level = LOG_INFO);
    static UpmLogLevel& getLogLevel();
  protected:
    std::ostringstream os;
  private:
    UPM_LOGGER(const UPM_LOGGER&);
    UPM_LOGGER& operator=(const UPM_LOGGER&);
  private:
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


  UpmLogLevel& UPM_LOGGER::getLogLevel()
  {
    static UpmLogLevel reportingLevel = LOG_DEBUG;
    return reportingLevel;
  }

  std::ostringstream& UPM_LOGGER::get(UpmLogLevel level)
  {
    using namespace std::chrono;

    os << "- " << duration_cast<milliseconds>
      (system_clock::now().time_since_epoch()).count();
    os << " " << getLogLevelName(level) << ": ";
    os << std::string(level > LOG_DEBUG ? level - LOG_DEBUG : 0, '\t');
    logLevel = level;

    return os;
  }

  UPM_LOGGER::~UPM_LOGGER()
  {
    if (logLevel >= UPM_LOGGER::getLogLevel()) {
      os << std::endl;
      fprintf(stderr, "%s", os.str().c_str());
      fflush(stderr);
    }
  }

}


