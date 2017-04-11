/*
 * Author: Gérard Vidal <gerard.vidal@ens-lyon.fr>
 * 2017 IFÉ ENS de Lyon
 * 
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
#include <iostream>
#include <signal.h>

#include "bmp280.hpp"
#include "tca9548a.hpp"

using namespace std;
using namespace upm;

/*
 * This examble is built to multiplex channel 2 and Channel 4 bearing 
 * respectively sensor BME280 and BMP 280
 * 
 */

void sig_handler(int signo)
{
  if (signo == SIGINT)
    shouldRun = false;
}


int main(int argc, char **argv)
{
  signal(SIGINT, sig_handler);

  // Instantiate a TCA9548A instance of i2c multiplexer
  upm::TCA9548A *multiplex = new upm::TCA9548A();
  
  //**************************************
  // Open i2c bus on multiplexer channel 4
   //**************************************
 multiplex->selSlave(0x70,CHANNEL_REG::C4);
  // Instantiate a BMP280 instance using multiplex C4 i2c bus and BMP 280 default address
  upm::BMP280 *sensor = new upm::BMP280();

  // update our values from the sensor
  sensor->update();
  // we show both C and F for temperature
  cout << "Compensation Temperature: " << sensor->getTemperature()
       << " C / " << sensor->getTemperature(true) << " F"
       << endl;
  cout << "Pressure: " << sensor->getPressure() << " Pa" << endl;
  cout << "Computed Altitude: " << sensor->getAltitude() << " m" << endl;

  cout << endl;
  cout << "Exiting..." << endl;

  delete sensor;
  multiplex->closeSlaves(0x70)
  
  //**************************************
  // Open i2c bus on multiplexer channel 2
  //**************************************
  multiplex->selSlave(0x70,CHANNEL_REG::C2);
  // Instantiate a BMP280 instance using multiplex C4 i2c bus and BMP 280 default address
  upm::BME280 *bme = new upm::BME280();
  multiplex->closeSlaves(0x70)
  //**************************************
  // Open i2c bus on multiplexer channel 4
  //**************************************
  multiplex->selSlave(0x70,CHANNEL_REG::C4);
  // Instantiate a BMP280 instance using multiplex C4 i2c bus and BMP 280 default address
  upm::BMP280 *bmp = new upm::BMP280();
  multiplex->closeSlaves(0x70)
  
  //*****************************************************
  // Open 2 i2c bus on multiplexer channel 4 and channel2
  //*****************************************************
  multiplex->selSlaves(0x70,CHANNEL_REG::RESET,CHANNEL_REG::RESET,CHANNEL_REG::C2,CHANNEL_REG::RESET,CHANNEL_REG::C4,CHANNEL_REG::RESET,CHANNEL_REG::RESET,CHANNEL_REG::RESET);

  // update our values from the sensor
  bme->update();
  bmp->update();
  // we show both C and F for temperature
  cout << "BME Compensation Temperature: " << bme->getTemperature()
       << " C / " << bme->getTemperature(true) << " F"
       << endl;
  cout << "BMP Compensation Temperature: " << bmp->getTemperature()
       << " C / " << bmp->getTemperature(true) << " F"
       << endl;
  cout << "BME Pressure: " << bme->getPressure() << " Pa" << endl;
  cout << "BMP Pressure: " << bmp->getPressure() << " Pa" << endl;
  cout << "BME Computed Altitude: " << bme->getAltitude() << " m" << endl;
  cout << "BMP Computed Altitude: " << bmp->getAltitude() << " m" << endl;

  cout << endl;
  cout << "Exiting..." << endl;

  delete bme;
  delete bmp;
  multiplex->closeSlaves(0x70)

  return 0;
}
