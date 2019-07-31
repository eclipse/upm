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

#include "rn2903.h"

namespace upm {
    /**
     * @brief Microchip RN2903 LoRa Radio
     * @defgroup rn2903 libupm-rn2903
     * @ingroup uart wifi
     */

    /**
     * @library rn2903
     * @sensor rn2903
     * @comname Microchip RN2903 LoRa radio
     * @type wifi
     * @con uart
     *
     * @brief API for the Microchip RN2903 LoRa radio
     *
     * The Microchip RN2903 Low-Power Long Range LoRa Technology
     * Transceiver module provides an easy to use, low-power solution
     * for long range wireless data transmission. The advanced command
     * interface offers rapid time to market.
     *
     * The RN2903 module complies with the LoRaWAN Class A protocol
     * specifications. It integrates RF, a baseband controller, and a
     * command Application Programming Interface (API) processor,
     * making it a complete long range solution.
     *
     * Most of the effort in this driver has been geared to supporting
     * operation within a LoRaWAN network, however methods are
     * provided to allow you to control the device directly so that
     * you can implement whatever solution you require.
     *
     * @snippet rn2903.cxx Interesting
     * @snippet rn2903-p2p-tx.cxx Interesting
     * @snippet rn2903-p2p-rx.cxx Interesting
     */

    class RN2903 {
    public:

        /**
         * RN2903 object constructor for a UART specified by MRAA number.
         *
         * @param uart Specify which uart to use.
         * @param baudrate Specify the baudrate to use.  The default
         * is 57600.
         */
        RN2903(unsigned int uart,
               unsigned int baudrate=RN2903_DEFAULT_BAUDRATE);

        /**
         * RN2903 object constructor for a UART specified by PATH (ex:
         * /dev/ttyUSB0)
         *
         * @param uart_path Specify path of UART device.
         * @param baudrate Specify the baudrate to use.  The default
         * is 57600.
         */
        RN2903(std::string uart_path,
               unsigned int baudrate=RN2903_DEFAULT_BAUDRATE);

        /**
         * RN2903 object destructor
         */
        virtual ~RN2903();

        /**
         * Set the default time, in milliseconds, to wait for a response
         * after sending a command.  All commands return at least one
         * response immediately after issuing the command.  This delay
         * sets the maximum amount of time to wait for it.
         *
         * @param wait_ms The response delay to set, in milliseconds.
         * The default is 1 second (1000ms).
         */
        void setResponseWaitTime(
            unsigned int wait_ms=RN2903_DEFAULT_RESP_DELAY);

        /**
         * Set the default time, in milliseconds, to wait for the second
         * response data to arrive.  Some commands will have a second
         * response emitted after the first response.  This delay sets the
         * maximum amount of time to wait for it.
         *
         * @param wait_ms The response delay to set, in milliseconds.
         * The default is 60 seconds (60000ms).
         */
        void setResponse2WaitTime(
            unsigned int wait_ms=RN2903_DEFAULT_RESP2_DELAY);

        /**
         * Determine whether there is data available to be read.  This
         * function will wait up to "millis" milliseconds for data to
         * become available.
         *
         * @param millis The number of milliseconds to wait for data to
         * become available.
         * @return true if data is available to be read, false otherwise.
         */
        bool dataAvailable(unsigned int millis);

        /**
         * Read and throw away any data currently available to be
         * read.  This is useful to avoid reading data that might have
         * been the result of a previous command interfering with data
         * you currently want to read.
         */
        void drain();

        /**
         * Send a command, wait for a response using
         * waitForResponse(), and return the status.  The response
         * itself will be stored internally, and can be retrieved
         * using getResponse() and getResponseLen().  Every command
         * will return at least one response, and this function will
         * always wait for it and store it into the internal response
         * buffer after sending the command.
         *
         * @param cmd A character string containing the command to
         * send
         * @return One of the RN2903_RESPONSE_T values
         */
        RN2903_RESPONSE_T command(const std::string cmd);

        /**
         * Build a command string with the supplied command and string
         * argument.  Then call command() with the result, and
         * return the result.  This is just a convenience function.
         *
         * @param cmd A character string containing the command to
         * send
         * @param arg A string argument for the command
         * @return One of the RN2903_RESPONSE_T values
         */
        RN2903_RESPONSE_T commandWithArg(const std::string cmd,
                                         const std::string arg);

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
         * getResponse() and getResponseLen().
         *
         * NOTE: the response buffer is overwritten whenever this function
         * is called, so if there is data in there that you need to save,
         * copy it somewhere else before calling any other methods in
         * this driver to be safe.
         *
         * @param wait_ms The maximum number of milliseconds to wait for a
         * response.
         * @return One of the RN2903_RESPONSE_T values
         */
        RN2903_RESPONSE_T waitForResponse(int wait_ms);

        /**
         * Return a string containing a copy of the last response
         * saved in the response buffer
         *
         * @return A string containing a copy of the response buffer
         */
        std::string getResponse();

        /**
         * Return the length in bytes of the string containing the
         * last response.
         *
         * @return The length of the last response in bytes
         */
        int getResponseLen();

        /**
         * Set the MAC device EUI for LoRaWAN communications.  The device
         * EUI must be a hex encoded string of 16 bytes.  This value must
         * be set for LoRaWAN OTAA joining.
         *
         * @param str The 16-byte hex encoded device EUI
         * @throws std::runtime_error if the EUI is invalid or the mac
         * set command failed
         */
        void setDeviceEUI(const std::string str);

        /**
         * Retrieve the device EUI from the device.  If this function
         * succeeds, you can then use getResponse() to get the value.
         *
         * @return UPM result
         * @throws std::runtime_error if the mac get command failed
         */
        void getDeviceEUI();

        /**
         * Set the MAC device address for LoRaWAN communications.  The
         * device address must be a hex encoded string of 8 bytes.  This
         * value must be set for LoRaWAN ABP joining.
         *
         * For OTAA joining, this value will be overwritten once the join
         * has completed, and therefore must not be set if performing an
         * OTAA join.
         *
         * @param str The 8-byte hex encoded device address
         * @throws std::runtime_error if the mac set command failed or
         * the hex string is invalid
         */
        void setDeviceAddr(std::string str);

        /**
         * Retrieve the device address from the device.  If this
         * function succeeds, you can then use getResponse() to get
         * the value.
         *
         * @throws std::runtime_error if the mac get failed
         */
        void getDeviceAddr();

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
         * @param str The 32-byte hex encoded network session key
         * @throws std::runtime_error if the mac set command failed or
         * the hex string is invalid
         */
        void setNetworkSessionKey(std::string str);

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
         * @param str The 32-byte hex encoded application session key
         * @throws std::runtime_error if the mac set command failed or
         * the hex string is invalid
         */
        void setApplicationSessionKey(std::string str);

        /**
         * Set the MAC application EUI for LoRaWAN communications.  The
         * application EUI must be a hex encoded string of 16 bytes.  This
         * value must be set for LoRaWAN OTAA joining.
         *
         * @param str The 16-byte hex encoded application EUI
         * @throws std::runtime_error if the EUI is invalid, or if the
         * mac set command failed
         */
        void setApplicationEUI(const std::string str);

        /**
         * Retrieve the application EUI from the device.  If this
         * function succeeds, you can then use getResponse() to get
         * the value.
         *
         * @throws std::runtime_error if the mac get command failed
         */
        void getApplicationEUI();

        /**
         * Set the MAC application key for LoRaWAN communications.
         * The application key must be a hex encoded string of 32
         * bytes.  This value must be set for LoRaWAN OTAA joining.
         *
         * @param str The 32-byte hex encoded application key
         * @throws std::runtime_error if the key is invalid, or if the
         * mac set command failed
         */
        void setApplicationKey(const std::string str);

        /**
         * Retrieve the application key from the device.  If this function
         * succeeds, you can then use rn2903_get_response() to get the
         * value.
         *
         * @throws std::runtime_error if the mac get command failed
         */
        void getApplicationKey();

        /**
         * Convert src into a hex byte string.  All non-command
         * related data such as keys, and payload sent to the device
         * must be hex encoded.
         *
         * @param src A string to encode
         * @return A string containing the hex encoded version of str.
         * In the event of an error, the return string will be empty.
         */
        std::string toHex(const std::string src);

        /**
         * Decode a hex byte string into the original string.  The hex
         * string must have a length that is a multiple of two, and
         * all characters in the string must be valid hex characters.
         *
         * @return A string containing the decoded contents of the hex
         * string passed, or an empty string if there was an error.
         */
        std::string fromHex(const std::string src);

        /**
         * Join a LoRaWAN network.  There are two types of joins
         * possible - Over The Air Activation (OTAA) and Activation by
         * Personalization (ABP).  Each join method requires that
         * certain items have been configured first.
         *
         * For OTAA activation, the Device Extended Unique Identifier
         * (EUI), the Application EUI, and Application Key must be
         * set.
         *
         * For ABP activation, The Device Address, Network Session
         * Key, and the Application Session Key must be set.
         *
         * @param type The LoRaWAN join activation type, one of the
         * RN2903_JOIN_TYPE_T values
         * @return The status of the join, one of the RN2903_JOIN_STATUS_T
         * values
         */
        RN2903_JOIN_STATUS_T join(RN2903_JOIN_TYPE_T type);

        /**
         * Transmit a packet in a LoRaWAN network.  You must
         * already be joined to a LoRaWAN network for this command to
         * succeed, and the MAC stack must be in the idle
         * (RN2903_MAC_STAT_IDLE) state.
         *
         * The packet payload must be a valid hex encoded string.
         *
         * There is the possibility of receiving a downlink message after
         * transmitting a packet.  If this occurs, this function will
         * return RN2903_MAC_TX_STATUS_RX_RECEIVED, and the returned data
         * will be stored in the response buffer.
         *
         * @param type The type of message to send - confirmed or
         * unconfirmed.  One of the RN2903_MAC_MSG_TYPE_T values.
         * @param port An integer in the range 1-223
         * @param payload A valid hex encoded string that makes up the
         * payload of the message
         * @return The status of the transmit request, one of the
         * RN2903_MAC_TX_STATUS_T values
         */
        RN2903_MAC_TX_STATUS_T macTx(RN2903_MAC_MSG_TYPE_T type,
                                     int port, std::string payload);

        /**
         * Transmit a packet.  This method uses the radio directly
         * without the LoRaWAN stack running.  For this reason, you
         * must call macPause() before trying to transmit using this
         * function.  You should also configure any radio parameters
         * (frequency, etc), before calling this function.
         *
         * @param payload A valid hex encoded string that makes up
         * the payload of the message
         * @return The status of the transmit request, one of the
         * RN2903_RESPONSE_T values
         */
        RN2903_RESPONSE_T radioTx(const std::string payload);

        /**
         * Receive a packet.  This method uses the radio directly
         * without the LoRaWAN stack running.  For this reason, you
         * must call macPause() before trying to receive using this
         * function.  You should also configure any parameters
         * (frequency, etc) to match the transmitter before calling
         * this function.
         *
         * @param window_size An integer that represents the number of
         * symbols to wait for (lora) or the maximum number of
         * milliseconds to wait (fsk).  This parameter is passed to
         * the "radio rx" command.  Passing 0 causes the radio to
         * enter continuous receive mode which will return when either
         * a packet is received, or the radio watchdog timer expires.
         * See the RN2903 Command Reference for details.
         * @return The status of the transmit request, one of the
         * RN2903_RESPONSE_T values
         */
        RN2903_RESPONSE_T radioRx(int window_size);

        /**
         * Return the Hardware Extended Unique Identifier (EUI).  The
         * is a 16 byte hex encoded string representing the 64b
         * hardware EUI.  This value cannot be changed, and is
         * globally unique to each device.  It is obtained from the
         * device at initialization time.
         *
         * @return A const string pointer to the hex encoded Hardware EUI
         */
        std::string getHardwareEUI();

        /**
         * Retrieve the device MAC status, decode it, and store it
         * internally.  This function must be called prior to calling
         * getMacStatusWord() or getMacStatus().
         *
         * @throws std::runtime_error if the mac get command failed
         */
        void updateMacStatus();

        /**
         * Retrieve the MAC status word. updateMacStatus() must have
         * been called prior to calling this function.
         *
         * @return The MAC status word.  This is a bitmask of
         * RN2903_MAC_STATUS_BITS_T bits.
         */
        int getMacStatusWord();

        /**
         * Retrieve the MAC status. updateMacStatus() must have been
         * called prior to calling this function.  The MAC status is a
         * bitfield embedded in the mac status word.  It provides
         * information on the status of the internal MAC state
         * machine.
         *
         * @return The MAC status, one of the RN2903_MAC_STATUS_T values.
         */
        RN2903_MAC_STATUS_T getMacStatus();

        /**
         * Save the configurable device values to EEPROM.  These
         * values will be reloaded automatically on a reset or power
         * up.
         *
         * The data that can be saved are: deveui, appeui, appkey,
         * nwkskey (Network Session Key), appskey, devaddr, channels,
         * upctr (Uplink Counter), dnctr (Downlink Counter), adr
         * (automatic data-rate) state, and rx2 (the RX2 receive
         * window parameters).
         *
         * @throws std::runtime_error if the mac save command failed
         */
        void macSave();

        /**
         * Pause the MAC LoRaWAN stack.  This device can operate in
         * one of two modes.
         *
         * The most common mode is used to join and participate in a
         * LoRaWAN network as a Class A End Device.  This is handled
         * by the MAC LoRaWAN stack on the device dealing with the
         * details of LoRaWAN participation automatically.
         *
         * The other mode disables MAC LoRaWAN stack functionality and
         * allows you to issue commands directly to the radio to set
         * frequencies, data rates, modulation and many other
         * parameters.
         *
         * Calling this function disables the MAC LoRaWAN stack and
         * allows you to issue radio commands that are otherwise
         * handled automatically.
         *
         * When pausing, the maximum allowable pause time in
         * milliseconds will be returned in the response buffer.  You
         * can grab this value by calling getResponse() after this
         * function completes successfully.
         *
         * When the MAC is idle (getMacStatus()), you can pause
         * the stack indefinitely.
         *
         * @throws std::runtime_error if the mac pause command failed
         */
        void macPause();

        /**
         * Resume the MAC LoRaWAN stack.  Call this to resume MAC
         * LoRaWAN operation after having called macPause(), to resume
         * participation in a LoRaWAN network.
         *
         * @param dev Device context
         * @return UPM result
         * @throws std::runtime_error if the mac resume command failed
         */
        void macResume();

        /**
         * Reset the device.  Any configuration is lost, as well as
         * the current join status.  This method also calls
         * setBaudrate() after the reset to re-establish
         * communications with the device in the event you are not
         * using the default baudrate (which the device will revert to
         * after a reset).
         *
         * @throws std::runtime_error if the mac reset, or
         * setBaudrate() command fails
         */
        void reset();

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
         * @param level The battery level value from 0-255
         * @throws std::runtime_error if the mac set bat command
         * failed, or if the battery level is invalid
         */
        void macSetBattery(int level);

        /**
         * Enable debugging.  If enabled, commands will be printed out
         * before being sent to the device.  Any responses will be printed
         * out after retrieval.  Other miscellaneous debug output will
         * also be printed.
         *
         * @param enable true to enable debugging, false otherwise
         */
        void setDebug(bool enable);

        /**
         * Set the baudrate of the device.  Auto-bauding is currently only
         * supported on Linux (due to the need to send a break signal) and
         * only on a recent MRAA which supports it (> 1.6.1).  If on a
         * non-linux OS, you should not try to change the baudrate to
         * anything other than the default 57600 or you will lose control
         * of the device.
         *
         * @param baudrate The baud rate to set for the device
         * @throws std::runtime_error if the autobaud test command failed
         */
        void setBaudrate(unsigned int baudrate);

        /**
         * Set a flow control method for the UART.  By default, during
         * initialization, flow control is disabled.  The device MAY
         * support hardware flow control, but MRAA does not (at least for
         * UART numbers), so we can't either.  We leave the option here
         * though so that if you are using a TTY (as opposed to a UART
         * instance) it might work if the device is also configured to use
         * hardware flow control.
         *
         * @param fc One of the RN2903_FLOW_CONTROL_T values
         * @throws std::runtime_error on failure
         */
        void setFlowControl(RN2903_FLOW_CONTROL_T fc);

        /**
         * This is a utility function that can be used to indicate if a
         * given string is present at the beginning of the response
         * buffer.  The search is case sensitive.
         *
         * @param str The string to search for
         * @return true if the string was found at the beginning of the
         * response buffer, false otherwise
         */
        bool find(const std::string str);

        /**
         * This is a utility function that can be used to return the
         * hex encoded payload string for radio_rx messages received.
         *
         * @return A string representing the hex encoded payload, or
         * an empty string if there was an error
         */
        std::string getRadioRxPayload();

        /**
         * This function attempts to sync the device to the current
         * baudrate.  It tries retries times, to send an autobaud
         * sequence to the device and run a test command.
         *
         * @param retries The number of times to retry autobaud detection
         * @return true if the test command succeeded, false otherwise
         */
        bool autobaud(int retries);


    protected:
        // rn2903 device context
        rn2903_context m_rn2903;

        /**
         * Read character data from the device
         *
         * @param size The maximum number of characters to read
         * @return string containing the data read
         */
        std::string read(int size);

        /**
         * Write character data to the device
         *
         * @param buffer The string containing the data to write
         * @return The number of bytes written
         */
        int write(std::string buffer);

    private:
        /* Disable implicit copy and assignment operators */
        RN2903(const RN2903&) = delete;
        RN2903 &operator=(const RN2903&) = delete;
    };
}
