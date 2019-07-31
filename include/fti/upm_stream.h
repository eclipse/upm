/* 
 * Authors: Jon Trulson <jtrulson@ics.com>
 * Copyright (c) 2016 Intel Corporation.
 *
 * This program and the accompanying materials are made available under the
 * terms of the The MIT License which is available at
 * https://opensource.org/licenses/MIT.
 *
 * SPDX-License-Identifier: MIT
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
