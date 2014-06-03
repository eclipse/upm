/*
 * Author: Yevgeniy Kiveisha <yevgeniy.kiveisha@intel.com>
 * Copyright (c) 2014 Intel Corporation.
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

#include <iostream>
#include <unistd.h>
#include <stdlib.h>

#include "tm1637.h"

using namespace upm;

const uint8_t digitToSegment[] = {
 	// XGFEDCBA
	0b00111111,    // 0
	0b00000110,    // 1
	0b01011011,    // 2
	0b01001111,    // 3
	0b01100110,    // 4
	0b01101101,    // 5
	0b01111101,    // 6
	0b00000111,    // 7
	0b01111111,    // 8
	0b01101111,    // 9
	0b01110111,    // A
	0b01111100,    // B
	0b00111001,    // C
	0b01000111,    // D
	0b01111001,    // E
	0b01110001     // F
};

TM1637::TM1637 (uint8_t di, uint8_t dcki) {
	maa_result_t error = MAA_SUCCESS;
	maa_init();

	// init clock context
	m_clkPinCtx = maa_gpio_init(dcki);
	if (m_clkPinCtx == NULL) {
        fprintf(stderr, "Are you sure that pin%d you requested is valid on your platform?", dcki);
		exit(1);
	}
	// init data context
	m_dataPinCtx = maa_gpio_init(di);
	if (m_dataPinCtx == NULL) {
        fprintf(stderr, "Are you sure that pin%d you requested is valid on your platform?", di);
		exit(1);
	}
	
	// set direction (out)
	error = maa_gpio_dir(m_clkPinCtx, MAA_GPIO_IN);
    if (error != MAA_SUCCESS) {
        maa_result_print(error);
    }

	// set direction (out)
	error = maa_gpio_dir(m_dataPinCtx, MAA_GPIO_IN);
    if (error != MAA_SUCCESS) {
        maa_result_print(error);
    }

	error = maa_gpio_write (m_dataPinCtx, LOW);
	error = maa_gpio_write (m_clkPinCtx, LOW);
}

TM1637::~TM1637() {
	maa_result_t error = MAA_SUCCESS;
	error = maa_gpio_close (m_dataPinCtx);
    if (error != MAA_SUCCESS) {
        maa_result_print(error);
    }
	error = maa_gpio_close (m_clkPinCtx);
    if (error != MAA_SUCCESS) {
        maa_result_print(error);
    }
}

maa_result_t
TM1637::setBrightness (uint8_t level) {
	m_brightness = level;
}

maa_result_t
TM1637::setSegments (const uint8_t segments[], uint8_t length, uint8_t pos) {
	start();
	writeByte(TM1637_I2C_COMM1);
	stop();

	start();
	writeByte(TM1637_I2C_COMM2 + (pos & 0x03));
	for (uint8_t idx = 0; idx < length; idx++) {
		writeByte(segments[idx]);
	}
	stop();

	start();
	writeByte(TM1637_I2C_COMM3 + (m_brightness & 0x0f));
	stop();
}

maa_result_t
TM1637::write (std::string msg) {
	char leter = '\0';
	uint8_t data[] = { 0x0, 0x0, 0x0, 0x0 };
	for (uint8_t idx = 0; idx < msg.length(); idx++) {
		leter = msg[idx];
		if (idx < 4) {
			data[idx] = digitToSegment[strtol(&leter, NULL, 16)];
		}
	}
	setBrightness (0x0f);
	setSegments(data);
}

maa_result_t
TM1637::pinMode (maa_gpio_context ctx, gpio_dir_t mode) {
	maa_result_t error = MAA_SUCCESS;
	error = maa_gpio_dir(ctx, mode);
    if (error != MAA_SUCCESS) {
        maa_result_print(error);
    }
}

maa_result_t
TM1637::start() {
	pinMode (m_dataPinCtx, MAA_GPIO_OUT);
	usleep(PULSE_LENGTH);
}

maa_result_t
TM1637::stop() {
	pinMode (m_dataPinCtx, MAA_GPIO_OUT);
	usleep(PULSE_LENGTH);
	pinMode (m_clkPinCtx, MAA_GPIO_IN);
	usleep(PULSE_LENGTH);
	pinMode (m_dataPinCtx, MAA_GPIO_IN);
	usleep(PULSE_LENGTH);
}

maa_result_t
TM1637::writeByte(uint8_t value) {
	for (uint8_t idx = 0; idx < 8; idx++) {
		pinMode(m_clkPinCtx, MAA_GPIO_OUT);
		usleep(PULSE_LENGTH);
		if (value & 0x01) {
	  		pinMode(m_dataPinCtx, MAA_GPIO_IN);
		} else {
	  		pinMode(m_dataPinCtx, MAA_GPIO_OUT);
		}
		usleep(PULSE_LENGTH);

		pinMode(m_clkPinCtx, MAA_GPIO_IN);
		usleep(PULSE_LENGTH);
		value = value >> 1;
	}

	pinMode(m_clkPinCtx, MAA_GPIO_OUT);
	pinMode(m_dataPinCtx, MAA_GPIO_IN);
	usleep(PULSE_LENGTH);

	pinMode(m_clkPinCtx, MAA_GPIO_IN);
	usleep(PULSE_LENGTH);

	uint8_t ack = maa_gpio_read (m_dataPinCtx);
	if (ack == 0) {
    	pinMode(m_dataPinCtx, MAA_GPIO_OUT);
	} usleep(PULSE_LENGTH);

	pinMode(m_clkPinCtx, MAA_GPIO_OUT);
	usleep(50);
}
