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
#pragma once

#include <string>

#include <modbus/modbus.h>

namespace upm {

  /**
   * @brief H803X Energy Meter
   * @defgroup h803x libupm-h803x
   * @ingroup uart electric
   */

  /**
   * @library h803x
   * @sensor h803x
   * @comname Veris H803X Energy Meter
   * @type electic
   * @man veris
   * @con uart
   * @web http://www.veris.com/Item/H8035-0100-2.aspx
   *
   * @brief UPM API for the Veris H803X Energy Meter
   *
   * This module implements support for the Veris H8035 and H8036
   * Energy Meters.
   *
   * The H8036 is similar to the H8035, but provides much more data.
   *
   * The Enercept H8035/H8036 is an innovative three-phase networked
   * (Modbus RTU) power transducer that combines electronics and high
   * accuracy industrial grade CTs in a single package. The need for
   * external electrical enclosures is eliminated, greatly reducing
   * installation time and cost. Color-coordination between voltage
   * leads and CTs makes phase matching easy. Additionally, these
   * transducers automatically detect and compensate for phase
   * reversal, eliminating the concern of CT load orientation. Up to
   * 63 Transducers can be daisy-chained on a single RS-485 network.
   *
   * This module was developed using libmodbus 3.1.2, and the H8035.
   * The H8036 has not been tested.  libmodbus 3.1.2 must be present
   * for this module to build.
   *
   * It was developed using an RS232->RS485 interface.  You cannot use
   * the built in MCU TTL UART pins for accessing this device -- you
   * must use a full Serial RS232->RS485 or USB-RS485 interface
   * connected via USB.
   *
   * @snippet h803x.cxx Interesting
   */

  class H803X {
  public:

    // MODBUS holding registers.  These offsets are for the MSW only.
    // The LSW always follows, though they are not enumerated here.
    // These are all 2 register (32-bit total (16b HSW + 16b LSW))
    // quantities, in IEEE 754 floating point format.
    typedef enum {
      // these two registers are used only for presetConsumption()
      HOLDING_CONSUMPTION_KWH_INT_L             = 0, // preset use only
      HOLDING_CONSUMPTION_KWH_INT_H             = 1, // preset use only

      // H8035/H8036
      HOLDING_CONSUMPTION_KWH                   = 258, // floating point data

      HOLDING_REAL_POWER_KW                     = 260,

      // H8036 only
      HOLDING_REACTIVE_POWER_KVAR               = 262,
      HOLDING_APPARENT_POWER_KVA                = 264,
      HOLDING_POWER_FACTOR                      = 266,
      HOLDING_VOLTS_LINE_TO_LINE                = 268,
      HOLDING_VOLTS_LINE_TO_NEUTRAL             = 270,
      HOLDING_CURRENT                           = 272,
      HOLDING_REAL_POWER_PHASE_A_KWH            = 274,
      HOLDING_REAL_POWER_PHASE_B_KWH            = 276,
      HOLDING_REAL_POWER_PHASE_C_KWH            = 278,
      HOLDING_POWER_FACTOR_PHASE_A              = 280,
      HOLDING_POWER_FACTOR_PHASE_B              = 282,
      HOLDING_POWER_FACTOR_PHASE_C              = 284,
      HOLDING_VOLTS_PHASE_AB                    = 286,
      HOLDING_VOLTS_PHASE_BC                    = 288,
      HOLDING_VOLTS_PHASE_AC                    = 290,
      HOLDING_VOLTS_PHASE_AN                    = 292,
      HOLDING_VOLTS_PHASE_BN                    = 294,
      HOLDING_VOLTS_PHASE_CN                    = 296,
      HOLDING_CURRENT_PHASE_A                   = 298,
      HOLDING_CURRENT_PHASE_B                   = 300,
      HOLDING_CURRENT_PHASE_C                   = 302,
      HOLDING_AVG_REAL_POWER_KW                 = 304,
      HOLDING_MIN_REAL_POWER_KW                 = 306,
      HOLDING_MAX_REAL_POWER_KW                 = 308
    } HOLDING_REGS_T;

    // these enums are used by presetConsumption() to scale the value
    // properly depending on the devices' current capacity.
    typedef enum {
      MULT_100A                                 = 128, // 100A devices
      MULT_300A_400A                            = 32,
      MULT_800A                                 = 16,
      MULT_1600A                                = 8,
      MULT_2400A                                = 4
    } MULTIPLIERS_T;

    /**
     * H803X constructor
     *
     * @param device Path to the serial device
     * @param address The MODBUS slave address
     * @param baud The baudrate of the device.  Default: 9600
     * @param bits The number of bits per byte.  Default: 8
     * @param parity The parity of the connection, 'N' for None, 'E'
     * for Even, 'O' for Odd.  Default: 'N'
     * @param stopBits The number of stop bits.  Default: 2
     */
    H803X(std::string device, int address, int baud=9600, int bits=8,
          char parity='N', int stopBits=2);

    /**
     * H803X Destructor
     */
    ~H803X();

    /**
     * Read current values from the sensor and update internal stored
     * values.  This method must be called prior to querying any
     * values.
     */
    void update();

    /**
     * Return a string corresponding the the device's MODBUS slave ID.
     *
     * @return string represnting the MODBUS slave ID
     */
    std::string getSlaveID();

    /**
     * Set a new MODBUS slave address.  This is useful if you have
     * multiple H803X devices on a single bus.  When this method is
     * called, the current stored data is cleared, and a new attempt
     * is made to determine whether the target device is an H8035 or
     * H8036.
     *
     * @param addr The new slave address to set
     */
    void setSlaveAddress(int addr);

    /**
     * Preset the kWh accumulated Consumption registers to a
     * predefined value.  This is generally not advised, but is
     * provided for those installations that might require it.  The
     * multiplier depends on the current range of your device.  Be
     * sure to select the right multiplier for your devices'
     * supported current capacity.
     *
     * @param value The desired value for the consumption accumulator
     * registers in kWh.
     * @param multiplier The correct MULTIPLIERS_T value for your device.
     */
    void presetConsumption(float value, MULTIPLIERS_T multiplier);

    /**
     * Return the accumulated consumption value, in kWh. update() must
     * have been called prior to calling this method.
     *
     * @return The accumulated consumption.
     */
    float getConsumption()
    {
      return m_consumptionkWh;
    };

    /**
     * Return the real power value in kW. update() must have been
     * called prior to calling this method.
     *
     * @return The real power value in kW.
     */
    float getRealPower()
    {
      return m_realPowerkW;
    };

    /**
     * Return the reactive power value in kVAR (kilo-volt Amperes
     * Reactive). update() must have been called prior to calling this
     * method.
     *
     * @return The reactive power value in kVAR.
     */
    float getReactivePower()
    {
      return m_reactivePowerkVAR;
    };

    /**
     * Return the apparent power value in kVA. update() must have been
     * called prior to calling this method.
     *
     * @return The apparent power value in kVA.
     */
    float getApparentPower()
    {
      return m_apparentPowerkVA;
    };

    /**
     * Return the power factor value. update() must have been called
     * prior to calling this method.
     *
     * @return The power factor.
     */
    float getPowerFactor()
    {
      return m_powerFactor;
    };

    /**
     * Return the voltage line to line value. update() must have been
     * called prior to calling this method.
     *
     * @return The voltage, line to line.
     */
    float getVoltsLineToLine()
    {
      return m_voltsLineToLine;
    };

    /**
     * Return the voltage line to neutral. update() must have been
     * called prior to calling this method.
     *
     * @return The voltage, line to neutral.
     */
    float getVoltsLineToNeutral()
    {
      return m_voltsLineToNeutral;
    };

    /**
     * Return the current value in amps. update() must have been
     * called prior to calling this method.
     *
     * @return The current value in amps.
     */
    float getCurrent()
    {
      return m_current;
    };

    /**
     * Return the real power for phase A. update() must have been
     * called prior to calling this method.
     *
     * @return The real power for phase A.
     */
    float getRealPowerPhaseA()
    {
      return m_realPowerPhaseAkW;
    };

    /**
     * Return the real power for phase B. update() must have been
     * called prior to calling this method.
     *
     * @return The real power for phase B.
     */
    float getRealPowerPhaseB()
    {
      return m_realPowerPhaseBkW;
    };

    /**
     * Return the real power for phase C. update() must have been
     * called prior to calling this method.
     *
     * @return The real power for phase C.
     */
    float getRealPowerPhaseC()
    {
      return m_realPowerPhaseCkW;
    };

    /**
     * Return the power factor for phase A. update() must have been
     * called prior to calling this method.
     *
     * @return The power factor for phase A.
     */
    float getPowerFactorPhaseA()
    {
      return m_powerFactorPhaseA;
    };

    /**
     * Return the power factor for phase B. update() must have been
     * called prior to calling this method.
     *
     * @return The power factor for phase B.
     */
    float getPowerFactorPhaseB()
    {
      return m_powerFactorPhaseB;
    };

    /**
     * Return the power factor for phase C. update() must have been
     * called prior to calling this method.
     *
     * @return The power factor for phase C.
     */
    float getPowerFactorPhaseC()
    {
      return m_powerFactorPhaseC;
    };

    /**
     * Return the voltage for phase A to B. update() must have been
     * called prior to calling this method.
     *
     * @return The voltage for phase A to B.
     */
    float getVoltsPhaseAToB()
    {
      return m_voltsPhaseAB;
    };

    /**
     * Return the voltage for phase B to C. update() must have been
     * called prior to calling this method.
     *
     * @return The voltage for phase B to C.
     */
    float getVoltsPhaseBToC()
    {
      return m_voltsPhaseBC;
    };

    /**
     * Return the voltage for phase A to B. update() must have been
     * called prior to calling this method.
     *
     * @return The voltage for phase A to B.
     */
    float getVoltsPhaseAToC()
    {
      return m_voltsPhaseAC;
    };

    /**
     * Return the voltage for phase A to neutral. update() must have
     * been called prior to calling this method.
     *
     * @return The voltage for phase A to neutral.
     */
    float getVoltsPhaseAToNeutral()
    {
      return m_voltsPhaseAN;
    };

    /**
     * Return the voltage for phase B to neutral. update() must have
     * been called prior to calling this method.
     *
     * @return The voltage for phase B to neutral.
     */
    float getVoltsPhaseBToNeutral()
    {
      return m_voltsPhaseBN;
    };

    /**
     * Return the voltage for phase C to neutral. update() must have
     * been called prior to calling this method.
     *
     * @return The voltage for phase C to neutral.
     */
    float getVoltsPhaseCToNeutral()
    {
      return m_voltsPhaseCN;
    };

    /**
     * Return the current for phase A. update() must have been called
     * prior to calling this method.
     *
     * @return The current for phase A.
     */
    float getCurrentPhaseA()
    {
      return m_currentPhaseA;
    };

    /**
     * Return the current for phase B. update() must have been called
     * prior to calling this method.
     *
     * @return The current for phase B.
     */
    float getCurrentPhaseB()
    {
      return m_currentPhaseB;
    };

    /**
     * Return the current for phase C. update() must have been called
     * prior to calling this method.
     *
     * @return The current for phase C.
     */
    float getCurrentPhaseC()
    {
      return m_currentPhaseC;
    };

    /**
     * Return the average real power. update() must have been called
     * prior to calling this method.
     *
     * @return The average real power.
     */
    float getAvgRealPower()
    {
      return m_avgRealPowerkW;
    };

    /**
     * Return the minimum real power. update() must have been called
     * prior to calling this method.
     *
     * @return The minimum real power.
     */
    float getMinRealPower()
    {
      return m_minRealPowerkW;
    };

    /**
     * Return the maximum real power. update() must have been called
     * prior to calling this method.
     *
     * @return The maximum real power.
     */
    float getMaxRealPower()
    {
      return m_maxRealPowerkW;
    };

    /**
     * Enable or disable debugging output.  This primarily enables and
     * disables libmodbus debugging output.
     *
     * @param enable true to enable debugging, false otherwise
     */
    void setDebug(bool enable);

    /**
     * Indicate whether the connected device is an H8035 or an H8036.
     * The H8036 provides many more data registers.
     *
     * @return true if we are using an H8036, false otherwise.
     */
    bool isH8036()
    {
      return m_isH8036;
    };

  protected:
    // holding registers
    int readHoldingRegs(HOLDING_REGS_T reg, int len, uint16_t *buf);
    void writeHoldingReg(HOLDING_REGS_T reg, int value);

    // clear out all stored data
    void clearData();
    
    // MODBUS context
    modbus_t *m_mbContext;

    // test to see if the connected device is an H8036, and set
    // m_isH8036 appropriately
    void testH8036();

    // Is this an H8036 (has extended registers)
    bool m_isH8036;

  private:
    bool m_debugging;

    // data

    // H8035 / H8036
    float m_consumptionkWh;
    float m_realPowerkW;

    // H8036 only
    float m_reactivePowerkVAR;
    float m_apparentPowerkVA;
    float m_powerFactor;
    float m_voltsLineToLine;
    float m_voltsLineToNeutral;
    float m_current; // in amps
    float m_realPowerPhaseAkW;
    float m_realPowerPhaseBkW;
    float m_realPowerPhaseCkW;
    float m_powerFactorPhaseA;
    float m_powerFactorPhaseB;
    float m_powerFactorPhaseC;
    float m_voltsPhaseAB;
    float m_voltsPhaseBC;
    float m_voltsPhaseAC;
    float m_voltsPhaseAN;
    float m_voltsPhaseBN;
    float m_voltsPhaseCN;
    float m_currentPhaseA;
    float m_currentPhaseB;
    float m_currentPhaseC;
    float m_avgRealPowerkW;
    float m_minRealPowerkW;
    float m_maxRealPowerkW;
  };
}
