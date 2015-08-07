/*
 * Author: Stan Gifford <stan@gifford.id.au>
 * Copyright (c) 2015 Intel Corporation.
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

#include <mraa/i2c.h>

#define MAX_BUFFER_LENGTH 6

#define SERVO_MIN       0
#define SERVO_MAX       180
#define SERVO_FREQ      60
#define PCA9685_SUBADR1 0x2
#define PCA9685_SUBADR2 0x3
#define PCA9685_SUBADR3 0x4

#define PCA9685_MODE1    0x0
#define PCA9685_PRESCALE 0xFE

#define LED0_ON_L  0x6
#define LED0_ON_H  0x7
#define LED0_OFF_L 0x8
#define LED0_OFF_H 0x9

#define ALLLED_ON_L  0xFA
#define ALLLED_ON_H  0xFB
#define ALLLED_OFF_L 0xFC
#define ALLLED_OFF_H 0xFD

#define PCA9685_MODE1_REG       0x00
#define PCA9685_PRESCALE_REG    0xFE
#define LED0_REG                0x06

namespace upm {

 /**
  * @brief Adafruit PCA9685-based Servo Shield library
  * @defgroup adafruitss libupm-adafruitss
  * @ingroup adafruit i2c servos
  */

 /**
  * @library adafruitss
  * @sensor adafruitss
  * @comname Adafruit Servo Shield
  * @type servos
  * @man adafruit
  * @web http://www.adafruit.com/product/1411
  * @con i2c
  *
  * @brief API for the Adafruit Servo Shield 
  *
  * UPM library for the PCA9685-based Adafruit 16-channel servo shield. If 3
  * or more GWS servos are attached, results could be unpredictable. Adafruit
  * Industries recommend installing a capacitor on the board, which should
  * alleviate the issue. Sizing depends on servos and their number.
  *
  * @image html adafruitss.jpg
  * @snippet adafruitss.cxx Interesting
  */

  class adafruitss {
  public:
    /**
     * Creates an adafruitss object
     *
     * @param bus Number of the used I2C bus
     * @param i2c_address Address of the servo shield on the I2C bus
     */
    adafruitss(int bus, int i2c_address);
    int update(void);
    /**
     * Sets the frequency of the servos
     *
     * @param freq Frequency at which the servos operate
     */
    void setPWMFreq(float freq);
    /**
     * Moves one of the servos to a specified angle
     *
     * @param port Port of the servo on the shield (servo number)
     * @param servo_type Can be 0 = standard (1ms to 2ms), 1 = extended
     * (0.6ms to 2.4ms), or 2 = extended (0.8ms to 2.2ms)
     * @param degrees Angle to set the servo to
     */
    void servo(uint8_t port, uint8_t servo_type, float degrees);

  private:

    int pca9685_addr;
    mraa_i2c_context m_i2c;
    uint8_t m_rx_tx_buf[MAX_BUFFER_LENGTH];
    float _duration_1ms;
};

}
