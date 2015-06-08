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
     * @ingroup seeed serial
     */

    /**
     * @library grovescam
     * @sensor grovescam
     * @comname Grove Serial Camera
     * @category other
     * @manufacturer seeed
     * @connection uart
     * @web http://www.seeedstudio.com/wiki/Grove_-_Serial_Camera_Kit
     *
     * @brief API for the Grove Serial Camera
     *
     * The driver was tested with the Grove Serial Camera.  There is
     * no protocol documentation currently available, so this module
     * was developed based completely on the Seeed Studio Arduino
     * sketch.
     *
     * It is connected via a UART at 115200 baud.
     *
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
     * GROVESCAM module constructor
     *
     * @param uart default uart to use (0 or 1)
     * @param camAddr the 3-bit address identifier of the camera, default 0
     */
    GROVESCAM(int uart, uint8_t camAddr=GROVESCAM_DEFAULT_CAMERA_ADDR);

    /**
     * GROVESCAM module Destructor
     */
    ~GROVESCAM();

    /**
     * check to see if there is data available for reading
     *
     * @param millis number of milliseconds to wait, 0 means no wait.
     * @return true if there is data available to be read
     */
    bool dataAvailable(unsigned int millis);

    /**
     * read any available data into a user-supplied buffer.  Note, the
     * call will block until data is available to be read.  Use
     * dataAvailable() to determine whether there is data available
     * beforehand, to avoid blocking.
     *
     * @param buffer the buffer to hold the data read
     * @param len the length of the buffer
     * @return the number of bytes read
     */
    int readData(uint8_t *buffer, size_t len);

    /**
     * write the data in buffer to the device
     *
     * @param buffer the buffer to hold the data read
     * @param len the length of the buffer
     * @return the number of bytes written
     */
    int writeData(uint8_t *buffer, size_t len);

    /**
     * setup the proper tty i/o modes and the baudrate.  The default
     * baud rate is 9600 (B9600) for this device.
     *
     * @param baud the desired baud rate.
     * @return true if successful
     */
    bool setupTty(speed_t baud=B115200);

    /**
     * read serial input and discard until no more characters are available
     *
     */
    void drainInput();

    /**
     * initialize the camera
     *
     */
    bool init();

    /**
     * tell camera to prepare for a capture
     *
     * @param fmt one of the PIC_FORMATS_T values
     */
    bool preCapture(PIC_FORMATS_T fmt=FORMAT_VGA);

    /**
     * start the capture
     *
     * @return true if successful
     */
    bool doCapture();

    /**
     * store the captured image in a file
     *
     * @param fname the name of the file to write
     * @return true if successful
     */
    bool storeImage(char *fname);

    /**
     * return the picture length.  Note, this is only valid after
     * doCapture() has run successfully.
     *
     * @return the image length
     */
    int getImageSize() { return m_picTotalLen; };

  protected:
    int ttyFd() { return m_ttyFd; };
    int setTtyFd(int fd) { m_ttyFd = fd; };

  private:
    mraa_uart_context m_uart;
    int m_ttyFd;

    uint8_t m_camAddr;
    int m_picTotalLen;
  };
}


