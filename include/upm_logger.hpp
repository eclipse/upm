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

#include <fstream>
#include <mutex>
#include <sstream>
#include <string>
#include <thread>
#include "date.h"

/* Helper macro for logger utility class. */
#define UPM_LOG(log_level)                                                                       \
    if (log_level < UPM_LOGGER::LogLevel())                                                      \
        ;                                                                                        \
    else                                                                                         \
    (LogHelper() && UPM_LOGGER::getInstance()).log(log_level) << __FILE__ << " " << __FUNCTION__ \
                                                              << " " << __LINE__ << ": "

#define DEFAULT_LOG_FILE "/dev/stdout"


namespace upm
{
enum UpmLogLevel { LOG_ERROR, LOG_WARNING, LOG_INFO, LOG_DEBUG, NA };

static const std::string logLevelNames[] = { "ERROR", "WARNING", "INFO", "DEBUG", "" };

class UPM_LOGGER
{
    friend class LogHelper;

  public:
    static UPM_LOGGER&
    getInstance()
    {
        static UPM_LOGGER instance;
        return instance;
    }

    virtual ~UPM_LOGGER()
    {
    }

    std::ofstream&
    log(UpmLogLevel level = LOG_ERROR)
    {
        using namespace date;
        
        _logStream << "UPM - "
                   << std::chrono::system_clock::now();
        _logStream << " [" << getLogLevelName(level) << "] ";

        return _logStream;
    }

    static UpmLogLevel&
    LogLevel()
    {
        static UpmLogLevel reportingLevel = LOG_DEBUG;

        return reportingLevel;
    }

    static const std::string&
    getLogLevelName(UpmLogLevel level)
    {
        if (level < LOG_ERROR || level >= NA) {
            return NULL;
        }

        return logLevelNames[level];
    }

    static std::string&
    outputFile()
    {
        static std::string file = DEFAULT_LOG_FILE;
        return file;
    }


  private:
    UPM_LOGGER()
    {
        _logStream.open(outputFile(), std::ios_base::app);
    }

    static std::mutex&
    logMutex()
    {
        static std::mutex _mutex;
        return _mutex;
    }

  public:
    UPM_LOGGER(const UPM_LOGGER&) = delete;
    UPM_LOGGER& operator=(const UPM_LOGGER&) = delete;

  private:
    std::ofstream _logStream;
};

/* This class exists only to append a newline when the log statement ends.*/
class LogHelper
{
  public:
    LogHelper()
    {
        _mutex = &UPM_LOGGER::logMutex();
        _mutex->lock();
        _os = &UPM_LOGGER::getInstance()._logStream;
    }

    ~LogHelper()
    {
        *(_os) << std::endl;
        _mutex->unlock();
    }

    friend UPM_LOGGER&
    operator&&(const LogHelper& l, UPM_LOGGER& r)
    {
        return r;
    }

  private:
    std::ofstream* _os;
    std::mutex* _mutex;
};
}
