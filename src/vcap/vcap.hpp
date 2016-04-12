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
#include <iostream>

#include <errno.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <jpeglib.h>
#include <linux/videodev2.h>

#define VCAP_DEFAULT_VIDEODEV "/dev/video0"
#define VCAP_DEFAULT_OUTPUTFILE "vcap.jpg"
#define VCAP_DEFAULT_WIDTH 640
#define VCAP_DEFAULT_HEIGHT 480
#define VCAP_DEFAULT_JPEG_QUALITY 99

namespace upm {
    /**
     * @brief Take a snapshot from a video camera and save as a JPEG
     * @defgroup vcap libupm-vcap
     * @ingroup video
     */

    /**
     * @library vcap
     * @sensor vcap
     * @comname Video Capture
     * @type video
     *
     * @brief API for the Video Capture driver
     *
     * This UPM module captures a still frame from a Linux V4L device,
     * such as a USB webcam, and and then allows you to save it as a
     * JPEG image into a file.
     *
     * The camera and driver in use must support streaming, mmap-able
     * buffers and must provide data in YUYV format.  This should
     * encompass most video cameras out there.  It has been tested
     * with a few off the shelf cameras without any problems.
     *
     * @snippet vcap.cxx Interesting
     */

  class VCAP {
  public:

    /**
     * VCAP object constructor
     *
     * @param videoDev The path to the video device, default is /dev/video0.
     */
    VCAP(std::string videoDev=VCAP_DEFAULT_VIDEODEV);

    /**
     * VCAP object destructor
     */
    ~VCAP();

    /**
     * Set the desired resolution of the output image.  Note, this is
     * a hint to the underlying video driver.  The video driver is
     * free to lower the specified resolution if the hardware cannot
     * support it.  You can use getHeight() and getWidth() after
     * calling this method to see what the video driver chose.
     *
     * @param width The desired width of the image.
     * @param width The desired height of the image.
     * @return true if the operation succeeded, false otherwise.
     */
    bool setResolution(int width, int height);

    /**
     * Capture an image from the camera.
     *
     * @return true if the operation succeeded, false otherwise.
     */
    bool captureImage();

    /**
     * Save the captured image (created with captureImage()) to a file
     * in JPEG format.  The file will be overwritten if it already
     * exists.
     *
     * @param filename The name of the file in which to store the image.
     * @return true if the operation succeeded, false otherwise.
     */
    bool saveImage(std::string filename=VCAP_DEFAULT_OUTPUTFILE);

    /**
     * Return the current width of the image.  You can use this method
     * to determine if the video driver downgraded it after a call to
     * setResolution().
     *
     * @return true Current width of capture.
     */
    int getWidth() const
    {
      return m_width;
    };

    /**
     * Return the current height of the image.  You can use this method
     * to determine if the video driver downgraded it after a call to
     * setResolution().
     *
     * @return true Current height of capture.
     */
    int getHeight() const
    {
      return m_height;
    };

    /**
     * Set the JPEG quality.
     *
     * @param quality A number between 0-100, with higher numbers
     * meaning higher quality. Numbers less than 0 will be clamped to
     * 0, numbers higher than 100 will be clamped to 100.
     */
    void setJPGQuality(unsigned int quality);

    /**
     * Get the current JPEG quality setting.
     *
     * @return the current JPEG quality setting.
     */
    int getJPGQuality() const
    {
      return m_jpgQuality;
    };

    /**
     * Enable or disable debugging output.
     *
     * @param enable true to enable debugging, false otherwise
     */
    void setDebug(bool enable)
    {
      m_debugging = enable;
    };
    
  protected:
    // open the device and check that it meats minimum requirements
    bool initVideoDevice();

    // make sure device is streamable, supports mmap and capture
    bool checkCapabilities();

    // read the mmapped buffer in YUYV format and create a jpeg image
    bool YUYV2JPEG(FILE *file);

    // buffer management
    bool allocBuffer();
    void releaseBuffer();

    // does the actual capture
    bool doCaptureImage();
    
  private:
    // internal ioctl
    int xioctl(int fd, int request, void* argp);
               
    std::string m_videoDevice;
    
    // our file descriptor to the video device
    int m_fd;

    // v4l info
    struct v4l2_capability m_caps;
    struct v4l2_format m_format;

    // our mmaped buffer
    unsigned char *m_buffer;
    size_t m_bufferLen;

    // the resolution and quality
    int m_width;
    int m_height;
    int m_jpgQuality;

    // at least one image captured with current settings?
    bool m_imageCaptured;

    // are we debugging?
    bool m_debugging;
  };
}


