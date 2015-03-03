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
 * @brief pca9685 based servo controller
 * @defgroup pca9685 libupm-pca9685
 */
 
 /**
  * @brief
  * UPM library for pca9685 Adafruit 16-channel servo shield/controller. When 3 or more GWS servos attached results unpredictable.
  * Adafruit do recommend a capacitor be installed on the board which should alleviate the issue. Sizing depends on servos and count.
  *
  * @ingroup i2c
  * @snippet pca9685ss.cxx Interesting
  */
  class pca9685ss {
  public:
    /**
     * Creates a pca9685ss object
     *
     * @param bus number of used i2c bus
     * @param i2c_address address of servo controller on i2c bus
     */
    pca9685ss(int bus, int i2c_address);
    int update(void);
    /**
     * Sets the frequency for your servos
     *
     * @param freq the frequency at which the servos operate
     */
    void setPWMFreq(float freq);
    /**
     * Moves the one of the servos to the specified angle
     *
     * @param port port of the servo on the controller (servo number)
     * @param servo_type can be 0 = standard 1ms to 2ms, 1 = extended 0.6ms to 2.4ms, or 2 = extended 0.8ms to 2.2ms
     * @param degrees angle to set the servo to
     */
    void servo(uint8_t port, uint8_t servo_type, uint16_t degrees);

  private:

    int pca9685_addr;
    mraa_i2c_context m_i2c;
    uint8_t m_rx_tx_buf[MAX_BUFFER_LENGTH];
    float _pwm_frequency;
    float _duration_1ms;
};

}
