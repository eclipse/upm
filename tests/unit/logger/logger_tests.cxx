#include "upm_logger.hpp"
#include "gtest/gtest.h"
#include <cstdio>
#include <fstream>
#include <string>

#define LOG_FILE "test_log_file"

size_t logCount = 0;

using namespace upm;

/* Utilities test fixture */
class logger_unit : public ::testing::Test
{
  protected:
    /* One-time setup logic if needed */
    logger_unit()
    {
        UPM_LOGGER::outputFile() = LOG_FILE;
    }

    /* One-time tear-down logic if needed */
    virtual ~logger_unit()
    {
    }

    /* Per-test setup logic if needed */
    virtual void
    SetUp()
    {
    }

    /* Per-test tear-down logic if needed */
    virtual void
    TearDown()
    {
    }

    bool
    logFileExists(const std::string& logFile)
    {
        std::ifstream f(logFile.c_str());
        return f.good();
    }

    bool
    deleteLogFile(const std::string& logFile)
    {
        if (logFileExists(logFile)) {
            if (remove(logFile.c_str()) != 0) {
                return false;
            }
        }

        return true;
    }

    std::ifstream&
    goToLine(std::ifstream& file, unsigned line)
    {
        file.seekg(std::ios::beg);
        for (unsigned i = 0; i < line - 1; ++i) {
            file.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }

        return file;
    }

    std::size_t
    getMsgPosition(std::string& line)
    {
        std::size_t pos = line.find_first_of(':');

        for (int i = 0; i < 2 && pos != std::string::npos; ++i) {
            pos = line.find_first_of(':', pos + 1);
        }

        return (pos + 2);
    }
};

/* Test log file creation. */
TEST_F(logger_unit, test_logger_create_log_file)
{
    // We actually have to write a log because of the lazy instantiation.
    UPM_LOG(LOG_DEBUG);
    logCount++;

    ASSERT_TRUE(logFileExists(LOG_FILE));
}

/* Test simple log. */
TEST_F(logger_unit, test_logger_simple_log)
{
    const std::string log = "log sample for gtest";
    std::ifstream infile(LOG_FILE);
    std::string line;

    UPM_LOG(LOG_DEBUG) << log;
    logCount++;

    goToLine(infile, logCount);
    std::getline(infile, line);
    ASSERT_STREQ(log.c_str(), line.substr(getMsgPosition(line)).c_str());
}

/* Test multiple logs. */
TEST_F(logger_unit, test_logger_multiple_logs)
{
    const std::string logs[] = { "log1", "log2", "log3" };

    std::ifstream infile(LOG_FILE);
    std::string line;

    for (int i = 0; i < 3; ++i) {
        UPM_LOG(LOG_DEBUG) << logs[i];
        logCount++;
        goToLine(infile, logCount);
        std::getline(infile, line);
        ASSERT_STREQ(logs[i].c_str(), line.substr(getMsgPosition(line)).c_str());
    }
}

/* Test log levels. */
TEST_F(logger_unit, test_logger_log_level)
{
    // By default, UPM_LOGGER logs with DEBUG level.
    UPM_LOGGER::LogLevel() = LOG_ERROR;
    ASSERT_STREQ(UPM_LOGGER::getLogLevelName(UPM_LOGGER::LogLevel()).c_str(), "ERROR");
    UPM_LOGGER::LogLevel() = LOG_DEBUG;
    ASSERT_STREQ(UPM_LOGGER::getLogLevelName(UPM_LOGGER::LogLevel()).c_str(), "DEBUG");

    deleteLogFile(LOG_FILE);
}
