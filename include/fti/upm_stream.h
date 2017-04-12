/* 
 * Authors: Jon Trulson <jtrulson@ics.com>
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

#ifndef UPM_STREAM_H_
#define UPM_STREAM_H_

#ifdef __cplusplus
extern "C" {
#endif

typedef struct _upm_stream_ft {
        /* read up to len bytes into buffer, return number of bytes read */
        int (*upm_stream_read) (void* dev, char *buffer, int len);
        /* write up to len bytes from buffer, return number of bytes written */
        int (*upm_stream_write) (void* dev, char *buffer, int len);
        /* return true if data is available to be read, false otherwise */
        bool (*upm_stream_data_available) (void* dev, unsigned int timeout);
} upm_stream_ft;

#ifdef __cplusplus
}
#endif

#endif /* UPM_STREAM_H_ */
