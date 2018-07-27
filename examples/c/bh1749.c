/*
* The MIT License (MIT)
*
* Author: Assam Boudjelthia
* Copyright (c) 2018 Rohm Semiconductor.
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

#include <stdio.h>
#include <signal.h>
#include "bh1749.h"
#include "upm_utilities.h"

bool isStopped;
#define SENSOR_ADDR 0x39
#define I2C_BUS 0

void signal_int_handler(int signo)
{
    if (signo == SIGINT)
        isStopped = true;
}

void print_data(void *dev)
{
	uint16_t result[5];
	bh1749_get_measurements((bh1749_context)dev, result);
	printf("R: %d, G: %d, B: %d, IR: %d, G2: %d\n", result[0],
			result[1], result[2], result[3], result[4]);
	bh1749_reset_interrupt(dev);
}

int main(int argc, char **argv)
{
	signal(SIGINT, signal_int_handler);

	bh1749_context dev = bh1749_init(I2C_BUS, SENSOR_ADDR);
	if (!dev) {
        printf("bh1749_init() failed.\n");
        return -1;
    }

	bh1749_soft_reset(dev);
	bh1749_sensor_init(dev, INT_JUDGE_1, MEAS_240MS, RGB_GAIN_1X, IR_GAIN_1X, RED);
	bh1749_set_threshold_high(dev, 511);
	bh1749_enable_interrupt(dev);
	printf("Installing ISR\n");
	bh1749_install_isr(dev, MRAA_GPIO_EDGE_FALLING, 33, &print_data, (void *)dev);
	bh1749_enable(dev);

	while(!isStopped) {
		upm_delay_ms(1000);
	}

	bh1749_close(dev);
	return 0;
}