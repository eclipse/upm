/*
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

#include <string>
#include <iostream>
#include <limits>

#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/select.h>
#include <sys/types.h>
#include <sys/stat.h>


#include <upm/upm_utilities.h>


/*MACROS and enum */


#define OM2JCIEBU_FLASH_LONG_DATA_READ_ADD_UART    0x500E
#define OM2JCIEBU_LIVE_LONG_DATA_READ_ADD_UART     0x5021

#define OM2JCIEBU_LED_CONFIGUARTION_ADD_UART        0x5111
#define OM2JCIEBU_ADV_CONFIGUARTION_ADD_UART        0x5115

#define OM2JCIEBU_LED_CONFIGUARTION_UUID_BLE       "ab705111-0a3a-11e8-ba89-0ed5f89f718b"
#define OM2JCIEBU_ADV_CONFIGUARTION_UUID_BLE       "ab705115-0a3a-11e8-ba89-0ed5f89f718b"
#define OM2JCIEBU_LIVE_LONG_DATA_READ_UUID_BLE     "ab705012-0a3a-11e8-ba89-0ed5f89f718b"



#define OM2JCIEBU_CRC_LENGTH                       2
#define OM2JCIEBU_CRC16                            0xFFFF
#define OM2JCIEBU_INTERVAL_UNIT                    0.625


/*=========================================================================*/

namespace upm
{
/**
  * @brief omron 2JCIEBU01 Environment sensor
  * @defgroup 2jciebu01 libupm-2jciebu01
  * @ingroup Omron USB  type
  */

/**
 * @library 2jciebu01
 * @sensor 2jciebu01
 * @comname Environment Sensor Module
 * @altname Omron Environment sensor USB type
 * @type USB
 * @man Omron
 * @web https://www.components.omron.com/solutions/mems-sensors/environment-sensor
 * @con usb and ble
 *
 * @brief API for the Omron USB type environment Sensor Module
 *
 * @snippet 2jciebu01.cxx Interesting
 */
class OM2JCIEBU
{
public :
    typedef enum {
        ALL_PARAM,
        TEMP,
        HUMIDITY,
        AMBIENT_LIGHT,
        PRESSURE,
        NOISE,
        ETVOC,
        ECO2,
        DISCOMFORT_INDEX,
        HEAT_STROKE,
        LED_CONFIGURE,
        ADV_CONFIGURE,
    } OM2JCIEBU_ATTRIBUTE_T;

    typedef enum {
        BLE,
        USB_TO_UART
    } OM2JCIEBU_INTERFACE_T;

    typedef enum {
        SENSOR_DATA            = 1,
        ACCELERATION_DATA,
        ACCELERATION_SENSOR_DATA,
        ACCELERATION_SENSOR_FLAG,
        SERIAL_NUMBER
    } OM2JCIEBU_ADV_PARAM_T;

    typedef enum {
        ERROR_CRC_WRONG        = -1,
        ERROR_WRONG_COMMAND,
        ERROR_WRONG_ADDRESS,
        ERROR_WRONG_LENGTH,
        ERROR_DATA_RANGE,
        ERROR_BUSY,
        ERROR_UNKNOWN,
        ERROR_CRC_MISMATCH,
        FAILURE                = 0,
        SUCCESS                = 1
    } OM2JCIEBU_ERROR_T;

    typedef enum {
        NORMALLY_OFF           = 0,
        NORMALLY_ON,
        TEMP_SACLE,
        HUMIDITY_SCALE,
        AMBIENT_LIGHT_SCALE,
        PRESSURE_SCALE,
        NOISE_SCALE,
        ETVOC_SCALE,
        SI_SCALE,
        PGA_SCALE
    } OM2JCIEBU_LED_SCALE_T;

    typedef struct {
        uint8_t sequence_number;
        int16_t temperature;
        int16_t relative_humidity;
        int16_t ambient_light;
        int32_t pressure;
        int16_t noise;
        int16_t eTVOC;
        int16_t eCO2;
        int16_t discomfort_index;
        int16_t heat_stroke;
    } __attribute__((packed))om2jciebuData_t;

    /**
    * OM2JCIEBU destructor
    */
    virtual ~OM2JCIEBU() {}

    /**
     * get address or UUID based on attribute name
     *
     * @param attribute_name     attribute name of sensor
     * @param interface          Interface name of sensor
     * @param attribute_value    address value and UUID based on attribute name
     */
    void getAddress(OM2JCIEBU_ATTRIBUTE_T attribute_name, OM2JCIEBU_INTERFACE_T interface, void *attribute_value);

    /**
    * Delay for read sensor data;
    *
    * @param second second for delay
    */
    void delay(int second);

    /**
    * Calculate crc-16 from the header
    * to the end of the payload.
    *
    * @param data   Packet
    * @param length length of packet
    * @return 16 bit crc of payload
    */
    uint16_t crc_16(uint8_t *data, int length);


    /**
    * Set LED configartion of sensor
    *
    * @param state  state for led configuartion
    * @param red    value of red
    * @param green  value of green
    * @param blue   value of blue
    */
    virtual void configureSensorLedState(OM2JCIEBU_LED_SCALE_T state, uint8_t red, uint8_t green, uint8_t blue) = 0;

    /**
     * Set Advertise configuration of sensor
     *
     * @param miliseconds   interval for Advertise data
     * @param adv_mode      Advertise mode
     */
    virtual void configureSensorAdvSetting(uint16_t milliseconds, OM2JCIEBU_ADV_PARAM_T adv_mode) = 0;


    /**
     * Calculate and parse sensor data and store into
     * Sensor data structure
     *
     * @param data   Packet
     *
     */
    virtual void parseSensorData(uint8_t *data) = 0;

    /**
     * Get omron sensor live data as per attribute name
     *
     * @param attribute_name   Name of attribute
     * @param attribute_data   Data of attirbute
     * @return One of the OM2JCIEBU_ERROR_T values
     */
    virtual OM2JCIEBU_ERROR_T getSensorData(OM2JCIEBU_ATTRIBUTE_T attribute_name, void *attribute_data) = 0;

    /**
    * Verifies the packet header and indicates it is valid or not
    *
    * @param pkt Packet to check
    * @param len length of packet
    * @return One of the OM2JCIEBU_ERROR_T values
    */

    virtual OM2JCIEBU_ERROR_T verifyPacket(uint8_t *pkt, int len) = 0;
};
}
