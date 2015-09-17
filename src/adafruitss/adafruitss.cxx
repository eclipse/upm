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

#include <string>
#include <stdexcept>

#include "adafruitss.h"
#include <unistd.h>
#include <math.h>

using namespace upm;

adafruitss::adafruitss(int bus,int i2c_address)
{
    if ( !(m_i2c = mraa_i2c_init(bus)) ) 
      {
        throw std::invalid_argument(std::string(__FUNCTION__) +
                                    ": mraa_i2c_init() failed");
        return;
      }

    pca9685_addr =  i2c_address;
    if (mraa_i2c_address(m_i2c, pca9685_addr) != MRAA_SUCCESS)
      {
        throw std::invalid_argument(std::string(__FUNCTION__) +
                                    ": mraa_i2c_address() failed");
        return;
      }

    m_rx_tx_buf[0]=PCA9685_MODE1;
    m_rx_tx_buf[1]=0;
    if (mraa_i2c_write(m_i2c,m_rx_tx_buf,2) != MRAA_SUCCESS)
      {
        throw std::invalid_argument(std::string(__FUNCTION__) +
                                    ": mraa_i2c_write() failed");
        return;
      }

    adafruitss::setPWMFreq(60);

    adafruitss::update();
}

void adafruitss::setPWMFreq(float freq) {
    float afreq= freq * 0.899683334F;  // Correct for overshoot in the frequency setting (see issue #11). (Tested at 60hz with Logic 4 for 50hz and 60hz)
    float prescaleval = 25000000;
    prescaleval /= 4096;
    prescaleval /= afreq;
    prescaleval -= 1;
    float pwm_frequency = freq; // Use actual requested frequency gives the correct pulse width

    _duration_1ms = ((4096*pwm_frequency)/1000);  // This is 1ms duration

    uint8_t prescale = roundf(prescaleval);



    mraa_i2c_address(m_i2c, pca9685_addr);
    mraa_i2c_read_byte_data(m_i2c,PCA9685_MODE1);


    m_rx_tx_buf[0]=PCA9685_MODE1;
    m_rx_tx_buf[1]=0x10; // sleep
    mraa_i2c_address(m_i2c, pca9685_addr);
    mraa_i2c_write(m_i2c,m_rx_tx_buf,2);



    m_rx_tx_buf[0]=PCA9685_PRESCALE;
    m_rx_tx_buf[1]=prescale;
    mraa_i2c_address(m_i2c, pca9685_addr);
    mraa_i2c_write(m_i2c,m_rx_tx_buf,2);




    m_rx_tx_buf[0]=PCA9685_MODE1;
    m_rx_tx_buf[1]=0x00;
    mraa_i2c_address(m_i2c, pca9685_addr);
    mraa_i2c_write(m_i2c,m_rx_tx_buf,2);

    // mraa_i2c_write_byte_data(m_i2c,0x00,PCA9685_MODE1);

    usleep(5000);


    m_rx_tx_buf[0]=PCA9685_MODE1;
    m_rx_tx_buf[1]=0xa1;
    mraa_i2c_address(m_i2c, pca9685_addr);
    mraa_i2c_write(m_i2c,m_rx_tx_buf,2);
}

int adafruitss::update(void)
{
    return MRAA_SUCCESS;
}

void adafruitss::servo(uint8_t port, uint8_t servo_type, float degrees) {
    // Set Servo values
    // Degrees is from 0 to 180
    // servo_type: 0 = standard 1ms to 2ms
    //             1 = extended 0.6ms to 2.4ms
    //             2 = extended 0.8ms to 2.2ms

    float duration;

    if(degrees>180) degrees=180;        // Ensure within bounds
    if (degrees<0) degrees=0;
    switch (servo_type) {
      default:
      case 0:              // Standard Servo 1ms to 2ms
         duration = _duration_1ms + ((_duration_1ms*degrees)/180);
         break;

      case 1:              // Extended Servo 0.6ms to 2.4ms, i.e. 1.8ms from 0 to 180
         //duration = (_duration_1ms*0.6) + ((_duration_1ms*1.8*degrees)/180); simplified to..
         duration = (_duration_1ms*0.6) + ((_duration_1ms*degrees)/100);
         break;

      case 2:              // Extended Servo 0.8ms to 2.2ms, i.e. 1.4ms from 0 to 180
         //duration = (_duration_1ms*0.8) + ((_duration_1ms*1.4*degrees)/180); simplified to..
         duration = (_duration_1ms*0.8) + ((_duration_1ms*degrees)/128);
         break;
      case 3:              // Extended Servo 0.9ms to 2.1ms,  - GWS Mini STD BB servo
         //duration = (_duration_1ms*0.9) + ((_duration_1ms*1.4*degrees)/180); simplified to..
         duration = (_duration_1ms*0.9) + ((_duration_1ms*degrees)/120);
         break;
   }

    uint16_t d= roundf(duration);
    mraa_i2c_address(m_i2c, pca9685_addr);
    m_rx_tx_buf[0]=LED0_REG+4*port;
    m_rx_tx_buf[1]=0;
    m_rx_tx_buf[2]=0;
    m_rx_tx_buf[3]=d;
    m_rx_tx_buf[4]=d>>8;

    mraa_i2c_write(m_i2c,m_rx_tx_buf,5);
 }

