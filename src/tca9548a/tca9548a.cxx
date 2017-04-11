/*
 * Author: Gérard Vidal <gerard.vidal@ens-lyon.fr>
 * Copyright (c) 2017 IFÉ-ENS-Lyon
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

#include "tca9548a.hpp"
#include <syslog.h>

#include <unistd.h>
#include <stdexcept>
#include <string>
#include <string.h>
#include <math.h>
#include "mraa.hpp"
#include "mraa/i2c.hpp"



using namespace upm;

TCA9548A::TCA9548A (int bus, uint8_t address){
	m_name = "tca9548a";
	if(!(i2c = new mraa::I2c(bus))){
	      throw std::invalid_argument(std::string(__FUNCTION__) +": I2c.init() failed");
	      return;
	}

	if((i2c->address(address) != mraa::SUCCESS)){
	      throw std::invalid_argument(std::string(__FUNCTION__) + ": I2c.address() failed");
	      return;
	}

	if(i2c->frequency( mraa::I2C_FAST) != mraa::SUCCESS){
          syslog(LOG_WARNING, "%s: I2c.frequency(I2C_FAST) failed, using default speed", std::string(__FUNCTION__).c_str());
	}
}

TCA9548A::~TCA9548A (){
	delete i2c;
}


void 
TCA9548A::selMultSlaves(uint8_t address, CHANNEL_REG channel1, CHANNEL_REG channel2, CHANNEL_REG channel3, CHANNEL_REG channel4, CHANNEL_REG channel5, CHANNEL_REG channel6, CHANNEL_REG channel7, CHANNEL_REG channel8) {
    uint8_t opened = 0x00;
    opened = opened | channel1 | channel2 | channel3 | channel4 | channel5 | channel6 | channel7 | channel8;
    if(i2c->writeReg(address, opened) != mraa::SUCCESS){
		  throw std::invalid_argument(std::string(__FUNCTION__) + ": I2c.write() failed");
		  return;
    }

}




void 
TCA9548A::selSlave(uint8_t address, CHANNEL_REG channel){
	if(i2c->writeReg(address, channel) != mraa::SUCCESS){
		  throw std::invalid_argument(std::string(__FUNCTION__) + ": I2c.write() failed");
		  return;
	}

}

void 
TCA9548A::closeSlaves(uint8_t address){
	if(i2c->writeReg(address, CHANNEL_REG::RESET) != mraa::SUCCESS){
		  throw std::invalid_argument(std::string(__FUNCTION__) + ": I2c.write() failed");
		  return;
	}

}


