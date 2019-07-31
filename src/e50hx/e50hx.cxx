/*
 * Author: Jon Trulson <jtrulson@ics.com>
 * Copyright (c) 2016 Intel Corporation.
 *
 * This program and the accompanying materials are made available under the
 * terms of the The MIT License which is available at
 * https://opensource.org/licenses/MIT.
 *
 * SPDX-License-Identifier: MIT
 */

#include <unistd.h>
#include <assert.h>
#include <errno.h>
#include <iostream>
#include <stdexcept>
#include <string>

#include "e50hx.hpp"

using namespace upm;
using namespace std;


E50HX::E50HX(uint32_t targetDeviceObjectID) :
  BACNETUTIL(targetDeviceObjectID)
{
  setDebug(false);

  // we disable this by default for performance reasons
  checkReliability(false);
}

E50HX::~E50HX()
{
}

uint16_t E50HX::getAlarmBits()
{
  return uint16_t(getAnalogInput(AI_Alarm_Bitmap));
}

void E50HX::writeConfig(CFG_VALUES_T config)
{
  setAnalogValue(AV_Config, float(config));
}

void E50HX::writeSystemType(SYSTEM_TYPES_T systype)
{
  setAnalogValue(AV_System_Type, float(systype));
}

void E50HX::writeCTRatioPrimary(float ctRatio)
{
  if (ctRatio < 5 || ctRatio > 32000)
    {
      throw std::out_of_range(std::string(__FUNCTION__)
                              + ": ctRatio must be between 5-32000");
    }

  setAnalogValue(AV_CT_Ratio_Primary, ctRatio);
}

void E50HX::writeCTRatioSecondary(CT_SECONDARY_T ctRatio)
{
  setAnalogValue(AV_CT_Ratio_Secondary, ctRatio);
}

void E50HX::writePTRatio(float ptRatio)
{
  if (ptRatio < 0.01 || ptRatio > 320.0)
    {
      throw std::out_of_range(std::string(__FUNCTION__)
                              + ": ptRatio must be between 0.01-320.0");
    }

  setAnalogValue(AV_PT_Ratio, ptRatio);
}

void E50HX::writeSystemVoltage(float sysVolts)
{
  if (sysVolts < 82.0 || sysVolts > 32000.0)
    {
      throw std::out_of_range(std::string(__FUNCTION__)
                              + ": sysVolts must be between 82.0-32000.0");
    }

  setAnalogValue(AV_System_Voltage, sysVolts);
}

void E50HX::writeDisplayUnits(DISP_UNITS_T dispUnits)
{
  setAnalogValue(AV_Display_Units, dispUnits);
}

void E50HX::writePhaseLossVT(float phaseLoss)
{
  if (phaseLoss < 1.0 || phaseLoss > 99.0)
    {
      throw std::out_of_range(std::string(__FUNCTION__)
                              + ": phaseLoss must be between 1.0-99.0");
    }

  setAnalogValue(AV_Phase_Loss_Voltage_Threshold, phaseLoss);
}

void E50HX::writePhaseLossIT(float phaseLoss)
{
  if (phaseLoss < 1.0 || phaseLoss > 99.0)
    {
      throw std::out_of_range(std::string(__FUNCTION__)
                              + ": phaseLoss must be between 1.0-99.0");
    }

  setAnalogValue(AV_Phase_Loss_Imbalance_Threshold, phaseLoss);
}
