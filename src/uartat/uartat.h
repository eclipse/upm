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

#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>

#include <upm.h>
#include <mraa/uart.h>

#include "uartat_defs.h"

#ifdef __cplusplus
extern "C" {
#endif

    /**
     * @file uartat.h
     * @library uartat
     * @brief Generic API for AT command based UART devices
     *
     */

    /**
     * Device context
     */
    typedef struct _uartat_context {
        mraa_uart_context        uart;

        // wait time for reading results after sending a command.  The
        // default is 250ms.
        unsigned int cmd_resp_wait_ms;

        // filter carriage returns (CR) out of responses?
        bool filter_cr;
    } *uartat_context;

    /**
     * UARTAT Initializer for generic UART operation using a UART index.
     *
     * @param uart Specify which uart to use.
     * @param baudrate Specify the baudrate to use.
     * @return an initialized device context on success, NULL on error.
     */
    uartat_context uartat_init(unsigned int uart, unsigned int baudrate);

    /**
     * UARTAT Initializer for generic UART operation using a filesystem
     * tty path (eg. /dev/ttyUSB0).
     *
     * @param uart_tty character string representing a filesystem path to a
     * serial tty device.
     * @param baudrate Specify the baudrate to use.
     * @return an initialized device context on success, NULL on error.
     */
    uartat_context uartat_init_tty(const char *uart_tty, unsigned int baudrate);

    /**
     * UARTAT sensor close function
     *
     * @param dev Device context
     */
    void uartat_close(uartat_context dev);

    /**
     * Read character data from the device.
     *
     * @param dev Device context
     * @param buffer The character buffer to read data into.
     * @param len The maximum size of the buffer
     * @return The number of bytes successfully read, or -1 on error
     */
    int uartat_read(const uartat_context dev, char *buffer, size_t len);

    /**
     * Write character data to the device.
     *
     * @param dev Device context
     * @param buffer The character buffer containing data to write.
     * @param len The number of bytes to write.
     * @return The number of bytes successfully written, or -1 on error.
     */
    int uartat_write(const uartat_context dev, const char *buffer, size_t len);

    /**
     * Set the baudrate of the device.
     *
     * @param dev Device context
     * @param baudrate The baud rate to set for the device.
     * @return UPM result
     */
    upm_result_t uartat_set_baudrate(const uartat_context dev,
                                     unsigned int baudrate);

    /**
     * Set the default time, in milliseconds, to wait for data to
     * arrive after sending a command.
     *
     * @param dev Device context
     * @param wait_ms The response delay to set, in milliseconds.
     */
    void uartat_set_response_wait_time(const uartat_context dev,
                                       unsigned int wait_ms);

    /**
     * Determine whether there is data available to be read.  This
     * function will wait up to "millis" milliseconds for data to
     * become available.
     *
     * @param dev Device context
     * @param millis The number of milliseconds to wait for data to
     * become available.
     * @return true if data is available to be read, false otherwise.
     */
    bool uartat_data_available(const uartat_context dev,
                               unsigned int millis);

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
     * @param dev Device context
     * @param cmd_chars The character sequence to write, typically "+++".
     * @param guard_ms The number of milliseconds to delay before and
     * after the cmd_chars are written.
     * @return true if AT command mode ("OK" detected) was
     * successfully entered, false otherwise.
     */
    bool uartat_command_mode(const uartat_context dev, const char *cmd_chars,
                             unsigned int guard_ms);

    /**
     * Check to see if the device is in command mode.  This is
     * accomplished by sending an "AT\r" command and seeing if "OK" or
     * "0" is returned.
     *
     * @param dev Device context
     * @return true if AT command mode was detected, false otherwise.
     */
    bool uartat_in_command_mode(const uartat_context dev);

    /**
     * Read and throw away any data currently available to be read.
     * This is useful to avoid reading data that might have been the
     * result of a previous command interfering with data you
     * currently want to read.  This function is automatically called
     * by uartat_command_with_response(), uartat_command(), and
     * uartat_command_waitfor() prior to writing the requested command
     * to the device.
     *
     * @param dev Device context
     */
    void uartat_drain(const uartat_context dev);

    /**
     * Send an AT command and optionally return a response.
     *
     * @param dev Device context
     * @param cmd A character string containing the AT command to
     * send, including the "AT" prefix and a terminating carriage
     * return ("\r").
     * @param resp A pointer to a buffer that will contain the
     * response.  If NULL is specified, the response is ignored.  The
     * returned string buffer will be 0 terminated like any ordinary C
     * string.
     * @param resp_len The length of the supplied response buffer.  If
     * 0, then any response will be ignored.  No more than resp_len
     * characters (including the trailing 0 byte) will be returned.
     * @return The number of bytes read, or -1 on error.
     */
    int uartat_command_with_response(const uartat_context dev,
                                     const char *cmd, char *resp,
                                     size_t resp_len);

    /**
     * Send an AT command and ignore any response.  This is a
     * shorthand version of uartat_command_with_response(), and is
     * equivalent to calling uartat_command_with_response(dev, cmd,
     * NULL, 0).
     *
     * @param dev Device context
     * @param cmd The AT command to send, including the "AT" prefix
     * and a terminating carriage return ("\r").
     */
    void uartat_command(const uartat_context dev, const char *cmd);

    /**
     * Read characters for up to millis milliseconds, returning
     * as soon as the wait_string is found.
     *
     * @param dev Device context
     * @param cmd The command to send
     * @param resp The response character buffer
     * @param resp_len The maximum size of the response buffer
     * @param wait_string The string to search for
     * @param millis The maximum number of milliseconds to look for
     * the wait_string.
     * @return true if the wait_string was found in the response,
     * false otherwise.
     */
    bool uartat_command_waitfor(const uartat_context dev, const char *cmd,
                                char *resp, size_t resp_len,
                                const char *wait_string,
                                unsigned int millis);

    /**
     * Set a flow control method for the UART.  By default, during
     * initialization, flow control is disabled.
     *
     * @param dev Device context
     * @param fc One of the UARTAT_FLOW_CONTROL_T values.
     * @return the UPM result.
     */
    upm_result_t uartat_set_flow_control(const uartat_context dev,
                                         UARTAT_FLOW_CONTROL_T fc);

    /**
     * Look for a string in a buffer.  This is a utility function that
     * can be used to indicate if a given string is present in a
     * supplied buffer.  The search is case sensitive.
     *
     * @param dev Device context
     * @param buffer The 0 teminated buffer in which to search.
     * @param str The 0 teminated string to search for.
     * @return true if the string was found, false otherwise.
     */
    bool uartat_find(const uartat_context dev, const char *buffer,
                     const char *str);

    /**
     * Filter out carriage returns (CR) from response buffers if
     * enabled.  This operates only on the response buffers returned
     * from uartat_command_with_response(), uartat_command(), and
     * uartat_command_waitfor().
     *
     * @param dev Device context
     * @param enable true to filter out CR's, false otherwise
     */
    void uartat_filter_cr(const uartat_context dev, bool enable);

#ifdef __cplusplus
}
#endif
