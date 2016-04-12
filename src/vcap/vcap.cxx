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
#include <stdexcept>
#include <unistd.h>
#include <stdio.h>
#include <math.h>
#include <string.h>

#include "vcap.hpp"

using namespace upm;
using namespace std;

#define CLAMP(_val, _min, _max) \
  (((_val) < (_min)) ? (_min) : (((_val) > (_max)) ? (_max) : (_val)))

VCAP::VCAP(string videoDev) :
  m_buffer(0), m_fd(-1)
{
  memset(&m_caps, 0, sizeof(struct v4l2_capability));
  memset(&m_format, 0, sizeof(struct v4l2_format));
  
  m_debugging = false;
  m_bufferLen = 0;
  m_videoDevice = videoDev;
  setJPGQuality(VCAP_DEFAULT_JPEG_QUALITY);

  // try to open the video device, and set a default format.
  if (!initVideoDevice())
    throw std::runtime_error(std::string(__FUNCTION__) +
                             ": initVideoDevice() failed");

  m_height = 0;
  m_width = 0;
  m_imageCaptured = false;
}

VCAP::~VCAP()
{
  releaseBuffer();

  if (m_fd >= 0)
    close(m_fd);

  m_fd = -1;
}

bool VCAP::initVideoDevice()
{
  if (m_videoDevice.empty())
    return false;

  if ((m_fd = open(m_videoDevice.c_str(), O_RDWR)) < 0)
    {
      cerr << __FUNCTION__ << ": open failed: " << strerror(errno) << endl;
      return false;
    }
  
  if (!checkCapabilities())
    {
      close(m_fd);
      m_fd = -1;
      return false;
    }

  return true;
}

// This seems... odd, but appears to be necessary.
// Ignore error and retry if the ioctl fails due to EINTR
int VCAP::xioctl(int fd, int request, void* argp)
{
  int r;
  
  do {
    r = ioctl(fd, request, argp);
  }
  while (r == -1 && errno == EINTR);
  
  return r;
}

bool VCAP::checkCapabilities()
{
  if (xioctl(m_fd, VIDIOC_QUERYCAP, &m_caps) < 0)
    {
      cerr << __FUNCTION__ << ": ioctl(VIDIOC_QUERYCAP) failed: "
           << strerror(errno) << endl;
      return false;
    }
  
  if (m_debugging)
    {
      cerr << "Driver: " << m_caps.driver << endl;
      cerr << "Device: "  << m_caps.card << endl;
      cerr << "Caps  : 0x" << std::hex << m_caps.capabilities << std::dec
           << endl;
    }

  // see if capturing is supported
  if (!(m_caps.capabilities & V4L2_CAP_VIDEO_CAPTURE))
    {
      cerr << __FUNCTION__ << ": Device does not support video capture"
           << endl;
      return false;
    }

  if (!(m_caps.capabilities & V4L2_CAP_STREAMING))
    {
      cerr << __FUNCTION__ << ": Device does not support streaming I/O"
           << endl;
      return false;
    }

  return true;
}

bool VCAP::setResolution(int width, int height)
{
  // in case we already created one
  releaseBuffer();

  m_width = width;
  m_height = height;

  m_format.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
  // initialize with the current format
  if (xioctl(m_fd, VIDIOC_G_FMT, &m_format) < 0)
    {
      cerr << __FUNCTION__ << ": ioctl(VIDIOC_G_FMT) failed: "
           << strerror(errno) << endl;
      return false;
    }

  // make our changes...
  m_format.fmt.pix.width = m_width;
  m_format.fmt.pix.height = m_height;
  m_format.fmt.pix.pixelformat = V4L2_PIX_FMT_YUYV;
  m_format.fmt.pix.field = V4L2_FIELD_ANY;
        
  if (xioctl(m_fd, VIDIOC_S_FMT, &m_format) < 0)
    {
      cerr << __FUNCTION__ << ": ioctl(VIDIOC_S_FMT) failed: "
           << strerror(errno) << endl;

      // If it's just busy, then this still might work, so don't fail here
      if (errno != EBUSY)
        return false;
    }

  // Now retrieve the driver's selected format and check it -
  // specifically, the width and height might change, causing
  // coredumps if we don't adjust them accordingly.

  if (xioctl(m_fd, VIDIOC_G_FMT, &m_format) < 0)
    {
      cerr << __FUNCTION__ << ": ioctl(VIDIOC_G_FMT) failed: "
           << strerror(errno) << endl;
      return false;
    }

  // G_FMT will have adjusted these if neccessary, so verify
  if (m_format.fmt.pix.width != m_width)
    {
      if (m_debugging)
        cerr << __FUNCTION__ << ": Warning: Selected width "
             << std::to_string(m_width)
             << " adjusted by driver to "
             << std::to_string(m_format.fmt.pix.width)
             << endl;

      m_width = m_format.fmt.pix.width;
    }
  
  if (m_format.fmt.pix.height != m_height)
    {
      if (m_debugging)
        cerr << __FUNCTION__ << ": Warning: Selected height "
             << std::to_string(m_height)
             << " adjusted by driver to "
             << std::to_string(m_format.fmt.pix.height)
             << endl;

      m_height = m_format.fmt.pix.height;
    }

  // now alloc the buffers here
  if (!allocBuffer())
    return false;

  return true;
}
 
bool VCAP::allocBuffer()
{
  struct v4l2_requestbuffers rb;
  memset(&rb, 0, sizeof(rb));

  // we just want one buffer, and we only support mmap().
  rb.count = 1;
  rb.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
  rb.memory = V4L2_MEMORY_MMAP;
 
  if (xioctl(m_fd, VIDIOC_REQBUFS, &rb) < 0)
    {
      if (errno == EINVAL)
        {
          cerr << __FUNCTION__ << ": Capture device does not support mmapped "
               << "buffers"
               << endl;
        }
      cerr << __FUNCTION__ << ": ioctl(VIDIOC_REQBUFS) failed: "
           << strerror(errno) << endl;
      
      return false;
    }
 
  // get the buffer and mmap it
  struct v4l2_buffer mbuf;
  memset(&mbuf, 0, sizeof(mbuf));

  mbuf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
  mbuf.memory = V4L2_MEMORY_MMAP;
  mbuf.index = 0;

  if (xioctl(m_fd, VIDIOC_QUERYBUF, &mbuf) < 0)
    {
      cerr << __FUNCTION__ << ": ioctl(VIDIOC_QUERYBUF) failed: "
           << strerror(errno) << endl;
      return false;
    }
 
  // map it
  m_buffer = (unsigned char *)mmap(NULL, mbuf.length, 
                                   PROT_READ | PROT_WRITE, MAP_SHARED, 
                                   m_fd, mbuf.m.offset);

  if (m_buffer == MAP_FAILED)
    {
      cerr << __FUNCTION__ << ": mmap() failed: "
           << strerror(errno) << endl;
      return false;
    }

  // we'll need this when unmapping
  m_bufferLen = mbuf.length;
  
  return true;
}

void VCAP::releaseBuffer()
{
  // first unmap any buffers
  if (m_buffer)
    munmap(m_buffer, m_bufferLen);

  m_buffer = 0;
  m_bufferLen = 0;

  // then, tell the kernel driver to free any allocated buffer(s)...
  struct v4l2_requestbuffers rb;
  memset(&rb, 0, sizeof(rb));

  rb.count = 0;
  rb.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
  rb.memory = V4L2_MEMORY_MMAP;
 
  if (xioctl(m_fd, VIDIOC_REQBUFS, &rb) < 0)
    {
      cerr << __FUNCTION__ << ": ioctl(VIDIOC_REQBUFS) failed while freeing: "
           << strerror(errno) << endl;
    }

  // reset captured flag
  m_imageCaptured = false;
}


bool VCAP::YUYV2JPEG(FILE *file)
{
  struct jpeg_compress_struct jpgInfo;
  struct jpeg_error_mgr jerr;
  JSAMPROW row_pointer[1];
  unsigned char *row_buffer = NULL;
  unsigned char *yuyv = NULL;
  int z;

  row_buffer = (unsigned char *)calloc(m_width * 3, 1);
  if (!row_buffer)
    {
      cerr << __FUNCTION__ << ": allocation of line buffer failed."
           << endl;
      return false;
    }

  yuyv = m_buffer;

  jpgInfo.err = jpeg_std_error(&jerr);
  jpeg_create_compress(&jpgInfo);
  jpeg_stdio_dest(&jpgInfo, file);

  jpgInfo.image_width = m_width;
  jpgInfo.image_height = m_height;

  // components R, G, B
  jpgInfo.input_components = 3;
  jpgInfo.in_color_space = JCS_RGB;

  jpeg_set_defaults(&jpgInfo);
  jpeg_set_quality(&jpgInfo, m_jpgQuality, TRUE);

  jpeg_start_compress(&jpgInfo, TRUE);

  z = 0;

  while (jpgInfo.next_scanline < jpgInfo.image_height)
    {
      int x;
      unsigned char *ptr = row_buffer;
      
      for (x = 0; x < m_width; x++)
        {
          int r, g, b;
          int y, u, v;
          
          if (!z)
            y = yuyv[0] << 8;
          else
            y = yuyv[2] << 8;
          u = yuyv[1] - 128;
          v = yuyv[3] - 128;
          
          r = (y + (359 * v)) >> 8;
          g = (y - (88 * u) - (183 * v)) >> 8;
          b = (y + (454 * u)) >> 8;
          
          *(ptr++) = CLAMP(r, 0, 255);
          *(ptr++) = CLAMP(g, 0, 255);
          *(ptr++) = CLAMP(b, 0, 255);
          
          if (z++)
            {
              z = 0;
              yuyv += 4;
            }
        }
      
      row_pointer[0] = row_buffer;
      jpeg_write_scanlines(&jpgInfo, row_pointer, 1);
    }

  jpeg_finish_compress(&jpgInfo);
  jpeg_destroy_compress(&jpgInfo);

  free(row_buffer);

  return true;
}

bool VCAP::saveImage(string filename)
{
  // check m_buffer to make sure we have an actual buffer... If not,
  // we throw here.
  if (!m_buffer)
    {
      throw std::runtime_error(std::string(__FUNCTION__) +
                               ": no buffer.  Call setResolution() first");
    }

  // if we haven't done at least one capture yet...
  if (!m_imageCaptured)
    {
      throw std::runtime_error(std::string(__FUNCTION__) +
                               ": No data, call captureImage() first");
    }

  FILE *file;
  if ((file = fopen(filename.c_str(), "wb")) == NULL)
    {
      cerr << __FUNCTION__ << ": fopen() failed: "
           << strerror(errno) << endl;
      return false;
    }
  
  YUYV2JPEG(file);
  fclose(file);

  if (m_debugging)
    cerr << __FUNCTION__ << ": Saved image to " << filename << endl;

  return true;
}

bool VCAP::captureImage()
{
  // first, make sure a resolution was specified.  If not, set the
  // default
  if (m_width == 0 || m_height == 0)
    {
      if (!setResolution(VCAP_DEFAULT_WIDTH, VCAP_DEFAULT_HEIGHT))
        throw std::runtime_error(std::string(__FUNCTION__) +
                                 ": setResolution() failed");
    }

  // we basically just call doCaptureImage() twice - once to grab and
  // discard the first frame (which is usually a remnent of a previous
  // capture), and another to grab the real frame we are interesed in.

  if (!doCaptureImage())
    {
      cerr << __FUNCTION__ << ": capture of first frame failed"
           << endl;
    }

  return doCaptureImage();
}


// the real workhorse
bool VCAP::doCaptureImage()
{
  struct v4l2_buffer buf = {0};
  buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
  buf.memory = V4L2_MEMORY_MMAP;
  buf.index = 0;

  // queue our buffer
  if (xioctl(m_fd, VIDIOC_QBUF, &buf) < 0)
    {
      cerr << __FUNCTION__ << ": ioctl(VIDIOC_QBUF) failed: "
           << strerror(errno) << endl;

      return false;
    }
  
  // enable streaming
  if (xioctl(m_fd, VIDIOC_STREAMON, &buf.type) < 0)
    {
      cerr << __FUNCTION__ << ": ioctl(VIDIOC_STREAMON) failed: "
           << strerror(errno) << endl;

      return false;
    }

  // use select to wait for a complete frame.
  fd_set fds;

  FD_ZERO(&fds);
  FD_SET(m_fd, &fds);

  struct timeval tv;
  memset(&tv, 0, sizeof(tv));

  // 5 seconds should be more than enough
  tv.tv_sec = 5;

  int rv;
  if ((rv = select(m_fd + 1, &fds, NULL, NULL, &tv)) < 0)
    {
      cerr << __FUNCTION__ << ": select() failed: "
           << strerror(errno) << endl;
      return false;
    }

  if (!rv)
    {
      // timed out
      cerr << __FUNCTION__ << ": select() timed out waiting for frame"
           << endl;

      return false;
    }

  // de-queue the buffer, we're now free to access it via the mmapped
  // ptr (m_buffer)
  if (xioctl(m_fd, VIDIOC_DQBUF, &buf) < 0)
    {
      cerr << __FUNCTION__ << ": ioctl(VIDIOC_DQBUF) failed: "
           << strerror(errno) << endl;

      return false;
    }

  // turn off streaming
  if (xioctl(m_fd, VIDIOC_STREAMOFF, &buf.type) < 0)
    {
      cerr << __FUNCTION__ << ": ioctl(VIDIOC_STREAMOFF) failed: "
           << strerror(errno) << endl;

      return false;
    }
    
  m_imageCaptured = true;

  return true;
}

 void VCAP::setJPGQuality(unsigned int qual)
 {
   m_jpgQuality = CLAMP(qual, 0, 100);
 }
