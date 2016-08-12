/*
 * Author: Marc Graham <marc@m2ag.net>
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

#include "mcp9808.hpp"
#include <cmath>
#include <syslog.h>

using namespace upm;

MCP9808::MCP9808 (int bus, uint8_t address){
	m_name = "mcp9808";
	m_celsius = true;
	m_tcrit = false;
	m_tupper = false;
	m_tlower = false;
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


float
MCP9808::getTemp(){
	uint16_t result;
	//Read the register
	result = i2c->readWordReg(MCP9808_REG_AMBIENT_TEMP);
	//Swap the bytes
	result = swapWord(result);
	//Get the flag bits.
	m_tcrit = (result & 0x8000) ;
	m_tupper = (result & 0x4000) ;
	m_tlower = (result & 0x2000) ;
	return getTempValue(result);
}

void
MCP9808::shutDown(bool sleep){
	if(sleep) this->updateConfigRegister(MCP9808_CONFIG_SHUTDOWN);
	 else this->updateConfigRegister(~(MCP9808_CONFIG_SHUTDOWN), false);
}


void
MCP9808::setMonitorReg(MCP9808_REG reg, float value){
	uint16_t t;
	if(m_celsius) t = value * 16.0;
	 else t = ((value - 32) * 5.0/9.0) * 16.0 ;

	t = swapWord(t);

	if(i2c->writeWordReg(reg, t) != mraa::SUCCESS){
		throw std::invalid_argument(std::string(__FUNCTION__) + ": I2c.write() failed");
		return;
	}
}

float
MCP9808::getMonitorReg(MCP9808_REG reg){
	uint16_t value = i2c->readWordReg(reg);
	value =  swapWord(value);
	return getTempValue(value);
}


void
MCP9808::clearInterrupt(){
	this->updateConfigRegister(MCP9808_CONFIG_INTCLR);
}

void
MCP9808::setAlertMode(uint16_t command){
	this->updateConfigRegister(command);
}

void
MCP9808::clearAlertMode(){
	//Preserve hysteresis and shutdown settings but
	//set all alert settings to power on default.
	this->updateConfigRegister(0x2007, false);
}


void
MCP9808::setHysteresis(MCP9808_CONFIG value){
	//Clear hysteresis first.
	this->updateConfigRegister(0xFFF9, false);
	this->updateConfigRegister(value);
}

float
MCP9808::getHysteresis(){
	uint16_t value = i2c->readWordReg(MCP9808_REG_CONFIG);
	value = (value >> 1 ) & 0xF ;
	float res =  (value == 0) ? 0.0 :  ( 1.5 * pow( 2.0 , value - 1 )) ;
	return (m_celsius) ? res : res * 9/5;
}


void
MCP9808::setResolution(MCP9808_RESOLUTION value){
	if(i2c->writeReg(MCP9808_REG_RESOLUTION, value) != mraa::SUCCESS){
		  throw std::invalid_argument(std::string(__FUNCTION__) + ": I2c.write() failed");
		  return;
	}
}

float
MCP9808::getResolution(){
	uint8_t value = i2c->readReg(MCP9808_REG_RESOLUTION);
	return   0.5 * (1.0 /  std::pow( 2.0,  value));
}


uint16_t
MCP9808::getManufacturer(){
	return swapWord(i2c->readWordReg(MCP9808_REG_MANUF_ID));
}

uint16_t
MCP9808::getDevicedId(){
	return swapWord(i2c->readWordReg(MCP9808_REG_DEVICE_ID));
}

MCP9808::~MCP9808 (){
	delete i2c;
}


//Private functions

float
MCP9808::getTempValue(uint16_t result){
	bool neg = (result & 0x1000);
	float res =  (result & 0xFFF) / 16.0;
	if(neg) res = 0 - ( 256 - res );
	if(!m_celsius) res = res * 9.0/5.0 + 32;
	return res;
}

void
MCP9808::updateConfigRegister(uint16_t update, bool on){
	uint16_t reg = i2c->readWordReg(MCP9808_REG_CONFIG);
	if(on) reg |= update;
	    else reg &= update;

	if(i2c->writeWordReg(MCP9808_REG_CONFIG, reg) != mraa::SUCCESS){
		  throw std::invalid_argument(std::string(__FUNCTION__) + ": I2c.write() failed");
		  return;
	}
}


uint16_t
MCP9808::swapWord(uint16_t value){
	uint16_t res = value;
	return ((res & 0xFF) << 8) | ((res >> 8 ) & 0xFF);
}
