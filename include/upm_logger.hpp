/*
 * Author: Mihai Stefanescu <mihai.stefanescu@rinftech.com>
 * Copyright (c) 2018 Intel Corporation.
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
 * LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
 * OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
 * WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

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
    UPM_LOGGER() {}
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
    std::ofstream logStream;
    static std::mutex logMutex;
  public:
    static const std::string& getLogLevelName(UpmLogLevel level);
  private:
    void write(std::ostringstream& os);
  };

  std::mutex UPM_LOGGER::logMutex;

  void UPM_LOGGER::write(std::ostringstream& os)
  {
    std::lock_guard<std::mutex> lock(logMutex);

    logStream.open(LOG_FILE, std::ios_base::app);
    logStream << os.str();
    logStream.flush();
  }

  const std::string& UPM_LOGGER::getLogLevelName(UpmLogLevel level)
  {
    if (level < LOG_ERROR || level >= NA) {
      return NULL;
    }

    return logLevelNames[level];
  }

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
