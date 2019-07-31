/*
 * Author: Gerard Vidal <gerard.vidal@ens-lyon.fr>
 * Copyright (c) 2017 IFE-ENS-Lyon
 * Author: Keelan Lightfoot <keelanlightfoot@gmail.com>
 * Copyright (c) 2015 Intel Corporation.
 *
 * This program and the accompanying materials are made available under the
 * terms of the The MIT License which is available at
 * https://opensource.org/licenses/MIT.
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include <iostream>
#include <string>
#include "mraa.hpp"
#include "mraa/i2c.hpp"

#define TCA9548A_I2C_BUS 0
#define TCA9548A_DEFAULT_ADDR 0x70
#define TCA9548A_NO_PORTS 0x00
#define TCA9548A_ALL_PORTS 0xFF

#define TCA9548A_MIN_PORT 0
#define TCA9548A_MAX_PORT 7

namespace upm {
    /**
     * @brief TCA9548A I2C Multiplexer
     * @defgroup tca9548a libupm-tca9548a
     * @ingroup adafruit i2c
     */
    /**
     * @library tca9548a
     * @sensor TCA9548A
     * @comname 1-to-8 I2C Multiplexer Breakout
     * @type multiplexer
     * @man adafruit
     * @con i2c
     * @web https://learn.adafruit.com/adafruit-tca9548a-1-to-8-i2c-multiplexer-breakout/overview
     *
     * @brief API TCA9548A Multiplexer Breakout
     *
     * The TCA9548A device has eight bidirectional translating switches
     * that can be controlled through the I2C bus. The SCL/SDA upstream
     * pair fans out to eight downstream pairs, or channels. Any
     * individual SCn/SDn channel or combination of channels can be
     * selected, determined by the contents of the programmable control
     * register.
     *
     * The TCA9548A Multiplexer Breakout enables to get - up to 8
     * same-address I2C devices hooked up to one microcontroller - or up
     * to 8 independent I2C buses. This multiplexer acts as a
     * gatekeeper, shuttling the commands to the selected set of I2C
     * pins with your command. The TCA9548A multiplexer is interesting
     * in that it has an I2C address (0x70 by default) - and you
     * basically send it a command to tell it which I2C multiplexed
     * output you want to talk to, then you can address the board you
     * want to address. You simply write a single byte with the desired
     * multiplexed output number to that port, and bam - any future I2C
     * packets will get sent to that port.
     *
     * The TCA9548A Multiplexer provides three pins (A0-A1-A2) that
     * enable to change its address in case 0x70 address is used by
     * another sensor on the same bus. By connecting one of the Ax pin
     * to Vin you change its value from 0 to 1, these values change the
     * value of the three first bits of the address :
     *
     * Address 0b01110-A2-A1-A0
     *    No wiring  A2=0 A1=0 A0=0  Address 0b01110000 0x70
     *    A0 wired   A2=0 A1=0 A0=1  Address 0b01110000 0x71
     *    A2 wired   A2=1 A1=0 A0=0  Address 0b01110100 0x74
     * Any address between 0x70 and 0x77 can be selected.
     *
     * Tested with Adafriut TCA9548A board.
     *
     * @image html tca9548a.jpg
     * @snippet tca9548.cxx Interesting
     */
    class TCA9548A {

    public:
        /**
         * @enum TCA9548A_PORT_STATE
         * @brief boolean enum containing port state
         *
         * @var TCA9548A_PORT_STATE::DISABLED = 0
         * @var TCA9548A_PORT_STATE::ENABLED = 1
         */
        typedef enum {
            DISABLED = 0,
            ENABLED = 1
        } TCA9548A_PORT_STATE;

        /**
         * @enum TCA9548A_PORT_MODE
         * @brief boolean enum containing port access mode
         *
         * @var TCA9548A_PORT_MODE::EXCLUSIVE = 0
         * @var TCA9548A_PORT_MODE::INCLUSIVE = 1
         */
        typedef enum {
            EXCLUSIVE = 0,
            INCLUSIVE = 1
        } TCA9548A_PORT_MODE;


        /**
         * TCA9548A constructor
         *
         * @param address. Device address. Default is 0x70.
         */
        TCA9548A (int bus, uint8_t address = 0x70);

        /**
         * TCA9548A destructor
         */
        ~TCA9548A();

        /**
         * Returns the name of the switch
         */
        std::string name() {
            return m_name;
        }

        /**
         * Returns the status of a port as configured in the multiplexer.
         *
         *  @param port Switch port to check
         */
        bool getPort(int port);

        /**
         *  Configure an individual port on the multiplexer
         *
         *  @param port Port to configure
         *  @param enabled Set to true to enable the port, false to
         *  disable the port.
         *  @param exclusive Set to true to disable all other
         *  ports, false to leave existing port config
         *  untouched. When exclusive is set to false, an
         *  additional i2c read is required to read the current
         *  port setting.
         */
        void setPort(int port, TCA9548A_PORT_STATE state,
                     TCA9548A_PORT_MODE mode);

        /**
         *  Disables all ports on the multiplexer.
         */
        void disableAllPorts();

        /**
         *  Enables all ports on the multiplexer. Useful when the
         *  multiplexer is used to electrically extend the bus
         *  rather than resolve address conflicts.
         */
        void enableAllPorts();

    private:
        /* Disable implicit copy and assignment operators */
        TCA9548A(const TCA9548A&) = delete;
        TCA9548A &operator=(const TCA9548A&) = delete;

        std::string m_name;

        mraa::I2c* i2c;

        uint8_t getPortConfig();
        void setPortConfig(uint8_t config);
        bool validPort(int port);
    };
}
