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

#include "rn2903_defs.h"

#ifdef __cplusplus
extern "C" {
#endif

    /**
     * @file rn2903.h
     * @library rn2903
     * @brief Generic API for the Microchip RN2903 LoRa Radio
     *
     */

    /**
     * Device context
     */
    typedef struct _rn2903_context {
        mraa_uart_context        uart;
        // store the baudrate
        int                      baudrate;

        // response data buffer, stripped of CR/LF
        char                     resp_data[RN2903_MAX_BUFFER];
        // length of response data
        size_t                   resp_len;

        // these are allocated buffers we use manage and reuse
        // internally to store conversions from/to hex and back
        char                     *to_hex_buf;
        char                     *from_hex_buf;

        // maximum time to wait for a response after a command is
        // submitted
        int                      cmd_resp_wait_ms;

        // maximum time to wait for a second response after a command is
        // submitted
        int                      cmd_resp2_wait_ms;

        // debugging output
        bool                     debug;

        // our hardware hex encoded EUI + terminating NULL
        char                     hardware_eui[RN2903_MAX_HEX_EUI64 + 1];

        // 16b mac status word
        uint16_t                 mac_status_word;
        // this is the mac_status bitfield of the mac status word
        RN2903_MAC_STATUS_T      mac_mac_status;
    } *rn2903_context;

    /**
     * RN2903 Initializer for UART operation using a UART index
     *
     * @param uart Specify which uart to use
     * @param baudrate Specify the baudrate to use.  57600 is the
     * default baudrate of this device.
     * @return an initialized device context on success, NULL on error
     */
    rn2903_context rn2903_init(unsigned int uart, unsigned int baudrate);

    /**
     * RN2903 Initializer for UART operation using a filesystem tty
     * path (eg. /dev/ttyUSB0)
     *
     * @param uart_tty character string representing a filesystem path to a
     * serial tty device
     * @param baudrate Specify the baudrate to use.  57600 is the
     * default baudrate of this device.
     * @return an initialized device context on success, NULL on error
     */
    rn2903_context rn2903_init_tty(const char *uart_tty, unsigned int baudrate);

    /**
     * RN2903 sensor close function
     *
     * @param dev Device context
     */
    void rn2903_close(rn2903_context dev);

    /**
     * Set the default time, in milliseconds, to wait for a response
     * after sending a command.  All commands return at least one
     * response immediately after issuing the command.  This delay
     * sets the maximum amount of time to wait for it.
     *
     * @param dev Device context
     * @param wait_ms The response delay to set, in milliseconds.
     */
    void rn2903_set_response_wait_time(const rn2903_context dev,
                                       unsigned int wait_ms);

    /**
     * Set the default time, in milliseconds, to wait for the second
     * response data to arrive.  Some commands will have a second
     * response emitted after the first response.  This delay sets the
     * maximum amount of time to wait for it.
     *
     * @param dev Device context
     * @param wait_ms The response delay to set, in milliseconds.
     */
    void rn2903_set_response2_wait_time(const rn2903_context dev,
                                        unsigned int wait_ms);

    /**
     * Determine whether there is data available to be read.  This
     * function will wait up to "millis" milliseconds for data to
     * become available.
     *
     * @param dev Device context
     * @param millis The number of milliseconds to wait for data to
     * become available
     * @return true if data is available to be read, false otherwise
     */
    bool rn2903_data_available(const rn2903_context dev,
                               unsigned int millis);

    /**
     * Read and throw away any data currently available to be read.
     * This is useful to avoid reading data that might have been the
     * result of a previous command interfering with data you
     * currently want to read.
     *
     * @param dev Device context
     */
    void rn2903_drain(const rn2903_context dev);

    /**
     * Send a command, wait for a response using
     * rn2903_waitfor_response(), and return the status.  The response
     * itself will be stored internally, and can be retrieved using
     * rm2903_get_response() and rn2903_get_response_len().  Every
     * command will return at least one response, and this function
     * will always wait for it and store it after sending the command.
     *
     * @param dev Device context
     * @param cmd A character string containing the command to
     * send
     * @return One of the RN2903_RESPONSE_T values
     */
    RN2903_RESPONSE_T rn2903_command(const rn2903_context dev,
                                     const char *cmd);

    /**
     * Build a command string with the supplied command and string
     * argument.  Then call rn2903_command() with the result, and
     * return the result.  This is just a convenience function.
     *
     * @param dev Device context
     * @param cmd A character string containing the command to
     * send
     * @param arg A string argument for the command
     * @return One of the RN2903_RESPONSE_T values
     */
    RN2903_RESPONSE_T rn2903_command_with_arg(const rn2903_context dev,
                                              const char *cmd,
                                              const char *arg);

    /**
     * Wait up to wait_ms milliseconds for a response.
     *
     * In the case of errors ("invalid_param" received, timeout
     * occurred, or other UPM specific error), this will be indicated
     * by the return value.
     *
     * Otherwise, an "ok" or other data value will not be considered
     * an error and will return RN2903_RESPONSE_OK.  The response
     * itself will be stored internally, and can be retrieved using
     * rm2903_get_response() and rn2903_get_response_len().
     *
     * NOTE: the response buffer is overwritten whenever this function
     * is called, so if there is data in there that you need to save,
     * copy it somewhere else before calling any other functions in
     * this driver to be safe.
     *
     * @param dev Device context
     * @param wait_ms The maximum number of milliseconds to wait for a
     * response.
     * @return One of the RN2903_RESPONSE_T values
     */
    RN2903_RESPONSE_T rn2903_waitfor_response(const rn2903_context dev,
                                              int wait_ms);

    /**
     * Return a pointer to a string containing the last response.  If
     * you wish to save the response for later, you will need to copy
     * it somewhere before calling rn2903_command(),
     * rn2903_command_with_arg() or rn2903_waitfor_response(), as
     * these functions will overwrite the internally stored response
     * buffer each time they are called.
     *
     * @param dev Device context
     * @return A const pointer to a string containing the last response.
     */
    const char *rn2903_get_response(const rn2903_context dev);

    /**
     * Return the length in bytes of the string containing the last
     * response.  If you wish to save the response length for later,
     * you will need to copy it somewhere before calling
     * rn2903_command() or rn2903_waitfor_respnse(), as these
     * functions will overwrite the internally stored response length
     * each time they are called.
     *
     * @param dev Device context
     * @return The length of the last response in bytes
     */
    size_t rn2903_get_response_len(const rn2903_context dev);

    /**
     * Set the MAC device EUI for LoRaWAN communications.  The device
     * EUI must be a hex encoded string of 16 bytes.  This value must
     * be set for LoRaWAN OTAA joining.
     *
     * @param dev Device context
     * @param str The 16-byte hex encoded device EUI
     * @return UPM result
     */
    upm_result_t rn2903_set_device_eui(const rn2903_context dev,
                                       const char *str);

    /**
     * Retrieve the device EUI from the device.  If this function
     * succeeds, you can then use rn2903_get_response() to get the
     * value.
     *
     * @param dev Device context
     * @return UPM result
     */
    upm_result_t rn2903_get_device_eui(const rn2903_context dev);

    /**
     * Set the MAC application EUI for LoRaWAN communications.  The
     * application EUI must be a hex encoded string of 16 bytes.  This
     * value must be set for LoRaWAN OTAA joining.
     *
     * @param dev Device context
     * @param str The 16-byte hex encoded application EUI
     * @return UPM result
     */
    upm_result_t rn2903_set_application_eui(const rn2903_context dev,
                                            const char *str);

    /**
     * Retrieve the application EUI from the device.  If this function
     * succeeds, you can then use rn2903_get_response() to get the
     * value.
     *
     * @param dev Device context
     * @return UPM result
     */
    upm_result_t rn2903_get_application_eui(const rn2903_context dev);

    /**
     * Set the MAC application key for LoRaWAN communications.  The
     * application key must be a hex encoded string of 32 bytes.  This
     * value must be set for LoRaWAN OTAA joining.
     *
     * @param dev Device context
     * @param str The 32-byte hex encoded application key
     * @return UPM result
     */
    upm_result_t rn2903_set_application_key(const rn2903_context dev,
                                            const char *str);

    /**
     * Retrieve the application key from the device.  If this function
     * succeeds, you can then use rn2903_get_response() to get the
     * value.
     *
     * @param dev Device context
     * @return UPM result
     */
    upm_result_t rn2903_get_application_key(const rn2903_context dev);

    /**
     * Set the MAC device address for LoRaWAN communications.  The
     * device address must be a hex encoded string of 8 bytes.  This
     * value must be set for LoRaWAN ABP joining.
     *
     * For OTAA joining, this value will be overwritten once the join
     * has completed, and therefore must not be set if performing an
     * OTAA join.
     *
     * @param dev Device context
     * @param str The 8-byte hex encoded device address
     * @return UPM result
     */
    upm_result_t rn2903_set_device_addr(const rn2903_context dev,
                                        const char *str);

    /**
     * Retrieve the device address from the device.  If this function
     * succeeds, you can then use rn2903_get_response() to get the
     * value.
     *
     * @param dev Device context
     * @return UPM result
     */
    upm_result_t rn2903_get_device_addr(const rn2903_context dev);

    /**
     * Set the MAC network session key for LoRaWAN communications.
     * The network session key must be a hex encoded string of 32
     * bytes.  This value must be set for LoRaWAN ABP joining.  It it
     * not possible to retrieve this key.
     *
     * For OTAA joining, this value will be overwritten once the join
     * has completed, and therefore must not be set if performing an
     * OTAA join.
     *
     * @param dev Device context
     * @param str The 32-byte hex encoded network session key
     * @return UPM result
     */
    upm_result_t rn2903_set_network_session_key(const rn2903_context dev,
                                                const char *str);

    /**
     * Set the MAC application session key for LoRaWAN communications.
     * The application session key must be a hex encoded string of 32
     * bytes.  This value must be set for LoRaWAN ABP joining.  It it
     * not possible to retrieve this key.
     *
     * For OTAA joining, this value will be overwritten once the join
     * has completed, and therefore must not be set if performing an
     * OTAA join.
     *
     * @param dev Device context
     * @param str The 32-byte hex encoded application session key
     * @return UPM result
     */
    upm_result_t rn2903_set_application_session_key(const rn2903_context dev,
                                                    const char *str);

    /**
     * Convert src into a hex byte string.  All non-command related
     * data such as keys, and payload sent to the device must be hex
     * encoded.  The buffers used for this conversion are managed
     * internally, so do not call free() on the returned pointer.  If
     * you need to keep a copy of the resulting hex string, copy it
     * somewhere before calling this function again.
     *
     * @param dev Device context
     * @param src A char pointer pointing to the byte array to encode
     * @param len The length in bytes of the data to encode
     * @return A const pointer to the resulting hex string, or NULL if
     * there was an error.
     */
    const char *rn2903_to_hex(const rn2903_context dev,
                              const char *src, int len);

    /**
     * Convert a hex byte string into an array of bytes.  The hex
     * string must have a length (not including the 0 terminator) that
     * is a multiple of two, and all characters in the string must be
     * valid hex characters.  The buffers used for this conversion are
     * managed internally, so do not call free() on the returned
     * pointer.  If you need to keep a copy of the resulting data byte
     * array, copy it somewhere before calling this function again.
     * The length of the returned data will be the length of the hex
     * source string divided by 2, with a 0 byte terminator at the end
     * in case a text string is being decoded.
     *
     * @param dev Device context
     * @return A const pointer to the resulting data string, or NULL if
     * there was an error
     */
    const char *rn2903_from_hex(const rn2903_context dev,
                                const char *src);

    /**
     * Join a LoRaWAN network.  There are two types of joins possible
     * - Over The Air Activation (OTAA) and Activation by
     * Personalization (ABP).  Each join method requires that certain
     * items have been configured first.
     *
     * For OTAA activation, the Device Extended Unique Identifier
     * (EUI), the Application EUI, and Application Key must be set.
     *
     * For ABP activation, The Device Address, Network Session Key,
     * and the Application Session Key must be set.
     *
     * @param dev Device context
     * @param type The LoRaWAN activation type, one of the
     * RN2903_JOIN_TYPE_T values
     * @return The status of the join, one of the RN2903_JOIN_STATUS_T
     * values
     */
    RN2903_JOIN_STATUS_T rn2903_join(const rn2903_context dev,
                                     RN2903_JOIN_TYPE_T type);

    /**
     * Transmit a packet in a LoRaWAN network.  You must
     * already be joined to a LoRaWAN network for this command to
     * succeed, and the MAC stack must be in the idle
     * (RN2903_MAC_STAT_IDLE) state.
     *
     * The packet payload must be a valid 0-terminated hex encoded
     * string.
     *
     * There is the possibility of receiving a downlink message after
     * transmitting a packet.  If this occurs, this function will
     * return RN2903_MAC_TX_STATUS_RX_RECEIVED, and the returned data
     * will be stored in the response buffer.  NOTE: calling pretty
     * much any function that issues commands to the device will
     * overwrite the response buffer, so save a copy of it if you need
     * it before calling other functions.
     *
     * @param dev Device context
     * @param type The type of message to send - confirmed or
     * unconfirmed.  One of the RN2903_MAC_MSG_TYPE_T values.
     * @param port An integer in the range 1-223
     * @param payload A 0-terminated, hex encoded string that makes up
     * the payload of the message
     * @return The status of the transmit request, one of the
     * RN2903_MAC_TX_STATUS_T values
     */
    RN2903_MAC_TX_STATUS_T rn2903_mac_tx(const rn2903_context dev,
                                         RN2903_MAC_MSG_TYPE_T type,
                                         int port, const char *payload);

    /**
     * Transmit a packet.  This method uses the radio directly without
     * the LoRaWAN stack running.  For this reason, you must call
     * rn2903_mac_pause() before trying to transmit using this
     * function.  You should also configure any parameters (frequency,
     * etc), before calling this function.
     *
     * @param dev Device context
     * @param payload A 0-terminated, hex encoded string that makes up
     * the payload of the message
     * @return The status of the transmit request, one of the
     * RN2903_RESPONSE_T values
     */
    RN2903_RESPONSE_T rn2903_radio_tx(const rn2903_context dev,
                                      const char *payload);

    /**
     * Receive a packet.  This method uses the radio directly without
     * the LoRaWAN stack running.  For this reason, you must call
     * rn2903_mac_pause() before trying to receive using this
     * function.  You should also configure any parameters (frequency,
     * etc) to match the transmitter before calling this function.
     *
     * @param dev Device context
     * @param window_size An integer that represents the number of
     * symbols to wait for (lora) or the maximum number of
     * milliseconds to wait (fsk).  This parameter is passed to the "radio
     * rx" command.  Passing 0 causes the radio to enter continuous
     * receive mode which will return when either a packet is
     * received, or the radio watchdog timer expires.  See the RN2903
     * Command Reference for details.
     * @return The status of the transmit request, one of the
     * RN2903_RESPONSE_T values
     */
    RN2903_RESPONSE_T rn2903_radio_rx(const rn2903_context dev,
                                      int window_size);

    /**
     * Return the Hardware Extended Unique Identifier.  The is a 16
     * byte hex encoded string representing the 64b hardware EUI.
     * This value cannot be changed, and is globally unique to each
     * device.  It is obtained from the device at initialization time.
     *
     * @param dev Device context
     * @return A const string pointer to the hex encoded Hardware EUI
     */
    const char *rn2903_get_hardware_eui(const rn2903_context dev);

    /**
     * Retrieve the device MAC status, decode it, and store it
     * internally.  This function must be called prior to calling
     * rn2903_get_mac_status_word() or rn2903_get_mac_status().
     *
     * @param dev Device context
     * @return UPM result
     */
    upm_result_t rn2903_update_mac_status(const rn2903_context dev);

    /**
     * Retrieve the MAC status word. rn2903_update_mac_status() must
     * have been called prior to calling this function.
     *
     * @param dev Device context
     * @return The MAC status word.  This is a bitmask of
     * RN2903_MAC_STATUS_BITS_T bits.
     */
    uint16_t rn2903_get_mac_status_word(const rn2903_context dev);

    /**
     * Retrieve the MAC status. rn2903_update_mac_status() must have
     * been called prior to calling this function.  The MAC status is
     * a bitfield embedded in the mac_status_word.  It provides
     * information on the status of the internal MAC state machine.
     *
     * @param dev Device context
     * @return The MAC status, one of the RN2903_MAC_STATUS_T values.
     */
    RN2903_MAC_STATUS_T rn2903_get_mac_status(const rn2903_context dev);

    /**
     * Save the configurable device values to EEPROM.  These values
     * will be reloaded automatically on a reset or power up.
     *
     * The data that can be saved are: deveui, appeui, appkey, nwkskey
     * (Network Session Key), appskey, devaddr, channels, upctr
     * (Uplink Counter), dnctr (Downlink Counter), adr (automatic
     * data-rate) state, and rx2 (the RX2 receive window parameters).
     *
     * @param dev Device context
     * @return UPM result
     */
    upm_result_t rn2903_mac_save(const rn2903_context dev);

    /**
     * Pause the MAC LoRaWAN stack.  This device can operate in one of
     * two modes.
     *
     * The most common mode is used to join and participate in a LoRaWAN
     * network as a Class A End Device.  This is handled by the MAC
     * LoRaWAN stack on the device dealing with the details of
     * LoRaWAN participation automatically.
     *
     * The other mode disables MAC LoRaWAN stack functionality and
     * allows you to issue commands directly to the radio to set
     * frequencies, data rates, modulation and many other parameters.
     *
     * Calling this function disables the MAC LoRaWAN stack and allows
     * you to issue radio commands that are otherwise handled
     * automatically.
     *
     * When pausing, the maximum allowable pause time in milliseconds
     * will be returned in the response buffer.  You can grab this
     * value by calling rn2903_get_response() after this function
     * completes successfully.
     *
     * When the MAC is idle (rn2903_get_mac_status()), you can pause
     * indefinitely.
     *
     * @param dev Device context
     * @return UPM result
     */
    upm_result_t rn2903_mac_pause(const rn2903_context dev);

    /**
     * Resume the MAC LoRaWAN stack.  Call this to resume MAC LoRaWAN
     * operation after having called rn2903_mac_pause(), to resume
     * participation in a LoRaWAN network.
     *
     * @param dev Device context
     * @return UPM result
     */
    upm_result_t rn2903_mac_resume(const rn2903_context dev);

    /**
     * Reset the device.  Any configuration is lost, as well as
     * the current join status.  This method also calls
     * rn2903_set_baudrate() after the reset to re-establish
     * communications with the device in the event you are not
     * using the default baudrate (which the device will revert to
     * after a reset).
     *
     * @param dev Device context
     * @return UPM result
     */
    upm_result_t rn2903_reset(const rn2903_context dev);

    /**
     * LoRaWAN communications allows for the reporting of current
     * battery charge remaining to the LoRaWAN gateway/network server.
     * This function allows you to specify the value that should be
     * reported.
     *
     * The valid values are from 0 to 255.
     * 0 = using external power
     * 1(low) to 254(high) = battery power
     * 255 = unable to measure battery level
     *
     * @param dev Device context
     * @param level The battery level value from 0-255
     * @return UPM result
     */
    upm_result_t rn2903_mac_set_battery(const rn2903_context dev, int level);

    /**
     * Enable debugging.  If enabled, commands will be printed out
     * before being sent to the device.  Any responses will be printed
     * out after retrieval.  Other miscellaneous debug output will
     * also be printed.
     *
     * @param dev Device context
     * @param enable true to enable debugging, false otherwise
     */
    void rn2903_set_debug(const rn2903_context dev, bool enable);

    /**
     * Read character data from the device
     *
     * @param dev Device context
     * @param buffer The character buffer to read data into
     * @param len The maximum size of the buffer
     * @return The number of bytes successfully read, or -1 on error
     */
    int rn2903_read(const rn2903_context dev, char *buffer, size_t len);

    /**
     * Write character data to the device
     *
     * @param dev Device context
     * @param buffer The character buffer containing data to write
     * @param len The number of bytes to write
     * @return The number of bytes successfully written, or -1 on error
     */
    int rn2903_write(const rn2903_context dev, const char *buffer, size_t len);

    /**
     * Set the baudrate of the device.  Auto-bauding is currently only
     * supported on Linux (due to the need to send a break signal) and
     * only on a recent MRAA which supports it (> 1.6.1).  If on a
     * non-linux OS, you should not try to change the baudrate to
     * anything other than the default 57600 or you will lose control
     * of the device.
     *
     * @param dev Device context
     * @param baudrate The baud rate to set for the device
     * @return UPM result
     */
    upm_result_t rn2903_set_baudrate(const rn2903_context dev,
                                     unsigned int baudrate);

    /**
     * Set a flow control method for the UART.  By default, during
     * initialization, flow control is disabled.  The device MAY
     * support hardware flow control, but MRAA does not (at least for
     * UART numbers), so we can't either.  We leave the option here
     * though so that if you are using a TTY (as opposed to a UART
     * instance) it might work if the device is also configured to use
     * hardware flow control.
     *
     * @param dev Device context
     * @param fc One of the RN2903_FLOW_CONTROL_T values
     * @return the UPM result
     */
    upm_result_t rn2903_set_flow_control(const rn2903_context dev,
                                         RN2903_FLOW_CONTROL_T fc);

    /**
     * This is a utility function that can be used to indicate if a
     * given string is present at the beginning of the response
     * buffer.  The search is case sensitive.
     *
     * @param dev Device context
     * @param str The 0 teminated string to search for
     * @return true if the string was found at the beginning of the
     * response buffer, false otherwise
     */
    bool rn2903_find(const rn2903_context dev, const char *str);

    /**
     * This is a utility function that can be used to return a pointer
     * to the location in the response buffer where the hex encoded
     * payload starts for radio_rx messages received.
     *
     * @param dev Device context
     * @return A pointer to the start of the hex payload in the
     * response buffer, or NULL if the response buffer does not
     * contain a radio_rx sentence.
     */
    const char *rn2903_get_radio_rx_payload(const rn2903_context dev);

    /**
     * This function attempts to sync the device to the current
     * baudrate.  It tries retries times, to send an autobaud
     * sequence to the device and run a test command.
     *
     * @param dev Device context
     * @param retries The number of times to retry autobaud detection
     * @return true if the test command succeeded, false otherwise
     */
    bool rn2903_autobaud(const rn2903_context dev, int retries);

#ifdef __cplusplus
}
#endif
