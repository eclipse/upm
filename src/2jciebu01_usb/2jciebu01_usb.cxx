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

#include "2jciebu01_usb.hpp"

using namespace upm;
using namespace std;

static const int defaultDelay = 1000;     // max wait time for read


OM2JCIEBU_UART::OM2JCIEBU_UART(std::string uart_raw, int baud) : m_uart(uart_raw) 
{
    if(!setupTty(baud))
        throw std::runtime_error(std::string(__FUNCTION__) +
                                 ": failed to set baud rate to " + std::to_string(baud));
}

bool OM2JCIEBU_UART::setupTty(uint32_t baud)
{
    return m_uart.setBaudRate(baud) == mraa::SUCCESS;
}

uint8_t OM2JCIEBU_UART::setMode(int bytesize, mraa::UartParity parity, int stopbits)
{
    return m_uart.setMode(bytesize, parity, stopbits);
}

uint8_t OM2JCIEBU_UART::setFlowControl(bool xonxoff, bool rtscts)
{
    return  m_uart.setFlowcontrol(xonxoff, rtscts);
}

int OM2JCIEBU_UART::readData(char *buffer, int len)
{
    if(buffer == NULL) {
        std::cout << "Null pointer received..." << std::endl;
        return FAILURE;
    }
    if(!m_uart.dataAvailable(defaultDelay)) //time out for read UART data
        return 0;               

    int rv = m_uart.read(buffer, len);

    //check for UART read fail
    if(rv < 0)
        throw std::runtime_error(std::string(__FUNCTION__) +
                                 ": Uart::read() failed: " + string(strerror(errno)));

    return rv;
}

int OM2JCIEBU_UART::writeData(char *buffer, int len)
{
    if(buffer == NULL) {
        std::cout << "Null pointer received..." << std::endl;
        return FAILURE;
    }
    int rv = m_uart.write(buffer, len);

    //check for UART write fail
    if(rv < 0)
        throw std::runtime_error(std::string(__FUNCTION__) +
                                 ": Uart::write() failed: " +
                                 string(strerror(errno)));
    //check for UART write fail
    if(rv == 0)
        throw std::runtime_error(std::string(__FUNCTION__) +
                                 ": Uart::write() failed, no bytes written");

    return rv;
}

int OM2JCIEBU_UART::readCmdPacket(OM2JCIEBU_UART::OM2JCIEBU_ATTRIBUTE_T attribute_name)
{
    /* Create a payload as per OMRON uart frame format*/
    uint8_t omPkt[OM2JCIEBU_UART_MAX_PKT_LEN] = {0};
    uint16_t pktLength = 0, crc = 0, pktIndex = 0, address = 0;


    omPkt[pktIndex++] = OM2JCIEBU_UART_HEADER_START;
    omPkt[pktIndex++] = OM2JCIEBU_UART_HEADER_END;

    pktLength = OM2JCIEBU_CRC_LENGTH + OM2JCIEBU_UART_ADDRESS_LENGTH + OM2JCIEBU_UART_COMMAND_LENGHT;

    omPkt[pktIndex++] = pktLength & 0x00FF;
    omPkt[pktIndex++] = pktLength >> 8;

    omPkt[pktIndex++] = UART_CMD_READ;

    getAddress(attribute_name, USB_TO_UART, &address);

    omPkt[pktIndex++] = address & 0x00FF;
    omPkt[pktIndex++] = address >> 8;

    crc = crc_16(omPkt, pktIndex);

    omPkt[pktIndex++] = crc & 0x00FF;
    omPkt[pktIndex++] = crc >> 8;

    return (writeData((char *)omPkt, pktIndex));
}

void OM2JCIEBU_UART::configureSensorAdvSetting(uint16_t milliseconds, OM2JCIEBU::OM2JCIEBU_ADV_PARAM_T adv_mode)
{
    
    uint8_t adv_config[3] = {0};
    uint16_t interval;
    interval = milliseconds / OM2JCIEBU_INTERVAL_UNIT; /*calculate interval which is given by user using interval unit */

    adv_config[0] = interval & 0x00FF;
    adv_config[1] = interval >> 8;
    adv_config[2] = adv_mode;

    writeCmdPacket(ADV_CONFIGURE, adv_config, sizeof(adv_config));
}

void OM2JCIEBU_UART::configureSensorLedState(OM2JCIEBU::OM2JCIEBU_LED_SCALE_T state, uint8_t red, uint8_t green, uint8_t blue)
{
    uint8_t led_config[5] = {0};

    led_config[0] = state;
    led_config[1] = 0x00;
    led_config[2] = red;
    led_config[3] = green;
    led_config[4] = blue;

    writeCmdPacket(LED_CONFIGURE, led_config, sizeof(led_config));
}

int OM2JCIEBU_UART::writeCmdPacket(OM2JCIEBU_UART::OM2JCIEBU_ATTRIBUTE_T attribute_name, uint8_t *data, uint16_t length)
{
    /* Create a frame formate for write a data on UART as per common frame formate*/
    if(data == NULL) {
        std::cout << "Null pointer received..." << std::endl;
        return FAILURE;
    }
    uint8_t omPkt[OM2JCIEBU_UART_MAX_PKT_LEN] = {0};
    uint16_t pktLength = 0, crc = 0, pktIndex = 0, l_iterator = 0, address = 0;

    omPkt[pktIndex++] = OM2JCIEBU_UART_HEADER_START;
    omPkt[pktIndex++] = OM2JCIEBU_UART_HEADER_END;

    pktLength = OM2JCIEBU_CRC_LENGTH + OM2JCIEBU_UART_ADDRESS_LENGTH + OM2JCIEBU_UART_COMMAND_LENGHT + length;

    omPkt[pktIndex++] = pktLength & 0x00FF;
    omPkt[pktIndex++] = pktLength >> 8;

    omPkt[pktIndex++] = UART_CMD_WRITE;

    getAddress(attribute_name, USB_TO_UART, &address);

    omPkt[pktIndex++] = address & 0x00FF;
    omPkt[pktIndex++] = address >> 8;

    for(l_iterator = 0; l_iterator < length; l_iterator++) {
        omPkt[pktIndex++] = data[l_iterator];
    }

    crc = crc_16(omPkt, pktIndex);

    omPkt[pktIndex++] = crc & 0x00FF;
    omPkt[pktIndex++] = crc >> 8;

    return (writeData((char *)omPkt, pktIndex));
}

void OM2JCIEBU_UART::getSensorAttribute(OM2JCIEBU_ATTRIBUTE_T attribute_name, void *attributeValue)
{
    /* Assign sensor value attributes to void pointer*/
    if(attributeValue == NULL) {
        std::cout << "Null pointer received..." << std::endl;
        return;
    }
    switch(attribute_name) {
        case ALL_PARAM:
            memcpy(attributeValue, &om2jciebuData_uart, sizeof(om2jciebuData_uart));
            break;
        case TEMP:
            *(int16_t *) attributeValue = om2jciebuData_uart.temperature;
            break;
        case HUMIDITY:
            *(int16_t *) attributeValue = om2jciebuData_uart.relative_humidity;
            break;
        case AMBIENT_LIGHT:
            *(int16_t *) attributeValue = om2jciebuData_uart.ambient_light;
            break;
        case PRESSURE:
            *(int32_t *) attributeValue = om2jciebuData_uart.pressure;
            break;
        case NOISE:
            *(int16_t *) attributeValue = om2jciebuData_uart.noise;
            break;
        case ETVOC:
            *(int16_t *) attributeValue = om2jciebuData_uart.eTVOC;
            break;
        case ECO2:
            *(int16_t *) attributeValue = om2jciebuData_uart.eCO2;
            break;
        case DISCOMFORT_INDEX:
            *(int16_t *) attributeValue = om2jciebuData_uart.discomfort_index;
            break;
        case HEAT_STROKE:
            *(int16_t *) attributeValue = om2jciebuData_uart.heat_stroke;
            break;
        case LED_CONFIGURE:
            break;
        case ADV_CONFIGURE:
            break;
    }
}

OM2JCIEBU_UART::OM2JCIEBU_ERROR_T OM2JCIEBU_UART::getSensorData(OM2JCIEBU_ATTRIBUTE_T attribute_name, void *attribute_data)
{
    if(attribute_data == NULL) {
        std::cout << "Null pointer received..." << std::endl;
        return FAILURE;
    }
    //create a payload frame for read sensor data
    readCmdPacket(attribute_name);

    char buf[OM2JCIEBU_UART_MAX_READ_PKT_LEN];
    uint8_t pkt[OM2JCIEBU_UART_MAX_PKT_LEN];
    int rv;
    int8_t pkt_index = 0, idx = 0;
    OM2JCIEBU_UART::OM2JCIEBU_ERROR_T verifyResult = FAILURE;

    //read from UART
    while(true) {
        rv = readData(buf, OM2JCIEBU_UART_MAX_READ_PKT_LEN);
        if(rv > 0) {
            for(idx = 0; idx < rv; idx++)
                pkt[pkt_index++] = buf[idx];
        } else {
            verifyResult = verifyPacket(pkt, pkt_index);
            if(verifyResult == SUCCESS) {
                break;
            } else {
                return verifyResult;
            }
        }
    }
    //calculate a data and store in struct
    parseSensorData(pkt);

    //copy data to user provided pointer
    getSensorAttribute(attribute_name, attribute_data);

    return verifyResult;
}

OM2JCIEBU_UART::OM2JCIEBU_ERROR_T OM2JCIEBU_UART::verifyPacket(uint8_t *pkt, int len)
{
    if(pkt == NULL) {
        std::cout << "Null pointer received..." << std::endl;
        return FAILURE;
    }
    uint16_t crc = 0;
    OM2JCIEBU_UART::OM2JCIEBU_ERROR_T verifyResult = FAILURE;
    //Verify a data which is read from UART buffer
    if((pkt[OM2JCIEBU_UART_COMMAND_INDEX] & 0xF0) == 0x80) { //Check for error in payload
        if(pkt[OM2JCIEBU_UART_COMMAND_ERROR_CODE_INDEX] == ERROR_UART_CRC_WRONG) {
            std::cout << "Error CRC wrong" << std::endl;
            verifyResult = ERROR_CRC_WRONG;
        } else if(pkt[OM2JCIEBU_UART_COMMAND_ERROR_CODE_INDEX] == ERROR_UART_WRONG_COMMAND) {
            std::cout << "Error Invalid Command" << std::endl;
            verifyResult =  ERROR_WRONG_COMMAND;
        } else if(pkt[OM2JCIEBU_UART_COMMAND_ERROR_CODE_INDEX] == ERROR_UART_WRONG_ADDRESS) {
            std::cout << "Error Invalid Address" << std::endl;
            verifyResult =  ERROR_WRONG_ADDRESS;
        } else if(pkt[OM2JCIEBU_UART_COMMAND_ERROR_CODE_INDEX] == ERROR_UART_WRONG_LENGTH) {
            std::cout << "Error Invalid Length" << std::endl;
            verifyResult =  ERROR_WRONG_LENGTH;
        } else if(pkt[OM2JCIEBU_UART_COMMAND_ERROR_CODE_INDEX] == ERROR_UART_DATA_RANGE) {
            std::cout << "Error Invalid Data Range" << std::endl;
            verifyResult =  ERROR_DATA_RANGE;
        } else if(pkt[OM2JCIEBU_UART_COMMAND_ERROR_CODE_INDEX] == ERROR_UART_BUSY) {
            std::cout << "Uart is BUSY" << std::endl;
            verifyResult =  ERROR_BUSY;
        }
    } else if((pkt[OM2JCIEBU_UART_COMMAND_INDEX] & 0xFF) == 0xFF) { //Check for unknow error in UART frame
        std::cout << "Invalid reponse" << std::endl;
        verifyResult =  ERROR_UNKNOWN;
    } else {
        crc = crc_16(pkt, (len - OM2JCIEBU_CRC_LENGTH));            //Check for CRC which is read from UART frame
        if(pkt[len - OM2JCIEBU_CRC_LENGTH] == (crc & 0x00FF) && pkt[len - 1] == crc >> 8) {
            verifyResult =  SUCCESS;
        } else {
            std::cout << "Does not match CRC" << std::endl;
            verifyResult =  ERROR_CRC_MISMATCH;
        }
    }
    return verifyResult;
}

void OM2JCIEBU_UART::parseSensorData(uint8_t *data)
{
    if(data == NULL) {
        std::cout << "Null pointer received..." << std::endl;
        return;
    }
    //Parse data after payload verfication
    om2jciebuData_uart.sequence_number = data[7];

    om2jciebuData_uart.temperature = data[8] | data[9] << 8;
    om2jciebuData_uart.temperature = om2jciebuData_uart.temperature / 100;

    om2jciebuData_uart.relative_humidity = data[10] | data[11] << 8;
    om2jciebuData_uart.relative_humidity = om2jciebuData_uart.relative_humidity / 100;

    om2jciebuData_uart.ambient_light = data[12] | data[13] << 8;

    om2jciebuData_uart.pressure = data[14] | data[15] << 8 | data[16] << 16 | data[17] << 24;
    om2jciebuData_uart.pressure = om2jciebuData_uart.pressure / 1000;

    om2jciebuData_uart.noise = data[18] | data[19] << 8;
    om2jciebuData_uart.noise = om2jciebuData_uart.noise / 100;

    om2jciebuData_uart.eTVOC = data[20] | data[21] << 8;

    om2jciebuData_uart.eCO2 = data[22] | data[23] << 8;

    om2jciebuData_uart.discomfort_index = data[24] | data[25] << 8;
    om2jciebuData_uart.discomfort_index = om2jciebuData_uart.discomfort_index / 100;

    om2jciebuData_uart.heat_stroke = data[26] | data[27] << 8;
    om2jciebuData_uart.heat_stroke = om2jciebuData_uart.heat_stroke / 100;
}
