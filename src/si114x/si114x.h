/*
 * Author: Jon Trulson <jtrulson@ics.com>
 * Copyright (c) 2015 Intel Corporation.
 *
 * Thanks to Adafruit for some important clues
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
#include <mraa/i2c.h>
#include <mraa/gpio.h>

#define SI114X_I2C_BUS 0
#define SI114X_DEFAULT_I2C_ADDR 0x60
#define SI114X_HW_KEY 0x17


namespace upm {
  
  /**
   * @brief SI1145 UV light sensor
   * @defgroup si114x libupm-si114x
   * @ingroup adafruit i2c light
   */

  /**
   * @library si114x
   * @sensor si114x
   * @comname SI1145 UV light sensor
   * @altname SI1146, SI1147
   * @type light
   * @man adafruit
   * @web https://www.adafruit.com/products/1777
   * @con i2c
   *
   * @brief API for the SI1145 UV light sensor
   *
   * This module was tested with the Adafruit UV Sensor
   *
   * This device is also capable of measuring IR and visible ambient
   * light as well.  It also supports the ability to use externally
   * attached LED's to perform proximity detection on 3 separate
   * channels.
   *
   * This class currently only supports the retriving of the
   * calculated UV index measured by the device, but enough
   * infrastructure is provided to make it easy to enhance this driver
   * in the future to support additional capabilities, including
   * interrupt support.
   *
   * @image html si114x.jpg
   * @snippet si114x.cxx Interesting
   */
  class SI114X {
  public:

    /**
     * SI114X registers
     */
    typedef enum { REG_PART_ID       = 0x00,
                   REG_REV_ID        = 0x01,
                   REG_SEQ_ID        = 0x02,
                   REG_INT_CFG       = 0x03,
                   REG_IRQ_ENABLE    = 0x04,

                   // these two are not documented in the datasheet,
                   // but are mentioned there, as well as in the
                   // Adafruit example
                   REG_IRQ_MODE1     = 0x05,
                   REG_IRQ_MODE2     = 0x06,

                   REG_HW_KEY        = 0x07,
                   REG_MEAS_RATE0    = 0x08,
                   REG_MEAS_RATE1    = 0x09,

                   REG_PS_LED21      = 0x0f,
                   REG_PS_LED3       = 0x10,

                   REG_UCOEF0        = 0x13,
                   REG_UCOEF1        = 0x14,
                   REG_UCOEF2        = 0x15,
                   REG_UCOEF3        = 0x16,
                   REG_PARAM_WR      = 0x17,
                   REG_COMMAND       = 0x18,

                   REG_RESPONSE      = 0x20,
                   REG_IRQ_STATUS    = 0x21,
                   REG_ALS_VIS_DATA0 = 0x22,
                   REG_ALS_VIS_DATA1 = 0x23,
                   REG_ALS_IR_DATA0  = 0x24,
                   REG_ALS_IR_DATA1  = 0x25,
                   REG_PS1_DATA0     = 0x26,
                   REG_PS1_DATA1     = 0x27,
                   REG_PS2_DATA0     = 0x28,
                   REG_PS2_DATA1     = 0x29,
                   REG_PS3_DATA0     = 0x2a,
                   REG_PS3_DATA1     = 0x2b,
                   REG_AUX_UVINDEX0  = 0x2c,
                   REG_AUX_UVINDEX1  = 0x2d,
                   REG_PARAM_READ    = 0x2e,

                   REG_CHIP_STAT     = 0x30,

                   REG_ANA_IN_KEY0   = 0x3b,
                   REG_ANA_IN_KEY1   = 0x3c,
                   REG_ANA_IN_KEY2   = 0x3d,
                   REG_ANA_IN_KEY3   = 0x3e
    } SI114X_REG_T;
    
    /**
     * Parameter memory (PARAM)
     */
    typedef enum { PARAM_I2C_ADDDR         = 0x00,
                   PARAM_CHLIST            = 0x01,
                   PARAM_PSLED12_SEL       = 0x02,
                   PARAM_PSLED3_SEL        = 0x03,

                   PARAM_PS_ENCODING       = 0x05,
                   PARAM_ALS_ENCODING      = 0x06,
                   PARAM_PS1_ADCMUX        = 0x07,
                   PARAM_PS2_ADCMUX        = 0x08,
                   PARAM_PS3_ADCMUX        = 0x09,
                   PARAM_PS_ADC_COUNT      = 0x0a,
                   PARAM_PS_ADC_GAIN       = 0x0b,
                   PARAM_PS_ADC_MISC       = 0x0c,

                   PARAM_ALS_IR_ADCMUX     = 0x0e,
                   PARAM_AUX_ADCMUX        = 0x0f,
                   PARAM_ALS_VIS_ADC_COUNT = 0x10,
                   PARAM_ALS_VIS_ADC_GAIN  = 0x11,
                   PARAM_ALS_VIS_ADC_MISC  = 0x12,

                   PARAM_LED_REC           = 0x1c,
                   PARAM_ALS_IR_ADC_COUNT  = 0x1d,
                   PARAM_ALS_IR_ADX_GAIN   = 0x1e,
                   PARAM_ALS_IR_ADC_MISC   = 0x1f
    } SI114X_PARAM_T;
    
    /**
     * Commands (written to the REG_COMMAND register)
     */
    typedef enum { CMD_NOOP          = 0x00, // clear RESPONSE reg
                   CMD_RESET         = 0x01,
                   CMD_BUSADDR       = 0x02,

                   CMD_PS_FORCE      = 0x05,
                   CMD_GET_CAL       = 0x12,
                   CMD_ALS_FORCE     = 0x06,
                   CMD_PSALS_FORCE   = 0x07,

                   CMD_PS_PAUSE      = 0x09,
                   CMD_ALS_PAUSE     = 0x0a,
                   CMD_PSALS_PAUSE   = 0x0b,

                   CMD_PS_AUTO       = 0x0d,
                   CMD_ALS_AUTO      = 0x0e,
                   CMD_PSALS_AUTO    = 0x0f,

                   CMD_PARAM_QUERY   = 0x80, // or'd with PARAM_T value
                   CMD_PARAM_SET     = 0xa0  // or'd with PARAM_T value
    } SI114X_CMD_T;


    /**
     * Channel List enable bits
     */
    typedef enum { CHLIST_EN_PS1     = 0x01, // proximity sense 1-3
                   CHLIST_EN_PS2     = 0x02,
                   CHLIST_EN_PS3     = 0x04,

                   CHLIST_EN_ALS_VIS = 0x10, // ambient light sense
                   CHLIST_EN_ALS_IR  = 0x20,
                   CHLIST_EN_AUX     = 0x40, // AUX sense
                   CHLIST_EN_UV      = 0x80  // UV sense
    } SI114X_CHLIST_BITS_T;

    /**
     * Error codes from the RESPONSE register
     */
    typedef enum { ERR_NONE          = 0x00, // no error if high nibble is 0
                                             // lower nibble is a counter
                   ERR_INVALID_SET   = 0x80, // invalid setting
                   ERR_PS1_ADC_OVER  = 0x88, // overflows
                   ERR_PS2_ADC_OVER  = 0x89,
                   ERR_PS3_ADC_OVER  = 0x8a,
                   ERR_ALS_VIS_ADC_OVER  = 0x8c,
                   ERR_ALS_IR_ADC_OVER   = 0x8d,
                   ERR_AUX_ADC_OVER  = 0x8e
    } SI114X_ERR_T;
    

    /**
     * Interrupt enable bits
     */
    typedef enum { IRQEN_ALS_IE      = 0x01,
                   IRQEN_PS1_IE      = 0x04,
                   IRQEN_PS2_IE      = 0x08,
                   IRQEN_PS3_IE      = 0x10
    } SI114X_IRQEN_BITS_T;

    /**
     * si114x constructor
     *
     * @param bus i2c bus to use
     * @param address the address for this device
     */
    SI114X(int bus, uint8_t address = SI114X_DEFAULT_I2C_ADDR);

    /**
     * SI114X Destructor
     */
    ~SI114X();

    /**
     * Write byte value into register
     *
     * @param reg register location to write into
     * @param byte byte to write
     * @return true if successful
     */
    bool writeByte(uint8_t reg, uint8_t byte);

    /**
     * Read byte value from register
     *
     * @param reg register location to read from
     * @return value at specified register
     */
    uint8_t readByte(uint8_t reg);

    /**
     * Read word value from register.
     *
     * @param reg register location to read from
     * @return value at specified register
     */
    uint16_t readWord(uint8_t reg);

    /**
     * disable interrupts and auto measuring, issue a device reset,
     * and then set the hardware key.
     */
    void reset();

    /**
     * set the UV calibration values.  The constructor sets default
     * values for you, so you only need this function if you need
     * different values for your device and situation.  If you set new
     * values here, be sure to do so before calling initialize().
     *
     * @param uvcoeff0 coefficient for REG_UCOEF0
     * @param uvcoeff1 coefficient for REG_UCOEF1
     * @param uvcoeff2 coefficient for REG_UCOEF2
     * @param uvcoeff3 coefficient for REG_UCOEF3
     *
     */
    void setUVCalibration(uint8_t uvcoeff0, uint8_t uvcoeff1, uint8_t uvcoeff2,
                          uint8_t uvcoeff3);

    /**
     * write a value to parameter memory.
     *
     * @param param the SI114X_PARAM_T register to write
     * @param value the value to write
     */
    void writeParam(SI114X_PARAM_T param, uint8_t value);
    
    /**
     * read a value from parameter memory
     *
     * @param param the SI114X_PARAM_T register to read
     * @return the value
     */
    uint8_t readParam(SI114X_PARAM_T param);

    /**
     * Reset and initialize device, start auto sampling
     */
    void initialize();

    /**
     * update stored values.  You should call this before calling
     * getUVIndex()
     */
    void update();

    /**
     * Read the currently measured UV Index value
     *
     * @return UV Index value
     */
    float getUVIndex() { return m_uvIndex; };

  private:
    mraa_i2c_context m_i2c;
    uint8_t m_addr;
    // UV calibration values
    uint8_t m_uv_cal[4];
    // updated by update()
    float m_uvIndex;
  };
}


