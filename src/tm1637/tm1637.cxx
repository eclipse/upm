/*
 * Author: Mihai Tudor Panu <mihai.tudor.panu@intel.com>
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
#include "tm1637.h"
#include <stdarg.h>

const uint8_t m_brkt[2]  = {0x39, 0x0f};
const uint8_t m_nums[10] = {0x3f, 0x06, 0x5b, 0x4f, 0x66,
                            0x6d, 0x7d, 0x07, 0x7f, 0x6f};
const uint8_t m_char[26] = {0x77, 0x7c, 0x39, 0x5e, 0x79,
                            0x71, 0x6f, 0x76, 0x30, 0x1e,
                            0x00, 0x38, 0x00, 0x00, 0x5c,
                            0x73, 0x67, 0x50, 0x5b, 0x78,
                            0x3e, 0x1c, 0x00, 0x00, 0x6e,
                            0x5b};

using namespace std;
using namespace upm;

upm::TM1637::TM1637(int clk_pin, int dio_pin, int bright, M_FAST_GPIO mmio) {

    if((m_clk = mraa_gpio_init(clk_pin)) == NULL){
       throw std::invalid_argument(std::string(__FUNCTION__) +
                                   ": mraa_gpio_init(clk) failed, invalid pin?");
       return;
    }

    if((m_dio = mraa_gpio_init(dio_pin)) == NULL){
       throw std::invalid_argument(std::string(__FUNCTION__) +
                                   ": mraa_gpio_init(dio) failed, invalid pin?");
       return;
    }

    mraa_gpio_dir(m_clk, MRAA_GPIO_OUT);
    mraa_gpio_dir(m_dio, MRAA_GPIO_OUT);

    // Let the resistors pull the lines high
    mraa_gpio_mode(m_clk, MRAA_GPIO_PULLUP);
    mraa_gpio_mode(m_dio, MRAA_GPIO_PULLUP);

    if(mmio){
       if(mraa_gpio_use_mmaped(m_clk, 1) != MRAA_SUCCESS ||
          mraa_gpio_use_mmaped(m_dio, 1) != MRAA_SUCCESS){
           throw std::runtime_error(std::string(__FUNCTION__) +
                                    ": mraa_gpio_use_mmaped() failed");
           return;
       }
    }

    mraa_gpio_write(m_clk, 0);
    mraa_gpio_write(m_dio, 0);

    for (int i = 0; i < M_DISPLAY_DIGITS; i++) {
        m_digits[i] = 0x00;
    }
    setBrightness(bright);
}
upm::TM1637::~TM1637() {
    for (int i = 0; i < M_DISPLAY_DIGITS; i++) {
        m_digits[i] = 0x00;
    }
    update();

    mraa_gpio_close(m_clk);
    mraa_gpio_close(m_dio);
}
mraa_result_t upm::TM1637::write(uint8_t *digits) {
    for (int i = 0; i < M_DISPLAY_DIGITS; i++) {
        m_digits[i] = digits[i];
    }
    update();
    return MRAA_SUCCESS;
}
mraa_result_t upm::TM1637::write(int d, ...) {
    va_list args;
    va_start(args, d);
    m_digits[0] = (uint8_t)d;

    for (int i = 1; i < M_DISPLAY_DIGITS; i++) {
        m_digits[i] = (uint8_t)va_arg(args, int);
        d++;
    }
    va_end(args);
    update();
    return MRAA_SUCCESS;
}
mraa_result_t upm::TM1637::writeAt(int index, char symbol) {
    if(index < 0 || index >= M_DISPLAY_DIGITS){
        cerr << "TM1637: invalid index in " << __FUNCTION__ << endl;
        return MRAA_ERROR_INVALID_PARAMETER;
    }
    m_digits[index] = encode(symbol);
    update();
    return MRAA_SUCCESS;
}
mraa_result_t upm::TM1637::write(std::string digits) {
    int len = digits.length();
    if( len > M_DISPLAY_DIGITS){
        len = M_DISPLAY_DIGITS;
    }
    for (int i = 0; i < len; i++) {
        m_digits[i] = encode(digits[i]);
    }
    update();
    return MRAA_SUCCESS;
}
void upm::TM1637::setColon(bool value) {
    if(value){
       m_digits[1] |= 0x80;
    }
    else{
       m_digits[1] &= 0x7f;
    }
    update();
}
void upm::TM1637::setBrightness(int value) {
    m_brightness = value & 0x07;
    update();
}
void upm::TM1637::i2c_start() {
    mraa_gpio_write(m_clk, 1);
    mraa_gpio_write(m_dio, 1);
    mraa_gpio_write(m_dio, 0);
}
void upm::TM1637::i2c_stop() {
    mraa_gpio_write(m_clk, 0);
    mraa_gpio_write(m_dio, 0);
    mraa_gpio_write(m_clk, 1);
    mraa_gpio_write(m_dio, 1);
}
void upm::TM1637::i2c_writeByte(uint8_t value) {
    for(uint8_t i = 0; i < 8; i++)
    {
        mraa_gpio_write(m_clk, 0);
        if(value & 0x01)
            mraa_gpio_write(m_dio, 1);
        else
            mraa_gpio_write(m_dio, 0);
        value >>= 1;
        mraa_gpio_write(m_clk, 1);
    }

    // Ack clock without skew, TM1637 is fast enough
    mraa_gpio_write(m_clk, 0);
    mraa_gpio_write(m_clk, 1);
    mraa_gpio_write(m_clk, 0);
}
void upm::TM1637::update() {
    i2c_start();
    i2c_writeByte(TM1637_ADDR);
    i2c_stop();

    i2c_start();
    i2c_writeByte(TM1637_REG);
    for (int i = 0; i < M_DISPLAY_DIGITS; i++) {
        i2c_writeByte(m_digits[i]);
    }
    i2c_stop();

    i2c_start();
    i2c_writeByte(TM1637_CMD | m_brightness);
    i2c_stop();
}
uint8_t upm::TM1637::encode(char c) {
    if(c >= '0' && c <= '9')
        return m_nums[(int)c - 48];
    if(c >= 'a' && c <= 'z')
        return m_char[(int)c - 97];
    if(c >= 'A' && c <= 'Z')
        return m_char[(int)c - 65];
    if(c == '[')
        return m_brkt[0];
    if(c == ']')
        return m_brkt[1];
    if(c == '(' || c == ')')
        return m_brkt[(int)c - 40];
    if(c == '-')
        return 0x40;
    if(c == '_')
        return 0x08;
    if(c == '}')
        return 0x70;
    if(c == '{')
        return 0x46;
    return 0x00;
}
