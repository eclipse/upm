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

#include <iostream>
#include <string>
#include <stdexcept>

#include "2jciebu01.hpp"

using namespace upm;
using namespace std;


void OM2JCIEBU::getAddress(OM2JCIEBU_ATTRIBUTE_T attribute_name, OM2JCIEBU_INTERFACE_T interface, void *attribute_value)
{
    if(attribute_value == NULL) {
        std::cout << "Null pointer received..." << std::endl;
        return;
    }
    switch(attribute_name) {
        case ALL_PARAM:
        case TEMP:
        case HUMIDITY:
        case AMBIENT_LIGHT:
        case PRESSURE:
        case NOISE:
        case ETVOC:
        case ECO2:
        case DISCOMFORT_INDEX:
        case HEAT_STROKE:
            if(interface == USB_TO_UART) //Check for interface
                *(uint16_t *)attribute_value = OM2JCIEBU_LIVE_LONG_DATA_READ_ADD_UART;
            else
                memcpy(attribute_value, OM2JCIEBU_LIVE_LONG_DATA_READ_UUID_BLE, strlen(OM2JCIEBU_LIVE_LONG_DATA_READ_UUID_BLE));
            break;
        case LED_CONFIGURE:
            if(interface == USB_TO_UART)
                *(uint16_t *)attribute_value = OM2JCIEBU_LED_CONFIGUARTION_ADD_UART;
            else
                memcpy(attribute_value, OM2JCIEBU_LED_CONFIGUARTION_UUID_BLE, strlen(OM2JCIEBU_LED_CONFIGUARTION_UUID_BLE));
            break;
        case ADV_CONFIGURE:
            if(interface == USB_TO_UART)
                *(uint16_t *)attribute_value = OM2JCIEBU_ADV_CONFIGUARTION_ADD_UART;
            else
                memcpy(attribute_value, OM2JCIEBU_ADV_CONFIGUARTION_UUID_BLE, strlen(OM2JCIEBU_ADV_CONFIGUARTION_UUID_BLE));
            break;
    }
}


void OM2JCIEBU::delay(int second)
{
    sleep(second); 
}

uint16_t OM2JCIEBU::crc_16(uint8_t *data, int length)
{
    /* calculate crc_16 for payload */
    if(data == NULL) {
        std::cout << "Null pointer received..." << std::endl;
        return 0;
    }
    uint16_t crc = OM2JCIEBU_CRC16, l_outeriterator = 0, l_Inneriterator = 0, carrayFlag = 0;
    for(l_outeriterator = 0; l_outeriterator < length; l_outeriterator++) {
        crc = crc ^ data[l_outeriterator];
        for(l_Inneriterator = 0; l_Inneriterator < 8; l_Inneriterator++) {
            carrayFlag = crc & 1;
            crc = crc >> 1;
            if(carrayFlag == 1) {
                crc = crc ^ 0xA001;
            }
        }
    }
    return crc;
}



