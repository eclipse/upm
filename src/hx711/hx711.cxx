/*
 * Author: Rafael da Mata Neri <rafael.neri@gmail.com>
 * Copyright (c) 2015 Intel Corporation.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */
#include <iostream>
#include <fstream>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdexcept>
#include "hx711.h"

using namespace upm;
using namespace std;

HX711::HX711(uint8_t data, uint8_t sck, uint8_t gain) {
    mraa_result_t error = MRAA_SUCCESS;

    this->m_dataPinCtx = mraa_gpio_init(data);
    if (this->m_dataPinCtx == NULL) {
        throw std::invalid_argument(std::string(__FUNCTION__) + 
                                    ": Couldn't initialize DATA pin.");
    }

    this->m_sckPinCtx = mraa_gpio_init(sck);
    if (this->m_sckPinCtx == NULL) {
        throw std::invalid_argument(std::string(__FUNCTION__) + 
                                    ": Couldn't initialize CLOCK pin.");
    }

    error = mraa_gpio_dir (this->m_dataPinCtx, MRAA_GPIO_IN);
    if (error != MRAA_SUCCESS) {
        throw std::invalid_argument(std::string(__FUNCTION__) + 
                                    ": Couldn't set direction for DATA pin.");
    }

    error = mraa_gpio_dir (this->m_sckPinCtx, MRAA_GPIO_OUT);
    if (error != MRAA_SUCCESS) {
        throw std::invalid_argument(std::string(__FUNCTION__) + 
                                    ": Couldn't set direction for CLOCK pin.");
    }

    this->setGain(gain);
}

HX711::~HX711() {
    mraa_result_t error = MRAA_SUCCESS;

    error = mraa_gpio_close (this->m_dataPinCtx);
    if (error != MRAA_SUCCESS) {
        mraa_result_print(error);
    }

    error = mraa_gpio_close (this->m_sckPinCtx);
    if (error != MRAA_SUCCESS) {
        mraa_result_print(error);
    }
}

unsigned long HX711::read() {
    unsigned long Count = 0;

    while (mraa_gpio_read(this->m_dataPinCtx));

    for (int i=0; i<GAIN; i++)
    {
        mraa_gpio_write(this->m_sckPinCtx, 1);
        Count = Count << 1;
        mraa_gpio_write(this->m_sckPinCtx, 0);
        if(mraa_gpio_read(this->m_dataPinCtx))
        {
            Count++;
        }
    }

    mraa_gpio_write(this->m_sckPinCtx, 1);
    Count = Count ^ 0x800000;
    mraa_gpio_write(this->m_sckPinCtx, 0);

    return (Count);
}

void HX711::setGain(uint8_t gain){
    switch (gain) {
        case 128:       // channel A, gain factor 128
            GAIN = 24;
            break;
        case 64:        // channel A, gain factor 64
            GAIN = 26;
            break;
        case 32:        // channel B, gain factor 32
            GAIN = 25;
            break;
    }

    mraa_gpio_write(this->m_sckPinCtx, 0);
    read();
}

unsigned long HX711::readAverage(uint8_t times){
    unsigned long sum = 0;
    for (uint8_t i = 0; i < times; i++) {
        sum += read();
    }
    return sum / times;
}

double HX711::getValue(uint8_t times){
    return readAverage(times) - OFFSET;
}

float HX711::getUnits(uint8_t times){
    return getValue(times) / SCALE;
}

void HX711::tare(uint8_t times){
    double sum = readAverage(times);
    setOffset(sum);
}

void HX711::setScale(float scale){
    SCALE = scale;
}

void HX711::setOffset(long offset){
    OFFSET = offset;
}
