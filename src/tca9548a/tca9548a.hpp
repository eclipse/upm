/*
 * Author: Gérard Vidal <gerard.vidal@ens-lyon.fr>
 * Copyright (c) 2017 IFÉ-ENS-Lyon
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
 * LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
 * OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
 * WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#pragma once

#include <iostream>
#include <string>

#define TCA9548A_DEFAULT_ADDR 0x70
#define TCA9548A_SLAVE1_ADDR 0x71
#define TCA9548A_SLAVE2_ADDR 0x72
#define TCA9548A_SLAVE3_ADDR 0x73
#define TCA9548A_SLAVE4_ADDR 0x74
#define TCA9548A_SLAVE5_ADDR 0x75
#define TCA9548A_SLAVE6_ADDR 0x76
#define TCA9548A_SLAVE7_ADDR 0x77

namespace mraa { class I2c;}

namespace upm {
  /**
   * @brief tca9548a multiplexer library
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
   * The TCA9548A Multiplexer Breakout enables to get - up to 8 same-address I2C devices hooked up 
   * to one microcontroller - or up to 8 independent I2C buses. This multiplexer acts as a gatekeeper, 
   * shuttling the commands to the selected set of I2C pins with your command. The TCA9548A 
   * multiplexer is interesting in that it has an I2C address (0x70 by default) - and you basically 
   * send it a command to tell it which I2C multiplexed output you want to talk to, then you can 
   * address the board you want to address. You simply write a single byte with the desired multiplexed 
   * output number to that port, and bam - any future I2C packets will get sent to that port. 
   * The TCA9548A Multiplexer provides three pins (A0-A1-A2) that enable to change its address in
   * case 0x70 address is used by another sensor on the same bus. By connecting one of the Ax pin to Vin you 
   * change its value from 0 to 1, these values change the value of the three first bits of the address :
   * Address 0b01110-A2-A1-A0
   * 	- No wiring  A2=0 A1=0 A0=0  Address 0b01110000 0x70
   *    - A0 wired   A2=0 A1=0 A0=1  Address 0b01110000 0x71
   *    - A2 wired   A2=1 A1=0 A0=0  Address 0b01110100 0x74
   * Any address between 0x70 and 0x77 can be selected.
   *
   * Tested with Adafriut TCA9548A board.
   *
   * @image html tca9548a.jpg
   * @snippet tca9548.cxx Interesting
   */
    class TCA9548A {

        public:
            /**
             * @enum uint8_t 
             * @brief uint8_t enum containing chanel addresses
             * used for selecting slave to activate
             *
             * @var uint8_t ::RESET = 0x00
             * @var uint8_t ::C0 = 0x01
             * @var uint8_t ::C1 = 0x02
             * @var uint8_t ::C2 = 0x04
             * @var uint8_t ::C3 = 0x08
             * @var uint8_t ::C4 = 0x10
             * @var uint8_t ::C5 = 0x20
             * @var uint8_t ::C6 = 0x40
             * @var uint8_t ::C7 = 0x80
             */
             
            typedef enum
            {
                 RESET = 0x00,
                 C0 = 0x01,
                 C1 = 0x02,
                 C2 = 0x04,
                 C3 = 0x08,
                 C4 = 0x10,
                 C5 = 0x20,
                 C6 = 0x40,
                 C7 = 0x80
            } CHANNEL_REG ;
            

            /**
             * TCA9548A constructor
             *
             * @param bus i2c bus the sensor is attached to.
             * @param address. Device address. Default is 0x70.
             */
            TCA9548A (int bus, uint8_t address = 0x70);

            /**
             * TCA9548A destructor
             */
            ~TCA9548A ();

             /**
             * Returns the name of the sensor
             */
            std::string name()
            {
                return m_name;
            }

           /**
             *  selMultSlaves - select multiple slaves.
             *
             *  @param address. Device address. Default is 0x70.
             *  @param channels. list of chanels that will be
             *  opened for communication 
             */
            void selMultSlaves(uint8_t address, CHANNEL_REG channel1, CHANNEL_REG channel2, CHANNEL_REG channel3, CHANNEL_REG channel4, CHANNEL_REG channel5, CHANNEL_REG channel6, CHANNEL_REG channel7, CHANNEL_REG channel8);

            /**
             *  selSlave - select slave.
             *
             * @param address. Device address. Default is 0x70.
             * @param channel. Default is reset. 
             * Channel that will be opened for communication
             */
            void selSlave(uint8_t address, CHANNEL_REG  channel);
            
            /**
             *  closeSlaves - close all com to slaves.
             *
             * @param address. Device address. Default is 0x70.
             * @param channel. Default is reset. 
             * All channels are closed for communication
             */
            void closeSlaves (uint8_t address);
            
        private:

            std::string m_name;
            mraa::I2c* i2c;

    };
}
