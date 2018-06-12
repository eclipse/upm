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

#include <unistd.h>
#include <signal.h>
#include "kxtj3.h"

#define SENSOR_ADDR 0x0f
#define I2C_BUS 0
#define SAMPLE_COUNT 10

bool isStopped = false;

void signal_int_handler(int signo)
{
    if (signo == SIGINT)
        isStopped = true;
}

void print_acceleration_data(kxtj3_context dev)
{
    float wait_time = kxtj3_get_acceleration_sampling_period(dev) * SECOND_IN_MICRO_S;
    uint8_t sample_counter = 0;
    float x, y, z;
    while (sample_counter < SAMPLE_COUNT && !isStopped)
    {
        kxtj3_get_acceleration_data(dev, &x, &y, &z);
        printf("%.02f | %.02f | %.02f\n", x, y, z);
        usleep(wait_time);
        sample_counter++;
    }
}

int main(int argc, char **argv)
{
    signal(SIGINT, signal_int_handler);

    printf("Sensor init\n");
    kxtj3_context dev = kxtj3_init(I2C_BUS, SENSOR_ADDR);
    if (!dev)
    {
        printf("kxtj3_init() failed.\n");
        return -1;
    }

    printf("Setting settings:\nODR: 25 Hz\nResolution: High\nAcceleration range: 16g with 14bits");
    kxtj3_sensor_init(dev, KXTJ3_ODR_25, HIGH_RES, KXTJ3_RANGE_16G_14);
    printf("Showing acceleration data:\n");
    print_acceleration_data(dev);

    printf("Closing sensor\n");
    kxtj3_close(dev);
    return 0;
}
