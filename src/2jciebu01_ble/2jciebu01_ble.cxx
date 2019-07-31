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


#include "2jciebu01_ble.hpp"

using namespace upm;
using namespace std;



OM2JCIEBU_BLE::OM2JCIEBU_BLE(std::string ble_address)
{
    //initializing tinyB 
    try {
        bleManager = BluetoothManager::get_bluetooth_manager();
    } catch(const std::runtime_error& e) {
        std::cerr << "Error while initializing libtinyb: " << e.what() << std::endl;
        exit(1);
    }
    OM2JCIEBU_BLE::bleMACaddress = ble_address;

}

bool OM2JCIEBU_BLE::connectBleDevice(std::string ble_address)
{
    
    bool ret;
    //Start BLE discovery
    if(startBleDiscovery()) {
        std::cout << "Searching for Sensor...\r\n";
        for(int i = 0; i < OM2JCIEBU_BLE_DISCOVERY_RETRY ; ++i) {
            auto list = bleManager->get_devices();

            for(auto it = list.begin(); it != list.end(); ++it) {
#if DEBUG_LOG
                std::cout << "Class = " << (*it)->get_class_name() << " ";
                std::cout << "Path = " << (*it)->get_object_path() << " ";
                std::cout << "Name = " << (*it)->get_name() << " ";
                std::cout << "Connected = " << (*it)->get_connected() << " ";
                std::cout << std::endl;
#endif

                /* Search for the device with the address given as a parameter to the program */
                if((*it)->get_address() == ble_address) {
                    bleSensorTag = (*it).release();
                }
            }
            /* Free the list of devices and stop if the device was found */
            if(bleSensorTag != nullptr) {
                std::cout << "Omron Sensor found  : " << ble_address << std::endl;
                break;
            }
            std::this_thread::sleep_for(std::chrono::seconds(4));
        }
    }
    ret = stopBleDiscovery();
    std::cout << "Stopped Ble Discovery = " << (ret ? "true" : "false") << std::endl;
    if(bleSensorTag == nullptr) {
        throw std::runtime_error(std::string(__FUNCTION__) +
                                 ": Could not find device " + ble_address);
    }
    if(bleSensorTag->connect()) {
        is_BleConnected = true;
    }
    return is_BleConnected;
}

bool OM2JCIEBU_BLE::removeBleDevice()
{
    if(is_BleConnected) { //disconnect with device if connected
        if(bleSensorTag->disconnect()) {
            is_BleConnected = false;
        }
    }
    return is_BleConnected;
}

bool OM2JCIEBU_BLE::startBleDiscovery()
{
    return bleManager->start_discovery();
}

bool OM2JCIEBU_BLE::stopBleDiscovery()
{
    return bleManager->stop_discovery();
}

bool OM2JCIEBU_BLE::writePacket(OM2JCIEBU::OM2JCIEBU_ATTRIBUTE_T attribute_name, const std::vector<unsigned char> &arg_value)
{
    if(!(is_BleConnected)) { //Connect with device if not connected
        if(!(connectBleDevice(OM2JCIEBU_BLE::bleMACaddress))) {
            return false;
        }
    }
    const char *cptr = (const char *)malloc(MAX_UUID_SIZE);
    if(cptr == NULL) {
        printf("Error. Allocation was unsuccessful. \r\n");
        return false;
    }
    memset((void *)cptr, 0, 64);
    getAddress(attribute_name, BLE, (void *)cptr);
    std::string uuid = ((const char *)cptr);
    free((void *)cptr);
    uint16_t getServiceRetryCount = 0;
    while(true) {
        auto list = bleSensorTag->get_services();
        //check for service list is empty or not
        if(list.empty()) {
            std::cout << "get services list empty " << std::endl;
            getServiceRetryCount++;
            if(getServiceRetryCount == OM2JCIEBU_BLE_DISCOVERY_RETRY) {
                getServiceRetryCount = 0;
                removeBleDevice();
                throw std::runtime_error(std::string(__FUNCTION__) +
                                         ": Could not find services ");
            }
            sleep(1);
            continue;
        }
        for(auto it = list.begin(); it != list.end(); ++it) {
#if DEBUG_LOG
            std::cout << "Class = " << (*it)->get_class_name() << " ";
            std::cout << "Path = " << (*it)->get_object_path() << " ";
            std::cout << "UUID = " << (*it)->get_uuid() << " ";
            std::cout << "Device = " << (*it)->get_device().get_object_path() << " ";
            std::cout << std::endl;
#endif

            /* Search for the LIVE sensor data service, by UUID */
            if((*it)->get_uuid() == OM2JCIEBU_BLE_LED_AND_ADV_CONFIGUARTION_SERVICE) {
                bleService = (*it).release();
            }
        }
        break;
    }
    if(bleService != nullptr) {
        auto charList = bleService->get_characteristics();
        std::cout << "Discovered characteristics: " <<  uuid  << std::endl;
        for(auto it = charList.begin(); it != charList.end(); ++it) {
#if DEBUG_LOG
            std::cout << "Class = " << (*it)->get_class_name() << " ";
            std::cout << "Path = " << (*it)->get_object_path() << " ";
            std::cout << "UUID = " << (*it)->get_uuid() << " ";
            std::cout << "Service = " << (*it)->get_service().get_object_path() << " ";
            std::cout << std::endl;
#endif

            if((*it)->get_uuid() == uuid) {
                bleSensorChar = (*it).release();
            }
        }
    } else {
        //If not find any sevice then disconnect BLE device
        removeBleDevice();
        throw std::runtime_error(std::string(__FUNCTION__) +
                                 ": Could not find Service ");
    }
    if(bleSensorChar == nullptr) {
        //If not find any characteristics then disconnect BLE device
        removeBleDevice();
        throw std::runtime_error(std::string(__FUNCTION__) +
                                 ": Could not find characteristics ");
    }
    bool ret = bleSensorChar->write_value(arg_value, 0); 
    return ret;
}

OM2JCIEBU_BLE::OM2JCIEBU_ERROR_T OM2JCIEBU_BLE::getDiscoveredServices(OM2JCIEBU::OM2JCIEBU_ATTRIBUTE_T attribute_name)
{
    if(!(is_BleConnected)) { //Connect with device if not connected
        if(!(connectBleDevice(OM2JCIEBU_BLE::bleMACaddress))) {
            return FAILURE;
        }
    }
    short int getServiceRetryCount = 0;
    const char *cptr = (const char *)malloc(MAX_UUID_SIZE);
    if(cptr == NULL) {
        printf("Error. Allocation was unsuccessful. \r\n");
        return FAILURE;
    }
    memset((void *)cptr, 0, MAX_UUID_SIZE);
    getAddress(attribute_name, BLE, (void *)cptr);
    std::string uuid = ((const char *)cptr);
    free((void *)cptr);
    //read services from connected BLE device
    while(true) {
        auto list = bleSensorTag->get_services();
        if(list.empty()) {
            std::cout << "get services list empty " << std::endl;
            getServiceRetryCount++;
            if(getServiceRetryCount == OM2JCIEBU_BLE_DISCOVERY_RETRY) {
                getServiceRetryCount = 0;
                removeBleDevice();
                throw std::runtime_error(std::string(__FUNCTION__) +
                                         ": Not Found any service ");
            }
            sleep(1);
            continue;
        }
        for(auto it = list.begin(); it != list.end(); ++it) {
#if DEBUG_LOG
            std::cout << "Class = " << (*it)->get_class_name() << " ";
            std::cout << "Path = " << (*it)->get_object_path() << " ";
            std::cout << "UUID = " << (*it)->get_uuid() << " ";
            std::cout << "Device = " << (*it)->get_device().get_object_path() << " ";
            std::cout << std::endl;
#endif
            //check for service 
            if((*it)->get_uuid() == OM2JCIEBU_BLE_LIVE_SENSOR_DATA_SERVICE) {
                bleService = (*it).release();
            }
        }
        break;
    }
    if(bleService != nullptr) {
        auto charList = bleService->get_characteristics();
        std::cout << "Discovered characteristics: " << uuid << std::endl;
        for(auto it = charList.begin(); it != charList.end(); ++it) {
#if DEBUG_LOG
            std::cout << "Class = " << (*it)->get_class_name() << " ";
            std::cout << "Path = " << (*it)->get_object_path() << " ";
            std::cout << "UUID = " << (*it)->get_uuid() << " ";
            std::cout << "Service = " << (*it)->get_service().get_object_path() << " ";
            std::cout << std::endl;
#endif
            //compare UUID 
            if((*it)->get_uuid() == uuid) {
                bleSensorChar = (*it).release();
                return SUCCESS;
            }
        }
    } else {
        //If not find any sevice then disconnect BLE device
        removeBleDevice();
        throw std::runtime_error(std::string(__FUNCTION__) +
                                 ": Could not find characteristics ");
    }
    if(bleSensorChar == nullptr) {
        //If not find any characteristics then disconnect BLE device
        removeBleDevice();
        throw std::runtime_error(std::string(__FUNCTION__) +
                                 ": Could not find characteristics ");
    }
    return FAILURE;
}


OM2JCIEBU_BLE::OM2JCIEBU_ERROR_T OM2JCIEBU_BLE::getSensorData(OM2JCIEBU::OM2JCIEBU_ATTRIBUTE_T attribute_name, void *attribute_data)
{
    if(attribute_data == NULL) {
        std::cout << "Null pointer received..." << std::endl;
        return FAILURE;
    }
    unsigned char *data;
    OM2JCIEBU_BLE::OM2JCIEBU_ERROR_T verifyResult = FAILURE;
    //Read raw data from connected BLE device based on characteristics
    std::vector<unsigned char> response = bleSensorChar->read_value();
    unsigned int size = response.size();
    if(size > 0) {
        data = response.data();
#if DEBUG_LOG
        std::cout << "Raw data=[";
        for(unsigned i = 0; i < response.size(); i++)
            std::cout << std::hex << static_cast<int>(data[i]) << ", ";
        std::cout << "] ";
        std::cout << "" << std::endl;
#endif
        //parse a data which is read from connected device
        parseSensorData(data);
        //Get data from structer
        getSensorAttribute(attribute_name, attribute_data);
        verifyResult = SUCCESS;
    }
    return verifyResult;
}

OM2JCIEBU_BLE::OM2JCIEBU_ERROR_T OM2JCIEBU_BLE::getAdvSensorData(OM2JCIEBU::OM2JCIEBU_ATTRIBUTE_T attribute_name, void *attribute_data)
{
    if(attribute_data == NULL) {
        std::cout << "Null pointer received..." << std::endl;
        return FAILURE;
    }
    uint8_t advSensordata[MAX_SENSOR_DATA_SIZE] = {0};
    int advDataindex = 0;
    OM2JCIEBU_BLE::OM2JCIEBU_ERROR_T verifyResult = FAILURE;
    if(is_BleConnected) { //disconnect with device if connected
        removeBleDevice();
        is_BleConnected = false;
    }

    if(startBleDiscovery()) {
        auto list = bleManager->get_devices();

        for(auto it = list.begin(); it != list.end(); ++it) {
			//read manufacturer data
            auto mfg = (*it)->get_manufacturer_data();
            if(!mfg.empty()) {
                //read manufacturer data for particular MAC address which is given by user
                if((*it)->get_address() == bleMACaddress) {
#if DEBUG_LOG
                    std::cout << "MFG" << std::endl;
                    for(auto it : mfg) {
                        std::cout << it.second.size() << std::endl;
                        std::cout << "\t" << it.first << " = [ ";
                        for(auto arr_it : it.second) {
                            std::cout << std::hex << (int) arr_it << ", ";
                        }
                        std::cout << "]" << std::endl;
                    }
#endif
                    //store in buffer for verify data and parsing 
                    for(auto it : mfg) {
                        advSensordata[advDataindex++] = it.first;
                        advSensordata[advDataindex++] = it.first >> 8;
                        for(auto arr_it : it.second) {
                            advSensordata[advDataindex++] = arr_it;
                        }
                    }
                    //verfiy a packet
                    verifyResult = verifyPacket(advSensordata, advDataindex);
                    if(verifyResult == SUCCESS) {
                        parseAdvSensorData(advSensordata);
                        getSensorAttribute(attribute_name, attribute_data);
                        advDataindex = 0;
                    }
                }
            }
        }
    }
    return verifyResult;
}

void OM2JCIEBU_BLE::getSensorAttribute(OM2JCIEBU::OM2JCIEBU_ATTRIBUTE_T attribute_name, void *attributeValue)
{
    if(attributeValue == NULL) {
        std::cout << "Null pointer received..." << std::endl;
        return;
    }
    switch(attribute_name) {
        case ALL_PARAM:
            memcpy(attributeValue, &om2jciebuData_ble, sizeof(om2jciebuData_ble));
            break;
        case TEMP:
            *(int16_t *) attributeValue = om2jciebuData_ble.temperature;
            break;
        case HUMIDITY:
            *(int16_t *) attributeValue = om2jciebuData_ble.relative_humidity;
            break;
        case AMBIENT_LIGHT:
            *(int16_t *) attributeValue = om2jciebuData_ble.ambient_light;
            break;
        case PRESSURE:
            *(int32_t *) attributeValue = om2jciebuData_ble.pressure;
            break;
        case NOISE:
            *(int16_t *) attributeValue = om2jciebuData_ble.noise;
            break;
        case ETVOC:
            *(int16_t *) attributeValue = om2jciebuData_ble.eTVOC;
            break;
        case ECO2:
            *(int16_t *) attributeValue = om2jciebuData_ble.eCO2;
            break;
        case DISCOMFORT_INDEX:
            break;
        case HEAT_STROKE:
            break;
        case LED_CONFIGURE:
            break;
        case ADV_CONFIGURE:
            break;
    }
}

OM2JCIEBU_BLE::OM2JCIEBU_ERROR_T OM2JCIEBU_BLE::verifyPacket(uint8_t *pkt, int len)
{
    if(pkt == NULL) {
        std::cout << "Null pointer received..." << std::endl;
        return FAILURE;
    }
    OM2JCIEBU_BLE::OM2JCIEBU_ERROR_T verifyResult = FAILURE;
    if(pkt[OM2JCIEBU_BLE_COM_ID_INDEX] == 0xD5) { //Check for Omron company ID
        if(pkt[OM2JCIEBU_BLE_COM_ID_INDEX + 1] == 0x02) {
            if(pkt[OM2JCIEBU_BLE_DATA_TYPE_INDEX] == 0x01) { // Check for sensor data type
                verifyResult = SUCCESS;
            } else {
                printf("Unable to get sensor data in advertisement packet,Please check advertisement setting\r\n");
                verifyResult = ERROR_UNKNOWN ;
            }
        }
    } else {
        printf("Unable to get sensor data in advertisement packet,Please check advertisement setting\r\n");
        verifyResult = ERROR_UNKNOWN ;
    }
    return verifyResult;
}

void OM2JCIEBU_BLE::configureSensorLedState(OM2JCIEBU::OM2JCIEBU_LED_SCALE_T state, uint8_t red, uint8_t green, uint8_t blue)
{
    std::vector<unsigned char> led_scale;

    led_scale.push_back(state);
    led_scale.push_back(0x00);
    led_scale.push_back(red);
    led_scale.push_back(green);
    led_scale.push_back(blue);

    writePacket(LED_CONFIGURE, led_scale);
}

void OM2JCIEBU_BLE::configureSensorAdvSetting(uint16_t milliseconds, OM2JCIEBU::OM2JCIEBU_ADV_PARAM_T adv_mode)
{
    std::vector<unsigned char> advSetting;
    uint16_t interval = 0;
    interval = milliseconds / OM2JCIEBU_INTERVAL_UNIT;/*calculate interval which is given by user using interval unit */

    advSetting.push_back(interval & 0x00FF);
    advSetting.push_back(interval >> 8);
    advSetting.push_back(adv_mode);

    writePacket(ADV_CONFIGURE, advSetting);
}

void OM2JCIEBU_BLE::parseSensorData(uint8_t *data)
{
	/* pasre and calculate sensor data and store in sensor data struct*/
    if(data == NULL) {
        std::cout << "Null pointer received..." << std::endl;
        return;
    }
    om2jciebuData_ble.sequence_number = data[0];

    om2jciebuData_ble.temperature = data[1] | data[2] << 8;
    om2jciebuData_ble.temperature = om2jciebuData_ble.temperature / 100;

    om2jciebuData_ble.relative_humidity = data[3] | data[4] << 8;
    om2jciebuData_ble.relative_humidity = om2jciebuData_ble.relative_humidity / 100;

    om2jciebuData_ble.ambient_light = data[5] | data[6] << 8;

    om2jciebuData_ble.pressure = data[7] | data[8] << 8 | data[9] << 16 | data[10] << 24;
    om2jciebuData_ble.pressure = om2jciebuData_ble.pressure / 1000;

    om2jciebuData_ble.noise = data[11] | data[12] << 8;
    om2jciebuData_ble.noise = om2jciebuData_ble.noise / 100;

    om2jciebuData_ble.eTVOC = data[13] | data[14] << 8;

    om2jciebuData_ble.eCO2 = data[15] | data[16] << 8;
}

void OM2JCIEBU_BLE::parseAdvSensorData(uint8_t *data)
{
	/* pasre and calculate Advertisement data and store in sensor data struct*/
    if(data == NULL) {
        std::cout << "Null pointer received..." << std::endl;
        return;
    }
    om2jciebuData_ble.sequence_number = data[3];

    om2jciebuData_ble.temperature = data[4] | data[5] << 8;
    om2jciebuData_ble.temperature = om2jciebuData_ble.temperature / 100;

    om2jciebuData_ble.relative_humidity = data[6] | data[7] << 8;
    om2jciebuData_ble.relative_humidity = om2jciebuData_ble.relative_humidity / 100;

    om2jciebuData_ble.ambient_light = data[8] | data[9] << 8;

    om2jciebuData_ble.pressure = data[10] | data[11] << 8 | data[12] << 16 | data[13] << 24;
    om2jciebuData_ble.pressure = om2jciebuData_ble.pressure / 1000;

    om2jciebuData_ble.noise = data[14] | data[15] << 8;
    om2jciebuData_ble.noise = om2jciebuData_ble.noise / 100;

    om2jciebuData_ble.eTVOC = data[16] | data[17] << 8;

    om2jciebuData_ble.eCO2 = data[18] | data[19] << 8;
}


