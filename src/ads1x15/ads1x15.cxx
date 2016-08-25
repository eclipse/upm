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


#include "ads1x15.hpp"

#include <unistd.h>
#include <syslog.h>

using namespace upm;

ADS1X15::ADS1X15(int bus, uint8_t address){

     if(!(i2c = new mraa::I2c(bus))){
           throw std::invalid_argument(std::string(__FUNCTION__) +": I2c.init() failed");
           return;
     }

     if((i2c->address(address) != mraa::SUCCESS)){
           throw std::runtime_error(std::string(__FUNCTION__) + ": I2c.address() failed");
           return;
     }

     if(i2c->frequency( mraa::I2C_FAST) != mraa::SUCCESS){
           syslog(LOG_WARNING, "%s: I2c.frequency(I2C_FAST) failed, using default speed", std::string(__FUNCTION__).c_str());
     }
     //Will be reset by sub class.
     m_bitShift = 0;
     m_conversionDelay = .001;
     m_config_reg = 0x0000;
}

ADS1X15::~ADS1X15(){}

float
ADS1X15::getSample(ADSMUXMODE mode){
     updateConfigRegister((m_config_reg & ~ADS1X15_MUX_MASK) | mode, true);
     usleep(m_conversionDelay);
     return getLastSample();
}

float
ADS1X15::getLastSample(int reg){
     uint16_t value = i2c->readWordReg(reg);
     bool neg = false;
     value = swapWord(value);
     if(value & 0x8000){
          neg = true;
          value = ~value;
     }
     if(m_name == "ADS1015") value = value >> m_bitShift;
     if(neg) return 0.0 - value * getMultiplier();
      else return value * getMultiplier();
}

void
ADS1X15::setGain(ADSGAIN gain){
     updateConfigRegister((m_config_reg & ~ADS1X15_PGA_MASK) | gain);
}

void
ADS1X15::setSPS(ADSSAMPLERATE rate){
     updateConfigRegister((m_config_reg & ~ADS1X15_DR_MASK) | rate);
}

void
ADS1X15::setCompMode(bool mode){
     if(mode) updateConfigRegister((m_config_reg & ~ADS1X15_CMODE_MASK));
      else updateConfigRegister((m_config_reg & ~ADS1X15_CMODE_MASK) | ADS1X15_CMODE_WINDOW);
}

void
ADS1X15::setCompPol(bool mode){
     if(!mode) updateConfigRegister((m_config_reg & ~ADS1X15_CPOL_MASK));
      else updateConfigRegister((m_config_reg & ~ADS1X15_CPOL_MASK) | ADS1X15_CPOL_ACTVHI);
}

void
ADS1X15::setCompLatch(bool mode){
     if(mode) updateConfigRegister((m_config_reg & ~ADS1X15_CLAT_MASK));
      else updateConfigRegister((m_config_reg & ~ADS1X15_CLAT_MASK) | ADS1X15_CLAT_LATCH);
}

void
ADS1X15::setCompQue(ADSCOMP mode){
     updateConfigRegister((m_config_reg & ~ADS1X15_CQUE_MASK) | mode);
}

void
ADS1X15::setContinuous(bool mode){
     if(mode) updateConfigRegister((m_config_reg & ~ADS1X15_MODE_MASK));
      else updateConfigRegister((m_config_reg & ~ADS1X15_MODE_MASK) | ADS1X15_MODE_SINGLE);
}

float
ADS1X15::getThresh(ADSTHRESH reg){
     if( THRESH_HIGH && THRESH_LOW) return getLastSample(reg);
     else return 0.0;
}

void
ADS1X15::setThresh(ADSTHRESH reg, float value){
     uint16_t set_value;
     switch((int)reg){
     case 4: //set conversion_rdy operation
          if(i2c->writeWordReg(ADS1X15_REG_POINTER_LOWTHRESH, 0x0000) != mraa::SUCCESS){
            throw std::runtime_error(std::string(__FUNCTION__) + ": I2c.write() failed");
            return;
          }
          if(i2c->writeWordReg(ADS1X15_REG_POINTER_HITHRESH, 0x0080) != mraa::SUCCESS){
            throw std::runtime_error(std::string(__FUNCTION__) + ": I2c.write() failed");
            return;
          }
          break;
     case 2:
     case 3:
          set_value = value / getMultiplier();
         set_value = set_value << m_bitShift;
          if(i2c->writeWordReg(reg, swapWord(set_value)) != mraa::SUCCESS){
            throw std::runtime_error(std::string(__FUNCTION__) + ": I2c.write() failed");
            return;
          }
          break;
     case 5: //set default
     default:
          if(i2c->writeWordReg(ADS1X15_REG_POINTER_LOWTHRESH, 0x0080) != mraa::SUCCESS){
            throw std::runtime_error(std::string(__FUNCTION__) + ": I2c.write() failed");
            return;
          }
          if(i2c->writeWordReg(ADS1X15_REG_POINTER_HITHRESH, 0xF07F) != mraa::SUCCESS){
            throw std::runtime_error(std::string(__FUNCTION__) + ": I2c.write() failed");
            return;
          }
          break;
     }
}


//Private functions
void
ADS1X15::getCurrentConfig(){
     m_config_reg = i2c->readWordReg(ADS1X15_REG_POINTER_CONFIG);
     m_config_reg = swapWord(m_config_reg);
     setDelay();
}

void
ADS1X15::updateConfigRegister(uint16_t update, bool read){
     uint16_t temp = update;
     //Mask out read bit if we are just updating the configuration.
     if(!read) temp = update & 0x7FFF;
     if(i2c->writeWordReg(ADS1X15_REG_POINTER_CONFIG, swapWord(temp)) != mraa::SUCCESS){
            throw std::runtime_error(std::string(__FUNCTION__) + ": I2c.write() failed");
            return;
     }
     //If we update the configuration re-set the in memory copy.
     if(!read) getCurrentConfig();
}

uint16_t
ADS1X15::swapWord(uint16_t value){
     uint16_t res = value;
     return ((res & 0xFF) << 8) | ((res >> 8 ) & 0xFF);
}


