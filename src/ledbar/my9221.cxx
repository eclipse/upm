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

#include "my9221.h"

using namespace upm;

MY9221::MY9221 (uint8_t di, uint8_t dcki) {
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
	error = maa_gpio_dir(m_clkPinCtx, MAA_GPIO_OUT);
    if (error != MAA_SUCCESS) {
        maa_result_print(error);
    }

	// set direction (out)
	error = maa_gpio_dir(m_dataPinCtx, MAA_GPIO_OUT);
    if (error != MAA_SUCCESS) {
        maa_result_print(error);
    }
}

MY9221::~MY9221() {
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
MY9221::setBarLevel (uint8_t level) {
	if (level > 10) {
		return MAA_ERROR_INVALID_PARAMETER;
	}

	send16bitBlock (CMDMODE);
	for(uint8_t block_idx = 0; block_idx < 12; block_idx++) {
        uint32_t state = (block_idx < level) ? BIT_HIGH : BIT_LOW;
        send16bitBlock (state);
    }
    lockData ();
}

maa_result_t
MY9221::lockData () {
	maa_result_t error = MAA_SUCCESS;
	error = maa_gpio_write (m_dataPinCtx, LOW);
	usleep(100);
	
	for(int idx = 0; idx < 4; idx++) {
		error = maa_gpio_write (m_dataPinCtx, HIGH);
		error = maa_gpio_write (m_dataPinCtx, LOW);
    }
}

maa_result_t
MY9221::send16bitBlock (short data) {
	maa_result_t error = MAA_SUCCESS;
	for (uint8_t bit_idx = 0; bit_idx < MAX_BIT_PER_BLOCK; bit_idx++) {
		uint32_t state = (data & 0x8000) ? HIGH : LOW;
		error = maa_gpio_write (m_dataPinCtx, state);
		state = maa_gpio_read (m_clkPinCtx);

		if (state) {
			state = LOW;
		} else {
			state = HIGH;
		}

		error = maa_gpio_write (m_clkPinCtx, state);

		data <<= 1;
	}
}
