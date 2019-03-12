/*
*
* Author: Hiroyuki Mino <omronsupportupm@omron.com>
* Copyright (c) 2019 Omron Electronic Components - Americas
*
* Permission is hereby granted, free of charge, to any person obtaining a copy of
* this software and associated documentation files (the "Software"), to deal in
* the Software without restriction, including without limitation the rights to
* use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
* the Software, and to permit persons to whom the Software is furnished to do so,
* subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in all
* copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
* FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
* COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
* IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
* CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

/*=========================================================================*/

#pragma once

#include "mraa.hpp"
#include "../2jciebu01.hpp"

/*MACROS and enum */

// protocol start codes
#define OM2JCIEBU_UART_HEADER_START                         0x52
#define OM2JCIEBU_UART_HEADER_END                           0x42

#define OM2JCIEBU_UART_MAX_PKT_LEN                          256
#define OM2JCIEBU_UART_MAX_READ_PKT_LEN                     10
#define OM2JCIEBU_UART_COMMAND_LENGHT                       1
#define OM2JCIEBU_UART_ADDRESS_LENGTH                       2
#define OM2JCIEBU_UART_HEADER_LENGTH                        2

#define OM2JCIEBU_UART_COMMAND_INDEX                        0x04
#define OM2JCIEBU_UART_COMMAND_ERROR_CODE_INDEX             0x07





/*=========================================================================*/

namespace upm
{
/**
  * @brief 2JCIEBU01 Environment sensor
  * @defgroup 2jciebu01 libupm-2jciebu01_usb
  * @ingroup Omron USB type
  */

/**
 * @library libupm-2jciebu01_usb
 * @sensor 2jciebu01
 * @comname Environment Sensor Module
 * @altname Omron Environment sensor USB type
 * @type USB
 * @man Omron
 * @web https://www.components.omron.com/solutions/mems-sensors/environment-sensor
 * @con usb
 *
 * @brief API for the Omron USB type environment Sensor Module using USB to UART interface
 *
 * It is connected via a UART at 115200 baud.
 *
 * @snippet 2jciebu01_usb.cxx Interesting
 */
class OM2JCIEBU_UART : public OM2JCIEBU
{
public :

    typedef enum {
        UART_CMD_READ                    = 0x01,
        UART_CMD_WRITE                   = 0x02
    } OM2JCIEBU_UART_COMMAND_T;


    typedef enum {
        ERROR_UART_CRC_WRONG             = 0x01,
        ERROR_UART_WRONG_COMMAND         = 0x02,
        ERROR_UART_WRONG_ADDRESS         = 0x03,
        ERROR_UART_WRONG_LENGTH          = 0x04,
        ERROR_UART_DATA_RANGE            = 0x05,
        ERROR_UART_BUSY                  = 0x06,
        ERROR_UART_UNKNOWN               = 0XFF
    } OM2JCIEBU_UART_ERROR_T;

    typedef enum {
        PARITY_UART_NONE                 = 0,
        PARITY_UART_EVEN                 = 1,
        PARITY_UART_ODD                  = 2,
        PARITY_UART_MARK                 = 3,
        PARITY_UART_SPACE                = 4
    } OM2JCIEBU_UART_PARITY_T;

    /**
    * OM2JCIEBU_UART Constructor, takes a string to the path of the serial
    * interface that is needed.
    *
    * @param uart File path (/dev/ttyXXX to uart
    * @param baud Desired baud rate
    */
    OM2JCIEBU_UART(std::string path, int baud = 115200);

    /**
     * Sets up proper tty I/O modes and the baud rate. For this device,
     * the default baud rate is 115200.
     *
     * @param baud Desired baud rate.
     * @return True if successful
     */
    bool setupTty(uint32_t baud = 115200);

    /**
     * Set the transfer mode
     * For example setting the mode to 8N1 would be
     * "dev.setMode(8,PARITY_NONE , 1)"
     *
     * @param bytesize data bits
     * @param parity   Parity bit setting
     * @param stopbits stop bits
     *
     * @return Return success or Failure
     */
    uint8_t setMode(int bytesize, mraa::UartParity parity, int stopbits);

    /**
     * Set the flowcontrol
     *
     * @param xonxoff XON/XOFF Software flow control.
     * @param rtscts RTS/CTS out of band hardware flow control
     * @return Return success or Failure
     */
    uint8_t setFlowControl(bool xonxoff, bool rtscts);


    /**
     * Get omron sensor live data as per request
     *
     * @param attribute_name   Name of attribute
     * @param attribute_data   Data of attirbute
     * @return One of the OM2JCIEBU_ERROR_T values
     */
    OM2JCIEBU_ERROR_T getSensorData(OM2JCIEBU::OM2JCIEBU_ATTRIBUTE_T attribute_name, void *attribute_data);

    /**
    * Set LED configartion of sensor
    *
    * @param state  state for led configuartion
    * @param red    value of red
    * @param green  value of green
    * @param blue   value of blue
    */
    void configureSensorLedState(OM2JCIEBU::OM2JCIEBU_LED_SCALE_T state, uint8_t red, uint8_t green, uint8_t blue);

    /**
     * Set Advertise interval setting of sensor
     *
     * @param miliseconds   interval for Advertise data
     * @param adv_mode      Advertise mode
     */
    void configureSensorAdvSetting(uint16_t milliseconds, OM2JCIEBU::OM2JCIEBU_ADV_PARAM_T adv_mode);

private:

    mraa::Uart m_uart;
    om2jciebuData_t om2jciebuData_uart;

    /**
    * Composes write command packet and Writes the data
    * in the buffer to the device
    *
    * @param attribute_name  Attribute name of sensor
    * @param data            data for write on particular address
    * @param length          length of data
    * @return Number of bytes written
    */
    int writeCmdPacket(OM2JCIEBU::OM2JCIEBU_ATTRIBUTE_T attribute_name, uint8_t *data, uint16_t length);

    /**
     * Composes read command packet and Writes the data
     * in the buffer to the device
     *
     * @param attribute_name  Attribute name of sensor
     * @return Number of bytes written
     */
    int readCmdPacket(OM2JCIEBU::OM2JCIEBU_ATTRIBUTE_T attribute_name);

    /**
     * Get sensor data from global struct.
     *
     * @param attributeValue   Data of attirbute
     *
     */

    void getSensorAttribute(OM2JCIEBU::OM2JCIEBU_ATTRIBUTE_T attribute_name, void *attributeValue);

    /**
     * Writes the data in the buffer to the device
     *
     * @param buffer Buffer to hold the data read
     * @param len Length of the buffer
     * @return Number of bytes written
     */
    int writeData(char *buffer, int len);

    /**
     * Reads any available data in a user-supplied buffer. Note: the
     * call blocks until data is available to be read. Use
     * dataAvailable() to determine whether there is data available
     * beforehand, to avoid blocking.
     *
     * @param buffer Buffer to hold the data read
     * @param len Length of the buffer
     * @return Number of bytes read
     */
    int readData(char *buffer, int len);

    /**
     * Verifies the packet header and indicates it is valid or not
     *
     * @param pkt Packet to check
     * @param len length of packet
     * @return One of the OM2JCIEBU_ERROR_T values
     */

    OM2JCIEBU_ERROR_T verifyPacket(uint8_t *pkt, int len);


    /**
    * Calculate and parse sensor data and store into
    * Sensor data structure
    *
    * @param data   Packet
    *
    */
    void parseSensorData(uint8_t *data);
};
}
