/*
 * Author: Hiroyuki Mino <omronsupportupm@omron.com>
 * Copyright (c) 2019 Omron Electronic Components - Americas
 *
 * This program and the accompanying materials are made available under the
 * terms of the The MIT License which is available at
 * https://opensource.org/licenses/MIT.
 *
 * SPDX-License-Identifier: MIT
 */

/*=========================================================================*/

#pragma once


#include <tinyb.hpp>
#include <iostream>
#include <string>
#include <stdexcept>
#include <vector>
#include <thread>
#include <atomic>
#include <csignal>
#include "2jciebu01.hpp"

/*MACROS and enum */


#define OM2JCIEBU_BLE_DISCOVERY_RETRY                      15
#define OM2JCIEBU_BLE_LED_AND_ADV_CONFIGUARTION_SERVICE    "ab705110-0a3a-11e8-ba89-0ed5f89f718b"
#define OM2JCIEBU_BLE_LIVE_SENSOR_DATA_SERVICE             "ab705010-0a3a-11e8-ba89-0ed5f89f718b"

#define OM2JCIEBU_BLE_COM_ID_INDEX                         0x00
#define OM2JCIEBU_BLE_DATA_TYPE_INDEX                      0x02

#define VERIFY_PACKET                                      0
#define DEBUG_LOG                                          0
#define MAX_UUID_SIZE                                      64
#define MAX_SENSOR_DATA_SIZE                               64


/*=========================================================================*/

namespace upm
{
/**
  * @brief 2JCIEBU01 BLE Environment Sensor
  * @defgroup 2jciebu01-ble libupm-2jciebu01-ble
  * @ingroup omron ble accelerometer pressure sound flexfor
  */

/**
 * @library 2jciebu01-ble
 * @sensor 2jciebu01-ble
 * @comname Omron BLE Environment Sensor Module
 * @type accelerometer pressure sound flexfor
 * @man omron
 * @con ble
 * @web https://www.components.omron.com/solutions/mems-sensors/environment-sensor
 *
 * @brief API for the Omron USB type environment Sensor Module using BLE interface
 *
 * It is connected via a BLE Interface
 *
 * @snippet 2jciebu01-ble.cxx Interesting
 */
class OM2JCIEBU_BLE : public OM2JCIEBU
{
public :

    /**
    * OM2JCIEBU_BLE Constructor, takes the device address as
    * an argument
    *
    * @param device MAC address of Omron Environment Sensor
    */
    OM2JCIEBU_BLE(std::string ble_address);


    /**
    * Get discovery service from Connetced BLE device
    *
    * @param attribute_name  attribute name of sensor
    * @return OM2JCIEBU_ERROR_T
    */
    OM2JCIEBU_ERROR_T getDiscoveredServices(OM2JCIEBU::OM2JCIEBU_ATTRIBUTE_T attribute_name);

    /**
     * Get omron sensor live data as per request uisng 0x5012 UUID
     *
     * @param attribute_name   Name of attribute
     * @param attribute_data   Data of attirbute
     * @return One of the OM2JCIEBU_ERROR_T values
     */
    OM2JCIEBU_ERROR_T getSensorData(OM2JCIEBU::OM2JCIEBU_ATTRIBUTE_T attribute_name, void *attribute_data);

    /**
     * Get omron sensor live data based on advertise payload
     *
     * @param attribute_name   Name of attribute
     * @param attribute_data   Data of attirbute
     * @return One of the OM2JCIEBU_ERROR_T values
     */
    OM2JCIEBU_ERROR_T getAdvSensorData(OM2JCIEBU::OM2JCIEBU_ATTRIBUTE_T attribute_name, void *attribute_data);

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

    /**
     * Disconnect BLE Device
     *
     * @return Return success or Failure
     *
     */
    bool removeBleDevice();

private:
    BluetoothManager *bleManager = nullptr;
    BluetoothDevice  *bleSensorTag = nullptr;
    BluetoothGattCharacteristic *bleSensorChar = nullptr;
    BluetoothGattService *bleService = nullptr;
    om2jciebuData_t om2jciebuData_ble;
    std::string bleMACaddress;
    bool is_BleConnected = false;


    /**
     *connect BLE Device
     *
     * @param device_address  BLE deivce adddress
     *
     * @return Return success or Failure
     */
    bool connectBleDevice(std::string ble_address);

    /**
     * Start BLE Discovery
     *
     * @return Return success or Failure
     */
    bool startBleDiscovery();

    /**
     * Stop BLE Discovery
     *
     * @return Return success or Failure
     */
    bool stopBleDiscovery();


    /**
     * Get sensor data from global struct.
     *
     * @param attribute_name   attribute_name of sensor data
     * @param attributeValue   Data of attirbute
     */

    void getSensorAttribute(OM2JCIEBU::OM2JCIEBU_ATTRIBUTE_T attribute_name, void *attributeValue);

    /**
     * Verifies the packet header and indicates its valid or not
     *
     * @param pkt Packet to check
     * @param len length of packet
     * @return One of the OM2JCIEBU_ERROR_T values
     */

    OM2JCIEBU_ERROR_T verifyPacket(uint8_t *pkt, int len);


    /**
    * Calculate and parse advertise sensor data and store into
    * structure
    *
    * @param data   Packet
    *
    */
    void parseAdvSensorData(uint8_t *data);

    /**
    * Calculate and parse sensor data and store into
    * structure
    *
    * @param data   Packet
    *
    */
    void parseSensorData(uint8_t *data);


    /**
     * Write packet over BLE
     *
     * @param attribute_name   attribute_name of sensor
     * @param arg_value        arg value for write a data over BLE
     *
     * @return Return success or Failure
     */
    bool writePacket(OM2JCIEBU::OM2JCIEBU_ATTRIBUTE_T attribute_name, const std::vector<unsigned char> &arg_value);
};
}

