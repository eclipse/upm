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
#include "bh1792.h"
#include  <sys/time.h>
#include "upm_utilities.h"

bool isStopped;
#define SENSOR_ADDR 0x5b
#define I2C_BUS 0

void signal_int_handler(int signo)
{
    if (signo == SIGINT)
        isStopped = true;
}

void print_data(void *args)
{
	bh1792_context dev = (bh1792_context)args;
	uint16_t led_on[32], led_off[32];
	struct timespec now;

	clock_gettime(CLOCK_MONOTONIC_RAW, &now);

	if(bh1792_get_fifo_data(dev, led_off, led_on) == UPM_SUCCESS) {
		printf("\nFIFO data:\n");
		for(int i = 0; i < 32; i++)
			printf("%d: off: %d, on: %d\n", i, led_off[i], led_on[i]);
			
		printf("timestamp %ld sec, %ld nsec\n", now.tv_sec, now.tv_nsec);
	}
}

int main(int argc, char **argv)
{
	float meas_time;

	signal(SIGINT, signal_int_handler);

	bh1792_context dev = bh1792_init(I2C_BUS, SENSOR_ADDR);
	if (!dev) {
        printf("bh1792_init() failed.\n");
        return -1;
    }

	printf("Software reset\n");
	bh1792_soft_reset(dev);

	bh1792_enable_sync_mode(dev, 256, 32);
	bh1792_install_isr(dev, MRAA_GPIO_EDGE_FALLING, 33, &print_data, dev);
	bh1792_get_meas_time_ms(dev, &meas_time);
	bh1792_start_measurement(dev);

	printf("Heart beat sensor data\n");

	while(!isStopped) {
		usleep(meas_time * 1000);
	}

	bh1792_close(dev);
	return 0;
}