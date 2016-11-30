/*
 * Author: Jon Trulson <jtrulson@ics.com>
 * Copyright (c) 2016 Intel Corporation.
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

#include <unistd.h>
#include <string>
#include <iostream>
#include <signal.h>
#include "bmi160.hpp"

using namespace std;

int shouldRun = true;

void sig_handler(int signo)
{
  if (signo == SIGINT)
    shouldRun = false;
}


int main(int argc, char **argv)
{
  signal(SIGINT, sig_handler);
  upm::BMI160 *sensor = NULL;
//! [Interesting]

  // Instantiate a BMI160 instance using default i2c bus and address.
  // If that fails, try default spi bus.
  try
  {
    sensor = new upm::BMI160();
  }
  catch (std::exception& e)
  {
    std::cout << "Failed to connect via I2C, trying default SPI bus" << std::endl;
  }

  try
  {
    if (sensor == NULL)
      sensor = new upm::BMI160(0, -1);
  }
  catch (std::exception& e)
  {
    std::cout << "Failed to connect via SPI, exiting." << std::endl;
    return -1;
  }

  while (shouldRun)
    {
      // update our values from the sensor
      sensor->update();

      float dataX, dataY, dataZ;

      sensor->getAccelerometer(&dataX, &dataY, &dataZ);
      cout << "Accelerometer: ";
      cout << "AX: " << dataX << " AY: " << dataY << " AZ: "
           << dataZ << endl;

      sensor->getGyroscope(&dataX, &dataY, &dataZ);
      cout << "Gryoscope:     ";
      cout << "GX: " << dataX << " GY: " << dataY << " GZ: "
           << dataZ << endl;

      sensor->getMagnetometer(&dataX, &dataY, &dataZ);
      cout << "Magnetometer:  ";
      cout << "MX: " << dataX << " MY: " << dataY << " MZ: "
           << dataZ << endl;

      cout << endl;

      usleep(500000);
    }
//! [Interesting]

  cout << "Exiting..." << endl;

  delete sensor;

  return 0;
}
