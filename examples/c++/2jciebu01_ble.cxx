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

/* standard headers */
#include <iostream>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>


/* omron sensor headers */
#include "2jciebu01_ble.hpp"

using namespace std;
using namespace upm;

#define PREVIOUS_MENU_CHOICE           8

volatile sig_atomic_t flag = 1;


upm::OM2JCIEBU_BLE::om2jciebuData_t om2jciebuSensorData;

void
sig_handler(int signum)
{
    if(signum == SIGABRT) {
        std::cout << "Exiting..." << std::endl;
    }
    if(signum == SIGINT) {
        flag = 0;
    }
}


void getSensorData(OM2JCIEBU_BLE *p_om2jcieble)
{

    if(p_om2jcieble == NULL) {
        std::cout << "Null pointer received..." << std::endl;
        return;
    }
    uint16_t parameterChoice = 0;
    int displayDelay = 0;
    uint16_t sensorParamData = 0;
    uint32_t pressureData = 0;
    bool seconds_validate = false;

    p_om2jcieble->getDiscoveredServices(OM2JCIEBU_BLE::ALL_PARAM);
    while(true) {
        printf("************************************************************\r\n");
        printf("Please select sensor attribute for display\r\n");
        printf("0) All parameter\r\n");
        printf("1) Temperature data\r\n");
        printf("2) Relative humidity data\r\n");
        printf("3) Ambient light data\r\n");
        printf("4) Barometric pressure data\r\n");
        printf("5) Sound noise data\r\n");
        printf("6) eTVOC data\r\n");
        printf("7) eCO2 data\r\n");
        printf("8) Return to main menu\r\n");
        printf("Note :: Press Ctrl+C for sensor attribute display menu\r\n");
        printf("************************************************************\r\n");
        while(!(std::cin >> parameterChoice)) {
            std::cin.clear(); //clear bad input flag
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); //discard input
            std::cout << "Invalid input; please re-enter.\n";
        }
        if(parameterChoice >= OM2JCIEBU_BLE::ALL_PARAM && parameterChoice <= OM2JCIEBU_BLE::ECO2) {
            flag = 1;
            printf("Please enter time interval (in Seconds), for display sensor data\r\n");
            while(!seconds_validate) {
                cin >> displayDelay;
                if(!cin.fail() && (cin.peek() == EOF || cin.peek() == '\n') && (displayDelay >= 1 && displayDelay <= 10)) {
                    seconds_validate = true;
                } else {
                    cin.clear();
                    cin.ignore();
                    cout << "Error, enter an second between 1 to 10!" << endl;
                }
            }
            while(flag) {
                switch(parameterChoice) {
                    case OM2JCIEBU_BLE::ALL_PARAM:
                        if(p_om2jcieble->getSensorData(OM2JCIEBU_BLE::ALL_PARAM, &om2jciebuSensorData) == OM2JCIEBU_BLE::SUCCESS) {
                            printf("**************  Sensor Attribute Values ***************\r\n");
                            printf("Sequence Number     :: %d \r\n", om2jciebuSensorData.sequence_number);
                            printf("Temperature         :: %d degC\r\n", om2jciebuSensorData.temperature);
                            printf("Relative humidity   :: %d RH\r\n", om2jciebuSensorData.relative_humidity);
                            printf("Ambient light       :: %d lx\r\n", om2jciebuSensorData.ambient_light);
                            printf("Barometric pressure :: %d hPa\r\n", om2jciebuSensorData.pressure);
                            printf("Sound noise         :: %d dB\r\n", om2jciebuSensorData.noise);
                            printf("eTVOC               :: %d ppb\r\n", om2jciebuSensorData.eTVOC);
                            printf("eCO2                :: %d ppm\r\n", om2jciebuSensorData.eCO2);
                            printf("**********************************************************\r\n");
                            memset(&om2jciebuSensorData, 0, sizeof(om2jciebuSensorData));
                        } else {
                            flag = 0;
                        }
                        break;
                    case OM2JCIEBU_BLE::TEMP:
                        if(p_om2jcieble->getSensorData(OM2JCIEBU_BLE::TEMP, &sensorParamData) == OM2JCIEBU_BLE::SUCCESS) {
                            printf("**************  Temperature Attribute Values ***************\r\n");
                            printf("Temperature         :: %d degC\r\n", sensorParamData);
                            printf("************************************************************\r\n");
                        } else {
                            flag = 0;
                        }
                        break;
                    case OM2JCIEBU_BLE::HUMIDITY:
                        if(p_om2jcieble->getSensorData(OM2JCIEBU_BLE::HUMIDITY, &sensorParamData) == OM2JCIEBU_BLE::SUCCESS) {
                            printf("**************  Relative humidity Attribute Values ***************\r\n");
                            printf("Relative humidity   :: %d RH\r\n", sensorParamData);
                            printf("******************************************************************\r\n");
                        } else {
                            flag = 0;
                        }
                        break;
                    case OM2JCIEBU_BLE::AMBIENT_LIGHT:
                        if(p_om2jcieble->getSensorData(OM2JCIEBU_BLE::AMBIENT_LIGHT, &sensorParamData) == OM2JCIEBU_BLE::SUCCESS) {
                            printf("**************  Ambient light Attribute Values ***************\r\n");
                            printf("Ambient light       :: %d lx\r\n", sensorParamData);
                            printf("***************************************************************\r\n");
                        } else {
                            flag = 0;
                        }
                        break;
                    case OM2JCIEBU_BLE::PRESSURE:
                        if(p_om2jcieble->getSensorData(OM2JCIEBU_BLE::PRESSURE, &pressureData) == OM2JCIEBU_BLE::SUCCESS) {
                            printf("**************  Barometric pressure Attribute Values ***************\r\n");
                            printf("Barometric pressure :: %d hPa\r\n", pressureData);
                            printf("********************************************************************\r\n");
                        }
                        break;

                    case OM2JCIEBU_BLE::NOISE:
                        if(p_om2jcieble->getSensorData(OM2JCIEBU_BLE::NOISE, &sensorParamData) == OM2JCIEBU_BLE::SUCCESS) {
                            printf("**************  Sound noise Attribute Values ***************\r\n");
                            printf("Sound noise         :: %d dB\r\n", sensorParamData);
                            printf("*************************************************************\r\n");
                        } else {
                            flag = 0;
                        }
                        break;

                    case OM2JCIEBU_BLE::ETVOC:
                        if(p_om2jcieble->getSensorData(OM2JCIEBU_BLE::ETVOC, &sensorParamData) == OM2JCIEBU_BLE::SUCCESS) {
                            printf("**************  eTVOC Attribute Values ***************\r\n");
                            printf("eTVOC               :: %d ppb\r\n", sensorParamData);
                            printf("*******************************************************\r\n");
                        } else {
                            flag = 0;
                        }
                        break;
                    case OM2JCIEBU_BLE::ECO2:
                        if(p_om2jcieble->getSensorData(OM2JCIEBU_BLE::ECO2, &sensorParamData) == OM2JCIEBU_BLE::SUCCESS) {
                            printf("**************  eCO2 Attribute Values ***************\r\n");
                            printf("eCO2                :: %d ppm\r\n\r\n", sensorParamData);
                            printf("******************************************************\r\n");
                        } else {
                            flag = 0;
                        }
                        break;
                }
                printf("\r\n");
                p_om2jcieble->delay(displayDelay);
            }
        } else if(parameterChoice == PREVIOUS_MENU_CHOICE) {
            break;
        } else {
            printf("Invalid choice\r\n");
        }
        seconds_validate = false;
    }
}

void getAdvSensorData(OM2JCIEBU_BLE *p_om2jcieble)
{
    if(p_om2jcieble == NULL) {
        std::cout << "Null pointer received..." << std::endl;
        return;
    }

    uint16_t parameterChoice = 0, sensorParamData = 0;
    int displayDelay = 0;
    uint32_t pressureData = 0;
    bool seconds_validate = false;

    while(true) {
        printf("************************************************************\r\n");
        printf("Please select sensor attribute for display\r\n");
        printf("0) All parameter\r\n");
        printf("1) Temperature data\r\n");
        printf("2) Relative humidity data\r\n");
        printf("3) Ambient light data\r\n");
        printf("4) Barometric pressure data\r\n");
        printf("5) Sound noise data\r\n");
        printf("6) eTVOC data\r\n");
        printf("7) eCO2 data\r\n");
        printf("8) Return to main menu\r\n");
        printf("Note :: Press Ctrl+C for sensor attribute display menu\r\n");
        printf("************************************************************\r\n");
        while(!(std::cin >> parameterChoice)) {
            std::cin.clear(); //clear bad input flag
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); //discard input
            std::cout << "Invalid input; please re-enter.\n";
        }
        if(parameterChoice >= OM2JCIEBU_BLE::ALL_PARAM && parameterChoice <= OM2JCIEBU_BLE::ECO2) {
            flag = 1;
            printf("Please enter time interval (in Seconds), for display sensor data\r\n");
            while(!seconds_validate) {
                cin >> displayDelay;
                if(!cin.fail() && (cin.peek() == EOF || cin.peek() == '\n') && (displayDelay >= 1 && displayDelay <= 10)) {
                    seconds_validate = true;
                } else {
                    cin.clear();
                    cin.ignore();
                    cout << "Error, enter an second between 1 and 10!" << endl;
                }
            }
            while(flag) {
                switch(parameterChoice) {
                    case OM2JCIEBU_BLE::ALL_PARAM:
                        if(p_om2jcieble->getAdvSensorData(OM2JCIEBU_BLE::ALL_PARAM, &om2jciebuSensorData) == OM2JCIEBU_BLE::SUCCESS) {
                            printf("**************  Advertisement Sensor Attribute Values ***************\r\n");
                            printf("Sequence Number     :: %d \r\n", om2jciebuSensorData.sequence_number);
                            printf("Temperature         :: %d degC\r\n", om2jciebuSensorData.temperature);
                            printf("Relative humidity   :: %d RH\r\n", om2jciebuSensorData.relative_humidity);
                            printf("Ambient light       :: %d lx\r\n", om2jciebuSensorData.ambient_light);
                            printf("Barometric pressure :: %d hPa\r\n", om2jciebuSensorData.pressure);
                            printf("Sound noise         :: %d dB\r\n", om2jciebuSensorData.noise);
                            printf("eTVOC               :: %d ppb\r\n", om2jciebuSensorData.eTVOC);
                            printf("eCO2                :: %d ppm\r\n", om2jciebuSensorData.eCO2);
                            printf("*********************************************************************\r\n");
                            memset(&om2jciebuSensorData, 0, sizeof(om2jciebuSensorData));
                        } else {
                            flag = 0;
                        }
                        break;
                    case OM2JCIEBU_BLE::TEMP:
                        if(p_om2jcieble->getAdvSensorData(OM2JCIEBU_BLE::TEMP, &sensorParamData) == OM2JCIEBU_BLE::SUCCESS) {
                            printf("**************  Advertisement Temperature Attribute Values ***************\r\n");
                            printf("Temperature         :: %d degC\r\n", sensorParamData);
                            printf("**************************************************************************\r\n");
                        } else {
                            flag = 0;
                        }
                        break;
                    case OM2JCIEBU_BLE::HUMIDITY:
                        if(p_om2jcieble->getAdvSensorData(OM2JCIEBU_BLE::HUMIDITY, &sensorParamData) == OM2JCIEBU_BLE::SUCCESS) {
                            printf("**************  Advertisement Relative humidity Attribute Values ***************\r\n");
                            printf("Relative humidity   :: %d RH\r\n", sensorParamData);
                            printf("********************************************************************************\r\n");
                        } else {
                            flag = 0;
                        }
                        break;
                    case OM2JCIEBU_BLE::AMBIENT_LIGHT:
                        if(p_om2jcieble->getAdvSensorData(OM2JCIEBU_BLE::AMBIENT_LIGHT, &sensorParamData) == OM2JCIEBU_BLE::SUCCESS) {
                            printf("************** Advertisement Ambient light Attribute Values ***************\r\n");
                            printf("Ambient light       :: %d lx\r\n", sensorParamData);
                            printf("***************************************************************************\r\n");
                        } else {
                            flag = 0;
                        }
                        break;
                    case OM2JCIEBU_BLE::PRESSURE:
                        if(p_om2jcieble->getAdvSensorData(OM2JCIEBU_BLE::PRESSURE, &pressureData) == OM2JCIEBU_BLE::SUCCESS) {
                            printf("************** Advertisement Barometric pressure Attribute Values ***************\r\n");
                            printf("Barometric pressure :: %d hPa\r\n", pressureData);
                            printf("*********************************************************************************\r\n");
                        } else {
                            flag = 0;
                        }
                        break;

                    case OM2JCIEBU_BLE::NOISE:
                        if(p_om2jcieble->getAdvSensorData(OM2JCIEBU_BLE::NOISE, &sensorParamData) == OM2JCIEBU_BLE::SUCCESS) {
                            printf("************** Advertisement Sound noise Attribute Values ***************\r\n");
                            printf("Sound noise         :: %d dB\r\n", sensorParamData);
                            printf("*************************************************************************\r\n");
                        } else {
                            flag = 0;
                        }
                        break;

                    case OM2JCIEBU_BLE::ETVOC:
                        if(p_om2jcieble->getAdvSensorData(OM2JCIEBU_BLE::ETVOC, &sensorParamData) == OM2JCIEBU_BLE::SUCCESS) {
                            printf("**************** Advertisement eTVOC Attribute Values ******************\r\n");
                            printf("eTVOC               :: %d ppb\r\n", sensorParamData);
                            printf("************************************************************************\r\n");
                        } else {
                            flag = 0;
                        }
                        break;
                    case OM2JCIEBU_BLE::ECO2:
                        if(p_om2jcieble->getAdvSensorData(OM2JCIEBU_BLE::ECO2, &sensorParamData) == OM2JCIEBU_BLE::SUCCESS) {
                            printf("************** Advertisement eCO2 Attribute Values ***************\r\n");
                            printf("eCO2                :: %d ppm\r\n\r\n", sensorParamData);
                            printf("******************************************************************\r\n");
                        } else {
                            flag = 0;
                        }
                        break;
                }
                printf("\r\n");
                p_om2jcieble->delay(displayDelay);
            }
        } else if(parameterChoice == PREVIOUS_MENU_CHOICE) {
            break;
        } else {
            printf("Invalid choice\r\n");
        }
        seconds_validate = false;
    }
}

void configureLEDSetting(OM2JCIEBU_BLE *p_om2jcieble)
{
    if(p_om2jcieble == NULL) {
        std::cout << "Null pointer received..." << std::endl;
        return;
    }
    int led_choice = 0;
    unsigned short int red_scale = 0, green_scale = 0, blue_scale = 0;
    bool red_scale_validate = false, green_scale_validate = false, blue_scale_validate = false;
    printf("**************  Sensor LED Configuration ***************\r\n");
    printf("Please select a operation for LED\r\n");
    printf("0) Normally OFF\r\n");
    printf("1) Normally ON\r\n");
    printf("2) Temperature value scales\r\n");
    printf("3) Relative humidity value scales\r\n");
    printf("4) Ambient light value scales\r\n");
    printf("5) Barometric pressure value scales\r\n");
    printf("6) Sound noise value scales\r\n");
    printf("7) eTVOC value scales\r\n");
    printf("8) SI vale scales\r\n");
    printf("9) PGA value scales\r\n");
    printf("**********************************************************\r\n");
    while(!(std::cin >> led_choice)) {
        std::cin.clear(); //clear bad input flag
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); //discard input
        std::cout << "Invalid input; please re-enter.\n";
    }

    if(led_choice == OM2JCIEBU_BLE::NORMALLY_ON) {
        printf("Please select a LED color scale\r\n");
        printf("Please enter Red Color scale (scale range 0 to 255)\r\n");
        while(!red_scale_validate) {
            cin >> red_scale;
            if(!cin.fail() && (cin.peek() == EOF || cin.peek() == '\n') && (red_scale >= 0 && red_scale <= 255)) {
                red_scale_validate = true;
            } else {
                cin.clear();
                cin.ignore();
                cout << "Error, enter an red color scale between 0 and 255!" << endl;
            }
        }
        printf("Please enter Green Color scale(scale range 0 to 255)\r\n");
        while(!green_scale_validate) {
            cin >> green_scale;
            if(!cin.fail() && (cin.peek() == EOF || cin.peek() == '\n') && (green_scale >= 0 && green_scale <= 255)) {
                green_scale_validate = true;
            } else {
                cin.clear();
                cin.ignore();
                cout << "Error, enter an green color scale between 0 and 255!" << endl;
            }
        }
        printf("Please enter Blue Color scale(scale range 0 to 255)\r\n");
        while(!blue_scale_validate) {
            cin >> blue_scale;
            if(!cin.fail() && (cin.peek() == EOF || cin.peek() == '\n') && (blue_scale >= 0 && blue_scale <= 255)) {
                blue_scale_validate = true;
            } else {
                cin.clear();
                cin.ignore();
                cout << "Error, enter an blue color scale between 0 and 255!" << endl;
            }
        }
        p_om2jcieble->configureSensorLedState(OM2JCIEBU_BLE::NORMALLY_ON, red_scale, green_scale, blue_scale);
    } else {
        switch(led_choice) {
            case OM2JCIEBU_BLE::NORMALLY_OFF:
                p_om2jcieble->configureSensorLedState(OM2JCIEBU_BLE::NORMALLY_OFF, 0, 0, 0);
                break;
            case OM2JCIEBU_BLE::TEMP_SACLE:
                p_om2jcieble->configureSensorLedState(OM2JCIEBU_BLE::TEMP_SACLE, 0, 0, 0);
                break;
            case OM2JCIEBU_BLE::HUMIDITY_SCALE:
                p_om2jcieble->configureSensorLedState(OM2JCIEBU_BLE::HUMIDITY_SCALE, 0, 0, 0);
                break;
            case OM2JCIEBU_BLE::AMBIENT_LIGHT_SCALE:
                p_om2jcieble->configureSensorLedState(OM2JCIEBU_BLE::AMBIENT_LIGHT_SCALE, 0, 0, 0);
                break;
            case OM2JCIEBU_BLE::PRESSURE_SCALE:
                p_om2jcieble->configureSensorLedState(OM2JCIEBU_BLE::PRESSURE_SCALE, 0, 0, 0);
                break;
            case OM2JCIEBU_BLE::NOISE_SCALE:
                p_om2jcieble->configureSensorLedState(OM2JCIEBU_BLE::NOISE_SCALE, 0, 0, 0);
                break;
            case OM2JCIEBU_BLE::ETVOC_SCALE:
                p_om2jcieble->configureSensorLedState(OM2JCIEBU_BLE::ETVOC_SCALE, 0, 0, 0);
                break;
            case OM2JCIEBU_BLE::SI_SCALE:
                p_om2jcieble->configureSensorLedState(OM2JCIEBU_BLE::SI_SCALE, 0, 0, 0);
                break;
            case OM2JCIEBU_BLE::PGA_SCALE:
                p_om2jcieble->configureSensorLedState(OM2JCIEBU_BLE::PGA_SCALE, 0, 0, 0);
                break;
            default:
                std::cout << "Wrong LED scale choice please try again" << std::endl;
        }
    }
}

void configureAdvInterval(OM2JCIEBU_BLE *p_om2jcieble)
{
    if(p_om2jcieble == NULL) {
        std::cout << "Null pointer received..." << std::endl;
        return;
    }
    uint16_t millisecond = 0;
    bool milisecond_validate = false;
    int advertising_mode = 0;
    printf("**************  Sensor Advertise Configuration ***************\r\n");
    printf("Please enter time interval (in Milliseconds), for changing Advertise interval, between 100 to 10240 milliseconds\r\n");
    while(!milisecond_validate) {
        cin >> millisecond;
        if(!cin.fail() && (cin.peek() == EOF || cin.peek() == '\n') && (millisecond >= 100 && millisecond <= 10240)) {
            milisecond_validate = true;
        } else {
            cin.clear();
            cin.ignore();
            cout << "Error, enter an millisecond between 100 and 10240!" << endl;
        }
    }
    printf("Please select an Advertise mode with the selected Advertise interval \r\n");
    printf("1) Sensor data\r\n");
    printf("2) Calculation data\r\n");
    printf("3) Sensor data and Calculation data\r\n");
    printf("4) Sensor flag and Calculation flag\r\n");
    printf("5) Serial number\r\n");
    while(!(std::cin >> advertising_mode)) {
        std::cin.clear(); //clear bad input flag
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); //discard input
        std::cout << "Invalid input; please re-enter.\n";
    }
    switch(advertising_mode) {
        case OM2JCIEBU_BLE::SENSOR_DATA:
            p_om2jcieble->configureSensorAdvSetting(millisecond, OM2JCIEBU_BLE::SENSOR_DATA);
            break;
        case OM2JCIEBU_BLE::ACCELERATION_DATA:
            p_om2jcieble->configureSensorAdvSetting(millisecond, OM2JCIEBU_BLE::ACCELERATION_DATA);
            break;
        case OM2JCIEBU_BLE::ACCELERATION_SENSOR_DATA:
            p_om2jcieble->configureSensorAdvSetting(millisecond, OM2JCIEBU_BLE::ACCELERATION_SENSOR_DATA);
            break;
        case OM2JCIEBU_BLE::ACCELERATION_SENSOR_FLAG:
            p_om2jcieble->configureSensorAdvSetting(millisecond, OM2JCIEBU_BLE::ACCELERATION_SENSOR_FLAG);
            break;
        case OM2JCIEBU_BLE::SERIAL_NUMBER:
            p_om2jcieble->configureSensorAdvSetting(millisecond, OM2JCIEBU_BLE::SERIAL_NUMBER);
            break;
        default:
            std::cout << "Invalid choice\n";
    }
    printf("**************************************************************\r\n");
}

int
main(int argc, char *argv[])
{
    int operation_choice = 0;
    if(argc <= 1) {
        std::cout << "usage ./a.out xx:xx:xx:xx:xx:xx" << std::endl;
        return 0;
    }
    signal(SIGABRT, sig_handler);
    signal(SIGINT, sig_handler);
    upm::OM2JCIEBU_BLE om2jciebu_ble(argv[1]);
    while(true) {
        std::cout << "*************************************************************" << std::endl;
        std::cout << "Please choose one option for Omron sensor operation" << std::endl;
        std::cout << "1) Display Advertisement Sensor attriutes" << std::endl;
        std::cout << "2) Display Sensor attriutes" << std::endl;
        std::cout << "3) Configure LED setting " << std::endl;
        std::cout << "4) Configure advertise setting" << std::endl;
        std::cout << "5) Exit" << std::endl;
        std::cout << "*************************************************************" << std::endl;
        while(!(std::cin >> operation_choice)) {
            std::cin.clear(); //clear bad input flag
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); //discard input
            std::cout << "Invalid input; please re-enter.\n";
        }

        switch(operation_choice) {
            case 1:
                getAdvSensorData(&om2jciebu_ble);
                break;
            case 2:
                getSensorData(&om2jciebu_ble);
                break;
            case 3:
                configureLEDSetting(&om2jciebu_ble);
                break;
            case 4:
                configureAdvInterval(&om2jciebu_ble);
                break;
            case 5:
                std::cout << "Application Exited" << std::endl;
                om2jciebu_ble.removeBleDevice();
                exit(0);
                break;
            default:
                std::cout << "Invalid choice" << std::endl;
                break;
        }
    }
}
