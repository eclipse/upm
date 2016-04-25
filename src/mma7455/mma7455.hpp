/*
 * Author: Yevgeniy Kiveisha <yevgeniy.kiveisha@intel.com>
 * Copyright (c) 2014 Intel Corporation.
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
#include <mraa/i2c.hpp>

#define ADDR               0x1D // device address

// Register names according to the datasheet.
// Register 0x1C is sometimes called 'PW', and sometimes 'PD'.
// The two reserved registers cannot be used.
#define MMA7455_XOUTL 0x00      // Read only, Output Value X LSB
#define MMA7455_XOUTH 0x01      // Read only, Output Value X MSB
#define MMA7455_YOUTL 0x02      // Read only, Output Value Y LSB
#define MMA7455_YOUTH 0x03      // Read only, Output Value Y MSB
#define MMA7455_ZOUTL 0x04      // Read only, Output Value Z LSB
#define MMA7455_ZOUTH 0x05      // Read only, Output Value Z MSB
#define MMA7455_XOUT8 0x06      // Read only, Output Value X 8 bits
#define MMA7455_YOUT8 0x07      // Read only, Output Value Y 8 bits
#define MMA7455_ZOUT8 0x08      // Read only, Output Value Z 8 bits
#define MMA7455_STATUS 0x09     // Read only, Status Register
#define MMA7455_DETSRC 0x0A     // Read only, Detection Source Register
#define MMA7455_TOUT 0x0B       // Temperature Output Value (Optional)
#define MMA7455_RESERVED1 0x0C  // Reserved
#define MMA7455_I2CAD 0x0D      // Read/Write, I2C Device Address
#define MMA7455_USRINF 0x0E     // Read only, User Information (Optional)
#define MMA7455_WHOAMI 0x0F     // Read only, "Who am I" value (Optional)
#define MMA7455_XOFFL 0x10      // Read/Write, Offset Drift X LSB
#define MMA7455_XOFFH 0x11      // Read/Write, Offset Drift X MSB
#define MMA7455_YOFFL 0x12      // Read/Write, Offset Drift Y LSB
#define MMA7455_YOFFH 0x13      // Read/Write, Offset Drift Y MSB
#define MMA7455_ZOFFL 0x14      // Read/Write, Offset Drift Z LSB
#define MMA7455_ZOFFH 0x15      // Read/Write, Offset Drift Z MSB
#define MMA7455_MCTL 0x16       // Read/Write, Mode Control Register
#define MMA7455_INTRST 0x17     // Read/Write, Interrupt Latch Reset
#define MMA7455_CTL1 0x18       // Read/Write, Control 1 Register
#define MMA7455_CTL2 0x19       // Read/Write, Control 2 Register
#define MMA7455_LDTH 0x1A       // Read/Write, Level Detection Threshold Limit Value
#define MMA7455_PDTH 0x1B       // Read/Write, Pulse Detection Threshold Limit Value
#define MMA7455_PD 0x1C         // Read/Write, Pulse Duration Value
#define MMA7455_LT 0x1D         // Read/Write, Latency Time Value (between pulses)
#define MMA7455_TW 0x1E         // Read/Write, Time Window for Second Pulse Value
#define MMA7455_RESERVED2 0x1F  // Reserved

// Defines for the bits to be able to change
// between the bit number and the binary definition.
// By using the bit number, programming MMA7455
// is like programming an AVR microcontroller.
// But instead of using "(1<<X)", or "_BV(X)",
// the Arduino "bit(X)" is used.
#define MMA7455_D0 0
#define MMA7455_D1 1
#define MMA7455_D2 2
#define MMA7455_D3 3
#define MMA7455_D4 4
#define MMA7455_D5 5
#define MMA7455_D6 6
#define MMA7455_D7 7

// Status Register
#define MMA7455_DRDY MMA7455_D0
#define MMA7455_DOVR MMA7455_D1
#define MMA7455_PERR MMA7455_D2

// Mode Control Register
#define MMA7455_MODE0 MMA7455_D0
#define MMA7455_MODE1 MMA7455_D1
#define MMA7455_GLVL0 MMA7455_D2
#define MMA7455_GLVL1 MMA7455_D3
#define MMA7455_STON MMA7455_D4
#define MMA7455_SPI3W MMA7455_D5
#define MMA7455_DRPD MMA7455_D6

// Control 1 Register
#define MMA7455_INTPIN MMA7455_D0
#define MMA7455_INTREG0 MMA7455_D1
#define MMA7455_INTREG1 MMA7455_D2
#define MMA7455_XDA MMA7455_D3
#define MMA7455_YDA MMA7455_D4
#define MMA7455_ZDA MMA7455_D5
#define MMA7455_THOPT MMA7455_D6
#define MMA7455_DFBW MMA7455_D7

// Control 2 Register
#define MMA7455_LDPL MMA7455_D0
#define MMA7455_PDPL MMA7455_D1
#define MMA7455_DRVO MMA7455_D2

// Interrupt Latch Reset Register
#define MMA7455_CLR_INT1 MMA7455_D0
#define MMA7455_CLR_INT2 MMA7455_D1

// Detection Source Register
#define MMA7455_INT1 MMA7455_D0
#define MMA7455_INT2 MMA7455_D1
#define MMA7455_PDZ MMA7455_D2
#define MMA7455_PDY MMA7455_D3
#define MMA7455_PDX MMA7455_D4
#define MMA7455_LDZ MMA7455_D5
#define MMA7455_LDY MMA7455_D6
#define MMA7455_LDX MMA7455_D7

// I2C Device Address Register
#define MMA7455_I2CDIS MMA7455_D7

#define HIGH               1
#define LOW                0

namespace upm {

typedef union {
    struct {
        unsigned char x_lsb;
        unsigned char x_msb;
        unsigned char y_lsb;
        unsigned char y_msb;
        unsigned char z_lsb;
        unsigned char z_msb;
    } reg;

    struct {
        short x;
        short y;
        short z;
    } value;
} accelData;

#define BIT(n) (1<<n)

/**
 * @brief MMA7455 Accelerometer library
 * @defgroup mma7455 libupm-mma7455
 * @ingroup generic i2c accelerometer
 */
/**
 * @library mma7455
 * @sensor mma7455
 * @comname MMA7455 Accelerometer Sensor
 * @type accelerometer
 * @man generic
 * @con i2c
 *
 * @brief API for the MMA7455 Accelerometer
 *
 * This module defines the MMA7455 interface for libmma7455
 *
 * @image html mma7455.jpg
 * @snippet mma7455.cxx Interesting
 */
class MMA7455 {
    public:
        /**
         * Instantiates an MMA7455 object
         *
         * @param bus Number of the used bus
         * @param devAddr Address of the used I2C device
         */
        MMA7455 (int bus=0, int devAddr=0x1D);

        /**
         * Returns the name of the component
         *
         * @return Name of the component
         */
        std::string name()
        {
            return m_name;
        }

        /**
         * Calibrates the sensor
         *
         * @return 0 (MRAA_SUCCESS) if successful; non-zero otherwise
         */
        mraa::Result calibrate ();

        /**
         * Reads X-axis, Y-axis, and Z-axis acceleration data
         *
         * @param ptrX X-axis
         * @param ptrY Y-axis
         * @param ptrZ Z-axis
         *
         * @return 0 (MRAA_SUCCESS) if successful; non-zero otherwise
         */
        mraa::Result readData (short * ptrX, short * ptrY, short * ptrZ);

#ifdef SWIGJAVA
        /**
         * Reads X-axis, Y-axis, and Z-axis acceleration data
         *
         * @return Array containing X, Y, Z acceleration data
         */
        short *readData ();
#endif
        /**
         * Internal function for reading I2C data
         *
         * @param reg Register address
         * @param buffer Register data buffer
         * @param len Buffer size
         */
        int i2cReadReg (unsigned char reg, uint8_t *buffer, int len);

        /**
         * Internal function for writing I2C data
         *
         * @param reg Register address
         * @param buffer Register data buffer
         * @param len Buffer size
         */
        mraa::Result i2cWriteReg (unsigned char reg, uint8_t *buffer, int len);

    private:
        std::string m_name;
        int              m_controlAddr;
        int              m_bus;
        mraa::I2c  m_i2ControlCtx;
};

}
