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

#include <iostream>
#include <math.h>
#include "tex00.hpp"

using namespace upm;
using namespace std;

// we average over several aio reads.
static const int maxSamples = 10;

// conversion from celsius to fahrenheit

static float c2f(float c)
{
  return (c * (9.0 / 5.0) + 32.0);
}


TEX00::TEX00(int tPin, float balanceResistor, SENSOR_TYPES_T stype, 
             float aref) :
  m_aioTemp(tPin)
{
  m_aResTemp = (1 << m_aioTemp.getBit());

  m_temperature = 0.0;
  m_outOfRange = false;

  m_aref = aref;
  m_balanceResistor = balanceResistor;

  // set default to NTC, however per-sensor init functions should set
  // properly for the relevant sensor
  m_isNTC = true;
  
  switch (stype)
    {
    case STYPE_THERMISTOR_TED:
      initThermistorTED();
      break;

    case STYPE_THERMISTOR_TEB:
      initThermistorTEB();
      break;

    case STYPE_THERMISTOR_TEC:
      initThermistorTEC();
      break;

    case STYPE_THERMISTOR_TEI:
      initThermistorTEI();
      break;

    case STYPE_THERMISTOR_TEE:
      initThermistorTEE();
      break;

    case STYPE_THERMISTOR_TEF:
      initThermistorTEF();
      break;

    case STYPE_THERMISTOR_TEH:
      initThermistorTEH();
      break;

    case STYPE_THERMISTOR_TEJ:
      initThermistorTEJ();
      break;

    case STYPE_THERMISTOR_TES:
      initThermistorTES();
      break;

    case STYPE_THERMISTOR_TER:
      initThermistorTER();
      break;

    case STYPE_THERMISTOR_TEM:
      initThermistorTEM();
      break;

    case STYPE_THERMISTOR_TEU:
      initThermistorTEU();
      break;

    case STYPE_THERMISTOR_TET:
      initThermistorTET();
      break;

    default:
      throw std::logic_error(std::string(__FUNCTION__) +
                             ": internal error: invalid stype");
    }
}

TEX00::~TEX00()
{
}

void TEX00::update()
{
  int val = average(maxSamples);

  // now determine the resistance of the sensor (this is a voltage divider)
  float resistance = m_balanceResistor / 
    ((float(m_aResTemp - 1) / float(val)) - 1.0);
  
  //  cerr << "Val = " << val << ", resistance = " << resistance << endl;

  m_temperature = thermistor(resistance);
}

float TEX00::getTemperature(bool fahrenheit)
{
  if (fahrenheit)
    return c2f(m_temperature);
  else
    return m_temperature;
}

int TEX00::average(int samples)
{
  if (samples <= 0)
    samples = 1;
  
  int avg = 0;
  for (int i=0; i<samples; i++)
    avg += m_aioTemp.read();
  
  return (avg / samples);
}

float TEX00::thermistor(float ohms)
{
  // sanity check
  if (m_tempVector.empty())
    {
      throw std::logic_error(std::string(__FUNCTION__) +
                             ": internal error: temperature table is empty");
    }

  int found = -1;
  int next = -1;
  
  if (m_isNTC)
    {
      // reverse search
      for (int i=m_tempVector.size() - 1; i>=0; i--)
        if (ohms < m_tempVector[i].ohms)
          {
            found = i;
            next = found + 1;
            break;
          }
    }
  else
    {
      // PTC
      for (int i=0; i<m_tempVector.size(); i++)
        if (ohms < m_tempVector[i].ohms)
          {
            found = i;
            next = found - 1;
            break;
          }
    }

  if (found < 0 || next < 0 || 
      found >= m_tempVector.size() || next >= m_tempVector.size())
    {
      m_outOfRange = true;
      // return last measured temperature
      return m_temperature;
    }
  else
    m_outOfRange = false;
  
  // calculate the percentages of ohms and temp
  float diffO = m_tempVector[found].ohms - m_tempVector[next].ohms;
  ohms -= m_tempVector[next].ohms;
  
  diffO = ohms / diffO;
  
  float diffT;
  if (m_isNTC)
    diffT = m_tempVector[next].temp - m_tempVector[found].temp;
  else
    diffT = m_tempVector[found].temp - m_tempVector[next].temp;

  // compute offset
  float tempOffset = fabs(diffT * diffO);

  // apply according to NTC or PTC
  if (m_isNTC)
    return m_tempVector[next].temp - tempOffset;
  else
    return m_tempVector[next].temp + tempOffset;
}

float TEX00::getTemperatureRangeMin()
{
  if (m_tempVector.empty())
    {
      throw std::logic_error(std::string(__FUNCTION__) +
                             ": internal error: temperature table is empty");
    }

  return m_tempVector[0].temp;
}

float TEX00::getTemperatureRangeMax()
{
  if (m_tempVector.empty())
    {
      throw std::logic_error(std::string(__FUNCTION__) +
                             ": internal error: temperature table is empty");
    }

  return m_tempVector[m_tempVector.size() - 1].temp;
}


void TEX00::initThermistorTED()
{
  // this is an NTC type thermistor
  m_isNTC = true;

  // This data comes from the Veris TW_TE_d0116.pdf datasheet for the
  // "D" (10K type 2) column

  // clear and initialize the vector, ordered from lowest to highest
  // temperature
  m_tempVector.clear();

  //                               ohms      temperature (C)
  m_tempVector.push_back(tempEntry(692700,   -50));
  m_tempVector.push_back(tempEntry(344700,   -40));
  m_tempVector.push_back(tempEntry(180100,   -30));
  m_tempVector.push_back(tempEntry(98320,    -20));
  m_tempVector.push_back(tempEntry(55790,    -10));
  m_tempVector.push_back(tempEntry(32770,    0));
  m_tempVector.push_back(tempEntry(19930,    10));
  m_tempVector.push_back(tempEntry(12500,    20));
  m_tempVector.push_back(tempEntry(10000,    25));
  m_tempVector.push_back(tempEntry(8055,     30));
  m_tempVector.push_back(tempEntry(5323,     40));
  m_tempVector.push_back(tempEntry(3599,     50));
  m_tempVector.push_back(tempEntry(2486,     60));
  m_tempVector.push_back(tempEntry(1753,     70));
  m_tempVector.push_back(tempEntry(1258,     80));
  m_tempVector.push_back(tempEntry(919,      90));
  m_tempVector.push_back(tempEntry(682,      100));
  m_tempVector.push_back(tempEntry(513,      110));
  m_tempVector.push_back(tempEntry(392,      120));
  m_tempVector.push_back(tempEntry(303,      130));
}

void TEX00::initThermistorTEB()
{
  // this is a PTC type thermistor
  m_isNTC = false;

  // This data comes from the Veris TW_TE_d0116.pdf datasheet for the
  // "B" (100 Ohm) column

  // clear and initialize the vector, ordered from lowest to highest
  // temperature
  m_tempVector.clear();

  //                               ohms        temperature (C)
  m_tempVector.push_back(tempEntry(80.306,     -50));
  m_tempVector.push_back(tempEntry(84.271,     -40));
  m_tempVector.push_back(tempEntry(88.222,     -30));
  m_tempVector.push_back(tempEntry(92.160,     -20));
  m_tempVector.push_back(tempEntry(96.086,     -10));
  m_tempVector.push_back(tempEntry(100,        0));
  m_tempVector.push_back(tempEntry(103.903,    10));
  m_tempVector.push_back(tempEntry(107.794,    20));
  m_tempVector.push_back(tempEntry(109.735,    25));
  m_tempVector.push_back(tempEntry(111.673,    30));
  m_tempVector.push_back(tempEntry(115.541,    40));
  m_tempVector.push_back(tempEntry(119.397,    50));
  m_tempVector.push_back(tempEntry(123.242,    60));
  m_tempVector.push_back(tempEntry(127.075,    70));
  m_tempVector.push_back(tempEntry(130.897,    80));
  m_tempVector.push_back(tempEntry(134.707,    90));
  m_tempVector.push_back(tempEntry(138.506,    100));
  m_tempVector.push_back(tempEntry(142.293,    110));
  m_tempVector.push_back(tempEntry(146.068,    120));
  m_tempVector.push_back(tempEntry(149.332,    130));
}

void TEX00::initThermistorTEC()
{
  // this is a PTC type thermistor
  m_isNTC = false;

  // This data comes from the Veris TW_TE_d0116.pdf datasheet for the
  // "C" (1000 Ohm) column

  // clear and initialize the vector, ordered from lowest to highest
  // temperature
  m_tempVector.clear();

  //                               ohms        temperature (C)
  m_tempVector.push_back(tempEntry(803.06,     -50));
  m_tempVector.push_back(tempEntry(842.71,     -40));
  m_tempVector.push_back(tempEntry(882.22,     -30));
  m_tempVector.push_back(tempEntry(921.60,     -20));
  m_tempVector.push_back(tempEntry(960.86,     -10));
  m_tempVector.push_back(tempEntry(1000,       0));
  m_tempVector.push_back(tempEntry(1039.03,    10));
  m_tempVector.push_back(tempEntry(1077.94,    20));
  m_tempVector.push_back(tempEntry(1097.35,    25));
  m_tempVector.push_back(tempEntry(1116.73,    30));
  m_tempVector.push_back(tempEntry(1155.41,    40));
  m_tempVector.push_back(tempEntry(1193.97,    50));
  m_tempVector.push_back(tempEntry(1232.42,    60));
  m_tempVector.push_back(tempEntry(1270.75,    70));
  m_tempVector.push_back(tempEntry(1308.97,    80));
  m_tempVector.push_back(tempEntry(1347.07,    90));
  m_tempVector.push_back(tempEntry(1385.06,    100));
  m_tempVector.push_back(tempEntry(1422.93,    110));
  m_tempVector.push_back(tempEntry(1460.68,    120));
  m_tempVector.push_back(tempEntry(1493.32,    130));
}

void TEX00::initThermistorTEI()
{
  // this is a PTC type thermistor
  m_isNTC = false;

  // This data comes from the Veris TW_TE_d0116.pdf datasheet for the
  // "I" (1000 Ohm) column

  // clear and initialize the vector, ordered from lowest to highest
  // temperature
  m_tempVector.clear();

  //                               ohms        temperature (C)
  m_tempVector.push_back(tempEntry(740.46,     -50));
  m_tempVector.push_back(tempEntry(773.99,     -40));
  m_tempVector.push_back(tempEntry(806.02,     -30));
  m_tempVector.push_back(tempEntry(841,        -20));
  m_tempVector.push_back(tempEntry(877.76,     -10));
  m_tempVector.push_back(tempEntry(913.66,     0));
  m_tempVector.push_back(tempEntry(952.25,     10));
  m_tempVector.push_back(tempEntry(991.82,     20));
  m_tempVector.push_back(tempEntry(1013.50,    25));
  m_tempVector.push_back(tempEntry(1035.18,    30));
  m_tempVector.push_back(tempEntry(1077.68,    40));
  m_tempVector.push_back(tempEntry(1120.52,    50));
  m_tempVector.push_back(tempEntry(1166.13,    60));
  m_tempVector.push_back(tempEntry(1210.75,    70));
  m_tempVector.push_back(tempEntry(1254.55,    80));
  m_tempVector.push_back(tempEntry(1301.17,    90));
  m_tempVector.push_back(tempEntry(1348.38,    100));
  m_tempVector.push_back(tempEntry(1397.13,    110));
  m_tempVector.push_back(tempEntry(1447.44,    120));
  m_tempVector.push_back(tempEntry(1496.28,    130));
}

void TEX00::initThermistorTEE()
{
  // this is an NTC type thermistor
  m_isNTC = true;

  // This data comes from the Veris TW_TE_d0116.pdf datasheet for the
  // "E" (2.2k Ohm) column

  // clear and initialize the vector, ordered from lowest to highest
  // temperature
  m_tempVector.clear();

  //                               ohms        temperature (C)
  m_tempVector.push_back(tempEntry(154464,     -50));
  m_tempVector.push_back(tempEntry(77081,      -40));
  m_tempVector.push_back(tempEntry(40330,      -30));
  m_tempVector.push_back(tempEntry(22032,      -20));
  m_tempVector.push_back(tempEntry(12519,      -10));
  m_tempVector.push_back(tempEntry(7373,       0));
  m_tempVector.push_back(tempEntry(4487,       10));
  m_tempVector.push_back(tempEntry(2814,       20));
  m_tempVector.push_back(tempEntry(2252,       25));
  m_tempVector.push_back(tempEntry(1814,       30));
  m_tempVector.push_back(tempEntry(1199,       40));
  m_tempVector.push_back(tempEntry(811.5,      50));
  m_tempVector.push_back(tempEntry(561,        60));
  m_tempVector.push_back(tempEntry(395.5,      70));
  m_tempVector.push_back(tempEntry(284,        80));
  m_tempVector.push_back(tempEntry(207.4,      90));
  m_tempVector.push_back(tempEntry(153.8,      100));
  m_tempVector.push_back(tempEntry(115.8,      110));
  m_tempVector.push_back(tempEntry(88.3,       120));
  m_tempVector.push_back(tempEntry(68.3,       130));
}

void TEX00::initThermistorTEF()
{
  // this is an NTC type thermistor
  m_isNTC = true;

  // This data comes from the Veris TW_TE_d0116.pdf datasheet for the
  // "F" (3k Ohm) column

  // clear and initialize the vector, ordered from lowest to highest
  // temperature
  m_tempVector.clear();

  //                               ohms        temperature (C)
  m_tempVector.push_back(tempEntry(205800,     -50));
  m_tempVector.push_back(tempEntry(102690,     -40));
  m_tempVector.push_back(tempEntry(53730,      -30));
  m_tempVector.push_back(tempEntry(29346,      -20));
  m_tempVector.push_back(tempEntry(16674,      -10));
  m_tempVector.push_back(tempEntry(9822,       0));
  m_tempVector.push_back(tempEntry(5976,       10));
  m_tempVector.push_back(tempEntry(3750,       20));
  m_tempVector.push_back(tempEntry(3000,       25));
  m_tempVector.push_back(tempEntry(2417,       30));
  m_tempVector.push_back(tempEntry(1598,       40));
  m_tempVector.push_back(tempEntry(1081,       50));
  m_tempVector.push_back(tempEntry(747,        60));
  m_tempVector.push_back(tempEntry(527,        70));
  m_tempVector.push_back(tempEntry(378,        80));
}

void TEX00::initThermistorTEH()
{
  // this is an NTC type thermistor
  m_isNTC = true;

  // This data comes from the Veris TW_TE_d0116.pdf datasheet for the
  // "H" (10k Type 3) column

  // clear and initialize the vector, ordered from lowest to highest
  // temperature
  m_tempVector.clear();

  //                               ohms        temperature (C)
  m_tempVector.push_back(tempEntry(454910,     -50));
  m_tempVector.push_back(tempEntry(245089,     -40));
  m_tempVector.push_back(tempEntry(137307,     -30));
  m_tempVector.push_back(tempEntry(79729,      -20));
  m_tempVector.push_back(tempEntry(47843,      -10));
  m_tempVector.push_back(tempEntry(29588,      0));
  m_tempVector.push_back(tempEntry(18813,      10));
  m_tempVector.push_back(tempEntry(12272,      20));
  m_tempVector.push_back(tempEntry(10000,      25));
  m_tempVector.push_back(tempEntry(8195,       30));
  m_tempVector.push_back(tempEntry(5593,       40));
  m_tempVector.push_back(tempEntry(3894,       50));
  m_tempVector.push_back(tempEntry(2763,       60));
  m_tempVector.push_back(tempEntry(1994,       70));
  m_tempVector.push_back(tempEntry(1462,       80));
  m_tempVector.push_back(tempEntry(1088,       90));
  m_tempVector.push_back(tempEntry(821,        100));
  m_tempVector.push_back(tempEntry(628,        110));
  m_tempVector.push_back(tempEntry(486,        120));
  m_tempVector.push_back(tempEntry(380,        130));
}

void TEX00::initThermistorTEJ()
{
  // this is an NTC type thermistor
  m_isNTC = true;

  // This data comes from the Veris TW_TE_d0116.pdf datasheet for the
  // "J" (10k Dale) column

  // clear and initialize the vector, ordered from lowest to highest
  // temperature
  m_tempVector.clear();

  //                               ohms        temperature (C)
  m_tempVector.push_back(tempEntry(672300,     -50));
  m_tempVector.push_back(tempEntry(337200,     -40));
  m_tempVector.push_back(tempEntry(177200,     -30));
  m_tempVector.push_back(tempEntry(97130,      -20));
  m_tempVector.push_back(tempEntry(55340,      -10));
  m_tempVector.push_back(tempEntry(32660,      0));
  m_tempVector.push_back(tempEntry(19900,      10));
  m_tempVector.push_back(tempEntry(12490,      20));
  m_tempVector.push_back(tempEntry(10000,      25));
  m_tempVector.push_back(tempEntry(8056,       30));
  m_tempVector.push_back(tempEntry(5326,       40));
  m_tempVector.push_back(tempEntry(3602,       50));
  m_tempVector.push_back(tempEntry(2489,       60));
  m_tempVector.push_back(tempEntry(1753,       70));
  m_tempVector.push_back(tempEntry(1258,       80));
  m_tempVector.push_back(tempEntry(917,        90));
  m_tempVector.push_back(tempEntry(679,        100));
  m_tempVector.push_back(tempEntry(511,        110));
  m_tempVector.push_back(tempEntry(389,        120));
  m_tempVector.push_back(tempEntry(301,        130));
}

void TEX00::initThermistorTES()
{
  // this is an NTC type thermistor
  m_isNTC = true;

  // This data comes from the Veris TW_TE_d0116.pdf datasheet for the
  // "S" (10k 3A221) column

  // clear and initialize the vector, ordered from lowest to highest
  // temperature
  m_tempVector.clear();

  //                               ohms        temperature (C)
  m_tempVector.push_back(tempEntry(333562,     -40));
  m_tempVector.push_back(tempEntry(176081,     -30));
  m_tempVector.push_back(tempEntry(96807,      -20));
  m_tempVector.push_back(tempEntry(55252,      -10));
  m_tempVector.push_back(tempEntry(32639,      0));
  m_tempVector.push_back(tempEntry(19901,      10));
  m_tempVector.push_back(tempEntry(12493,      20));
  m_tempVector.push_back(tempEntry(10000,      25));
  m_tempVector.push_back(tempEntry(8055,       30));
  m_tempVector.push_back(tempEntry(5324,       40));
  m_tempVector.push_back(tempEntry(3600,       50));
  m_tempVector.push_back(tempEntry(2486,       60));
  m_tempVector.push_back(tempEntry(1751,       70));
  m_tempVector.push_back(tempEntry(1255,       80));
  m_tempVector.push_back(tempEntry(915,        90));
  m_tempVector.push_back(tempEntry(678,        100));
  m_tempVector.push_back(tempEntry(509,        110));
  m_tempVector.push_back(tempEntry(388,        120));
  m_tempVector.push_back(tempEntry(299,        130));
}

void TEX00::initThermistorTER()
{
  // this is an NTC type thermistor
  m_isNTC = true;

  // This data comes from the Veris TW_TE_d0116.pdf datasheet for the
  // "R" (10k "G" US) column

  // clear and initialize the vector, ordered from lowest to highest
  // temperature
  m_tempVector.clear();

  //                               ohms        temperature (C)
  m_tempVector.push_back(tempEntry(441200,     -50));
  m_tempVector.push_back(tempEntry(239700,     -40));
  m_tempVector.push_back(tempEntry(135300,     -30));
  m_tempVector.push_back(tempEntry(78910,      -20));
  m_tempVector.push_back(tempEntry(47540,      -10));
  m_tempVector.push_back(tempEntry(29490,      0));
  m_tempVector.push_back(tempEntry(18780,      10));
  m_tempVector.push_back(tempEntry(12260,      20));
  m_tempVector.push_back(tempEntry(10000,      25));
  m_tempVector.push_back(tempEntry(8194,       30));
  m_tempVector.push_back(tempEntry(5592,       40));
  m_tempVector.push_back(tempEntry(3893,       50));
  m_tempVector.push_back(tempEntry(2760,       60));
  m_tempVector.push_back(tempEntry(1990,       70));
  m_tempVector.push_back(tempEntry(1458,       80));
  m_tempVector.push_back(tempEntry(1084,       90));
  m_tempVector.push_back(tempEntry(816.8,      100));
  m_tempVector.push_back(tempEntry(623.6,      110));
  m_tempVector.push_back(tempEntry(481.8,      120));
  m_tempVector.push_back(tempEntry(376.4,      130));
}

void TEX00::initThermistorTEM()
{
  // this is an NTC type thermistor
  m_isNTC = true;

  // This data comes from the Veris TW_TE_d0116.pdf datasheet for the
  // "M" (20k NTC) column

  // clear and initialize the vector, ordered from lowest to highest
  // temperature
  m_tempVector.clear();

  //                               ohms        temperature (C)
  m_tempVector.push_back(tempEntry(1267600,    -50));
  m_tempVector.push_back(tempEntry(643800,     -40));
  m_tempVector.push_back(tempEntry(342000,     -30));
  m_tempVector.push_back(tempEntry(189080,     -20));
  m_tempVector.push_back(tempEntry(108380,     -10));
  m_tempVector.push_back(tempEntry(64160,      0));
  m_tempVector.push_back(tempEntry(38440,      10));
  m_tempVector.push_back(tempEntry(24920,      20));
  m_tempVector.push_back(tempEntry(20000,      25));
  m_tempVector.push_back(tempEntry(16144,      30));
  m_tempVector.push_back(tempEntry(10696,      40));
  m_tempVector.push_back(tempEntry(7234,       50));
  m_tempVector.push_back(tempEntry(4992,       60));
  m_tempVector.push_back(tempEntry(3512,       70));
  m_tempVector.push_back(tempEntry(2516,       80));
  m_tempVector.push_back(tempEntry(1833,       90));
  m_tempVector.push_back(tempEntry(1356,       100));
  m_tempVector.push_back(tempEntry(1016,       110));
  m_tempVector.push_back(tempEntry(770,        120));
  m_tempVector.push_back(tempEntry(591,        130));
}

void TEX00::initThermistorTEU()
{
  // this is an NTC type thermistor
  m_isNTC = true;

  // This data comes from the Veris TW_TE_d0116.pdf datasheet for the
  // "U" (20k "D") column

  // clear and initialize the vector, ordered from lowest to highest
  // temperature
  m_tempVector.clear();

  //                               ohms        temperature (C)
  m_tempVector.push_back(tempEntry(803200,     -40));
  m_tempVector.push_back(tempEntry(412800,     -30));
  m_tempVector.push_back(tempEntry(220600,     -20));
  m_tempVector.push_back(tempEntry(112400,     -10));
  m_tempVector.push_back(tempEntry(70200,      0));
  m_tempVector.push_back(tempEntry(41600,      10));
  m_tempVector.push_back(tempEntry(25340,      20));
  m_tempVector.push_back(tempEntry(20000,      25));
  m_tempVector.push_back(tempEntry(15884,      30));
  m_tempVector.push_back(tempEntry(10210,      40));
  m_tempVector.push_back(tempEntry(6718,       50));
  m_tempVector.push_back(tempEntry(4518,       60));
  m_tempVector.push_back(tempEntry(3100,       70));
  m_tempVector.push_back(tempEntry(2168,       80));
  m_tempVector.push_back(tempEntry(1542,       90));
  m_tempVector.push_back(tempEntry(1134,       100));
  m_tempVector.push_back(tempEntry(816,        110));
  m_tempVector.push_back(tempEntry(606,        120));
  m_tempVector.push_back(tempEntry(456,        130));
}

void TEX00::initThermistorTET()
{
  // this is an NTC type thermistor
  m_isNTC = true;

  // This data comes from the Veris TW_TE_d0116.pdf datasheet for the
  // "T" (100k) column

  // clear and initialize the vector, ordered from lowest to highest
  // temperature
  m_tempVector.clear();

  //                               ohms        temperature (C)
  m_tempVector.push_back(tempEntry(3366000,    -40));
  m_tempVector.push_back(tempEntry(1770000,    -30));
  m_tempVector.push_back(tempEntry(971200,     -20));
  m_tempVector.push_back(tempEntry(553400,     -10));
  m_tempVector.push_back(tempEntry(326600,     0));
  m_tempVector.push_back(tempEntry(199000,     10));
  m_tempVector.push_back(tempEntry(124900,     20));
  m_tempVector.push_back(tempEntry(100000,     25));
  m_tempVector.push_back(tempEntry(80580,      30));
  m_tempVector.push_back(tempEntry(53260,      40));
  m_tempVector.push_back(tempEntry(36020,      50));
  m_tempVector.push_back(tempEntry(24880,      60));
  m_tempVector.push_back(tempEntry(17510,      70));
  m_tempVector.push_back(tempEntry(12560,      80));
  m_tempVector.push_back(tempEntry(9164,       90));
  m_tempVector.push_back(tempEntry(6792,       100));
  m_tempVector.push_back(tempEntry(5108,       110));
  m_tempVector.push_back(tempEntry(3894,       120));
  m_tempVector.push_back(tempEntry(3006,       130));
}

