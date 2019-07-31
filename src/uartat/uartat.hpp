/*
 * Author: Jon Trulson <jtrulson@ics.com>
 * Copyright (c) 2017 Intel Corporation.
 *
 * This program and the accompanying materials are made available under the
 * terms of the The MIT License which is available at
 * https://opensource.org/licenses/MIT.
 *
 * SPDX-License-Identifier: MIT
 */
#pragma once

#include <string>
#include <iostream>

#include "uartat.h"

namespace upm {
    /**
     * @brief Generic AT Command Based UART Modem Library
     * @defgroup uartat libupm-uartat
     * @ingroup uart
     */

    /**
     * @library uartat
     * @sensor uartat
     * @comname Generic AT Command Based UART Device
     * @type other
     * @con uart
     *
     * @brief API for a Generic AT command based UART device
     *
     * This is a generic UART device driver for accessing UART based
     * devices that utilize an "AT" command set.  Typically these
     * devices are Radios, Modems, and similar devices that are
     * configured and controlled by emitting "AT" commands.
     *
     * @snippet uartat-le910.cxx Interesting
     */

    class UARTAT {
    public:

        /**
         * UARTAT object constructor for a UART specified by MRAA number.
         *
         * @param uart Specify which uart to use.
         * @param baudrate Specify the baudrate to use.
         */
        UARTAT(unsigned int uart, unsigned int baudrate);

        /**
         * UARTAT object constructor for a UART specified by PATH (ex:
         * /dev/ttyUSB0)
         *
         * @param uart_path Specify path of UART device.
         * @param baudrate Specify the baudrate to use.
         */
        UARTAT(std::string uart_path, unsigned int baudrate);

        /**
         * UARTAT object destructor
         */
        ~UARTAT();

        /**
         * Read character data from the device.
         *
         * @param size The maximum number of characters to read.
         * @return string containing the data read.
         */
        std::string readStr(size_t size);

        /**
         * Write character data to the device.
         *
         * @param buffer The string containing the data to write.
         * @return The number of bytes written.
         */
        int writeStr(std::string buffer);

        /**
         * Set the baudrate of the device.
         *
         * @param baudrate The baud rate to set for the device.
         */
        void setBaudrate(unsigned int baudrate);

        /**
         * Set the default time, in milliseconds, to wait for data to
         * arrive after sending a command.
         *
         * @param wait_time The response delay to set, in milliseconds.
         */
        void setResponseWaitTime(unsigned int wait_time);

        /**
         * Determine whether there is data available to be read.  In the
         * case of a UART, this function will wait up to "millis"
         * milliseconds for data to become available.  In the case of an I2C
         * device, the millis argument is ignored and the function will
         * return immediately, indicating whether data is available.
         *
         * @param millis The number of milliseconds to wait for data to
         * become available.
         * @return true if data is available to be read, false otherwise.
         */
        bool dataAvailable(unsigned int millis);

        /**
         * Place the device in AT command mode.  Many devices operate in a
         * transparent mode and an AT command mode.  Command mode is
         * required to issue AT based commands.  When in transparent mode,
         * the device will usually listen for a special sequence of
         * characters and delays, indicating that AT command mode should
         * be entered.
         *
         * On most devices, the sequence is:
         * <wait 1 second>+++<wait 1 second>
         *
         * For most devices, the wait time is 1 second (1000 ms) and the
         * character sequence is "+++".  These options can often be
         * configured on the device.
         *
         * This function will wait millis milliseconds, write the command
         * characters (typically "+++"), then wait millis milliseconds again.
         * At this time a read will be attempted, looking for the "OK"
         * response indicating command mode was successfully entered.
         *
         * @param cmd_chars The character sequence to write, typically "+++".
         * @param guard_ms The number of milliseconds to delay before and
         * after the cmd_chars are written.
         * @return true if AT command mode ("OK" detected) was
         * successfully entered, false otherwise.
         */
        bool commandMode(const std::string cmd_chars, unsigned int guard_ms);

        /**
         * Check to see if the device is in command mode.  This is
         * accomplished by sending an "AT\r" command and seeing if
         * "OK" or "0" is returned.
         *
         * @return true if AT command mode was detected, false otherwise.
         */
        bool inCommandMode();

        /**
         * Read and throw away any data currently available to be
         * read.  This is useful to avoid reading data that might have
         * been the result of a previous command interfering with data
         * you currently want to read.  This function is automatically
         * called by commandWithResponse(), command(), and
         * commandWaitfor() prior to writing the requested command to
         * the device.
         *
         */
        void drain();

        /**
         * Send an AT command and optionally return a response.
         *
         * @param cmd A character string containing the AT command to
         * send, including the "AT" prefix and a terminating carriage
         * return ("\r").
         * @param resp_len The maximum number of characters to read from the
         * device.
         * @return The device response string, if any.
         */
        std::string commandWithResponse(const std::string cmd, size_t resp_len);

        /**
         * Send an AT command and return a response, while waiting for
         * a specific string.  If the string isn't found the returned
         * string will be empty.  If the string is found, the function
         * will return immediately.
         *
         * @param cmd A character string containing the AT command to
         * send, including the "AT" prefix and a terminating carriage
         * return ("\r").
         * @param resp_len The maximum number of characters to read from the
         * device.
         * @param waitString The string to look for.  If found, the
         * response will be returned immediately regardless of the
         * timeout setting.
         * @param millis The maximum number of milliseconds to wait
         * for the string.
         * @return A string containing the response if the search
         * string was found, otherwise and empty string is returned.
         */
        std::string commandWaitFor(const std::string cmd, size_t resp_len,
                                   const std::string waitString,
                                   unsigned int millis);

        /**
         * Send an AT command and ignore any response.
         *
         * @param cmd The AT command to send, including the "AT" prefix
         * and a terminating carriage return ("\r").
         */
        void command(const std::string cmd);

        /**
         * This is a convenience method that converts each CR (\r) in a
         * string to a LF (\n) and returns it.  This is useful for
         * outputting the response to an AT command for instance, which is
         * often CR terminated.
         *
         * @param str The string to convert
         * @return The converted string
         */
        std::string stringCR2LF(std::string str);

        /**
         * Set a flow control method for the UART.  By default, during
         * initialization, flow control is disabled.
         *
         * @param fc One of the UARTAT_FLOW_CONTROL_T values.
         */
        void setFlowControl(UARTAT_FLOW_CONTROL_T fc);

        /**
         * Look for a string in a buffer.  This is a utility function that
         * can be used to indicate if a given string is present in a
         * supplied buffer.  The search is case sensitive.
         *
         * @param buffer The string buffer in which to search.
         * @param str The string to search for.
         * @return true if the string was found, false otherwise.
         */
        bool find(const std::string buffer, const std::string str);

        /**
         * Filter out carriage returns (CR) from response buffers if
         * enabled.  This operates only on the response buffers returned
         * from commandWithResponse(), command(), and
         * commandWaitfor().
         *
         * @param enable true to filter out CR's, false otherwise
         */
        void filterCR(bool enable);

    protected:
        // uartat device context
        uartat_context m_uartat;

    private:
        /* Disable implicit copy and assignment operators */
        UARTAT(const UARTAT&) = delete;
        UARTAT &operator=(const UARTAT&) = delete;
    };
}
