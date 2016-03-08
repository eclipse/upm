/**************************************************************************/
/*!
@file     MQ135.h
@author   G.Krocker (Mad Frog Labs)
@license  GNU GPLv3

First version of an Arduino Library for the MQ135 gas sensor
TODO: Review the correction factor calculation. This currently relies on
the datasheet but the information there seems to be wrong.
    
 Migrated to mraa / upm libraries to be used with  IoT INTEL Edison or other
 modi by G. Vidal ENS de Lyon 2016-03 http://ife.ens-lyon.fr
  
  
@section  HISTORY

v1.0 - First release
*/
/**************************************************************************/
#pragma once

//#include <iostream>
//#include <string>
#include <mraa/aio.h>
#include <mraa/i2c.hpp>

namespace upm {
  /**
   * @library gas
   * @sensor mq135
   * @comname Click MQ1355 Gas Sensor
   * @type gaseous
   * @man seeed
   * @con analog
   * @kit eak
   *
   * @brief API for the MQ5 Gas Sensor
   *
   * The Click MQ135 Gas Sensor module is useful for gas leakage detection
   * (in home and industry). It can detect LPG, natural gas, town gas, and so
   * on. It is highly sensitive and has a detection range of
   * 300-10,000 ppm.
   *
   * @image html mq135.jpeg
   * @snippet mq135.cxx Interesting
   */
/// The load resistance on the board
#define RLOAD 10.0
/// Calibration resistance at atmospheric CO2 level
#define RZERO 76.63
/// Parameters for calculating ppm of CO2 from sensor resistance
#define PARA 116.6020682
#define PARB 2.769034857

/// Parameters to model temperature and humidity dependence
#define CORA 0.00035
#define CORB 0.02718
#define CORC 1.39538
#define CORD 0.0018

/// Atmospheric CO2 level for calibration purposes
#define ATMOCO2 397.13

class MQ135 {
 private:
  std::string m_name;
  mraa_aio_context m_aio;
	    
 public:

  MQ135(int gasPin);
  ~MQ135 ();
  float getCorrectionFactor(float t, float h);
  float getResistance();
  float getCorrectedResistance(float t, float h);
  float getPPM();
  float getCorrectedPPM(float t, float h);
  float getRZero();
  float getCorrectedRZero(float t, float h);
  std::string name()
  {
      return m_name;
   }
};

}
