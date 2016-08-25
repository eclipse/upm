/*
 * Author: Jon Trulson <jtrulson@ics.com>
 * Copyright (c) 2015 Intel Corporation.
 *
 * Thanks to Seeed Studio for a working arduino sketch
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
#include <iostream>

#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>
#include <termios.h>
#include <sys/time.h>
#include <sys/select.h>
#include <sys/types.h>
#include <sys/stat.h>

#include <mraa/uart.h>

#define GROVESCAM_DEFAULT_UART 0

#define GROVESCAM_DEFAULT_CAMERA_ADDR 0

namespace upm {
    /**
     * @brief Grove Serial Camera library
     * @defgroup grovescam libupm-grovescam
     * @ingroup seeed uart other
     */

    /**
     * @library grovescam
     * @sensor grovescam
     * @comname Grove Serial Camera
     * @type other
     * @man seeed
     * @con uart
     * @web http://www.seeedstudio.com/wiki/Grove_-_Serial_Camera_Kit
     *
     * @brief API for the Grove Serial Camera
     *
     * The driver was tested with the Grove Serial Camera. There is
     * no protocol documentation currently available, so this module
     * was developed based completely on the Seeed Studio* Arduino*
     * sketch.
     *
     * It is connected via a UART at 115,200 baud.
     * 
     * @image html grovescam.jpg
     * @snippet grovescam.cxx Interesting
     */

  class GROVESCAM {
  public:

    static const unsigned int MAX_PKT_LEN = 128;

    typedef enum {
      FORMAT_VGA                   = 7, // 640x480
      FORMAT_CIF                   = 5, // 352×288
      FORMAT_OCIF                  = 3  // ??? (maybe they meant QCIF?)
    } PIC_FORMATS_T;

    /**
     * Grove Serial Camera constructor
     *
     * @param uart Default UART to use (0 or 1)
     * @param camAddr 3-bit address identifier of the camera; default is 0
     */
    GROVESCAM(int uart, uint8_t camAddr=GROVESCAM_DEFAULT_CAMERA_ADDR);

    /**
     * GROVESCAM destructor
     */
    ~GROVESCAM();

    /**
     * Checks to see if there is data available for reading
     *
     * @param millis Number of milliseconds to wait; 0 means no waiting.
     * @return True if there is data available for reading
     */
    bool dataAvailable(unsigned int millis);

    /**
     * Reads any available data into a user-supplied buffer. Note: the
     * call blocks until data is available to be read. Use
     * dataAvailable() to determine whether there is data available
     * beforehand, to avoid blocking.
     *
     * @param buffer Buffer to hold the data read
     * @param len Length of the buffer
     * @return Number of bytes read
     */
    int readData(uint8_t *buffer, int len);

    /**
     * Writes the data in the buffer to the device
     *
     * @param buffer Buffer to hold the data read
     * @param len Length of the buffer
     * @return Number of bytes written
     */
    int writeData(uint8_t *buffer, int len);

    /**
     * Sets up proper tty I/O modes and the baud rate. For this device, the default
     * baud rate is 9,600 (B9600).
     *
     * @param baud Desired baud rate
     * @return True if successful
     */
    bool setupTty(speed_t baud=B115200);

    /**
     * Reads serial input and discards until no more characters are available
     *
     */
    void drainInput();

    /**
     * Initializes the camera
     *
     */
    bool init();

    /**
     * Tells the camera to prepare for a capture
     *
     * @param fmt One of the PIC_FORMATS_T values
     */
    bool preCapture(PIC_FORMATS_T fmt=FORMAT_VGA);

    /**
     * Starts the capture
     *
     * @return True if successful
     */
    bool doCapture();

    /**
     * Stores the captured image in a file
     *
     * @param fname Name of the file to write
     * @return True if successful
     */
    bool storeImage(const char *fname);

    /**
     * Returns the picture length. Note: this is only valid after
     * doCapture() has run successfully.
     *
     * @return Image length
     */
    int getImageSize() { return m_picTotalLen; };

  protected:
    int ttyFd() { return m_ttyFd; };

  private:
    mraa_uart_context m_uart;
    int m_ttyFd;

    uint8_t m_camAddr;
    int m_picTotalLen;
  };
}


