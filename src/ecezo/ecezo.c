/*
 * Author: Jon Trulson <jtrulson@ics.com>
 * Copyright (c) 2016 Intel Corporation.
 *
 * This program and the accompanying materials are made available under the
 * terms of the The MIT License which is available at
 * https://opensource.org/licenses/MIT.
 *
 * SPDX-License-Identifier: MIT
 */

#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <ctype.h>

#include <upm_utilities.h>

#include "ecezo.h"

// "Typical" command completion delay in ms
#define CMD_DELAY (350)

// uncomment for dubugging
//#define ECEZO_DEBUG (1)

// I2C read helper
static int readBytes(const ecezo_context dev, uint8_t *buffer, int len)
{
    assert(dev != NULL);
    assert(dev->i2c != NULL);

    bool done = false;
    int rv;
    int retries = 10;

    while (!done && (retries-- > 0))
    {
        if ((rv = mraa_i2c_read(dev->i2c, buffer, len)) < 0)
        {
            printf("%s: mraa_i2c_read(code) failed.\n", __FUNCTION__);
            return rv;
        }

#if defined(ECEZO_DEBUG)
        printf("CODE: %02x\n", buffer[0]);
#endif

        if (buffer[0] == 0xff || buffer[0] == 0x02)
        {
            // no data available, or error
            return -1;
        }
        else if (buffer[0] == 0x01)
        {
            // data is ready
            done = true;

            // now we need to move the data one byte down so the rest
            // of this driver can work as-is.
            memmove(buffer, (buffer + 1), len - 1);
        }
        else
        {
            // buffer[0] 0xfe - data is pending. wait and loop again.
            upm_delay_ms(CMD_DELAY);
        }
    }

    if (retries <= 0)
    {
        printf("%s: timed out waiting for correct response.\n", __FUNCTION__);
        return -1;
    }

#if defined(ECEZO_DEBUG)
    printf("%s: Got %d bytes\n", __FUNCTION__, rv);

    for (int i=0; i<rv; i++)
    {
        printf("%02x (%c) ", buffer[i],
               isprint(buffer[i]) ? buffer[i] : '@');
    }
    printf("\n");
#endif // ECEZO_DEBUG

    return rv;
}

static upm_result_t generic_init(const ecezo_context dev)
{
    assert(dev != NULL);

    // do some generic initialization
    bool error = false;

    // turn off response (*OK) codes (UART only)
    if (dev->uart)
    {
        if (ecezo_send_command(dev, "Response,0", NULL, 0) < 0)
            error = true;
    }

    // turn off continuous sampling
    if (ecezo_set_continuous(dev, false))
        error = true;

    // make sure all parameters are enabled
    if (ecezo_send_command(dev, "O,EC,1", NULL, 0) < 0)
        error = true;
    if (ecezo_send_command(dev, "O,TDS,1", NULL, 0) < 0)
        error = true;
    if (ecezo_send_command(dev, "O,S,1", NULL, 0) < 0)
        error = true;
    if (ecezo_send_command(dev, "O,SG,1", NULL, 0) < 0)
        error = true;

    if (error)
        return UPM_ERROR_OPERATION_FAILED;
    else
        return UPM_SUCCESS;
}

static upm_result_t decode_report(const ecezo_context dev, char *data)
{
    assert(dev != NULL);

    char *startptr = data;
    char *endptr = NULL;
    float val;

    // the format of the data string should be: ec,tds,s,sg

    // ec
    val = strtof(startptr, &endptr);

    if (startptr == endptr)
    {
        // error
        return UPM_ERROR_OPERATION_FAILED;
    }

    dev->ec = val;
    startptr = endptr + 1;

    // tds
    val = strtof(startptr, &endptr);

    // error
    if (startptr == endptr)
        return UPM_ERROR_OPERATION_FAILED;

    dev->tds = val;
    startptr = endptr + 1;

    // salinity
    val = strtof(startptr, &endptr);

    // error
    if (startptr == endptr)
        return UPM_ERROR_OPERATION_FAILED;

    dev->salinity = val;
    startptr = endptr + 1;

    // sg
    val = strtof(startptr, &endptr);

    if (startptr == endptr)
        return UPM_ERROR_OPERATION_FAILED;

    dev->sg = val;

    return UPM_SUCCESS;
}

static bool ecezo_data_available(const ecezo_context dev, unsigned int millis)
{
    assert(dev != NULL);

    // i2c, we don't support this
    if (dev->i2c)
    {
        return false;
    }

    // uart
    if (mraa_uart_data_available(dev->uart, millis))
        return true;
    else
        return false;
}

// uart init
ecezo_context ecezo_uart_init(unsigned int uart, unsigned int baudrate)
{
    // make sure MRAA is initialized
    int mraa_rv;
    if ((mraa_rv = mraa_init()) != MRAA_SUCCESS)
    {
        printf("%s: mraa_init() failed (%d).\n", __FUNCTION__, mraa_rv);
        return NULL;
    }

    ecezo_context dev =
        (ecezo_context)malloc(sizeof(struct _ecezo_context));

    if (!dev)
        return NULL;

    // zero out context
    memset((void *)dev, 0, sizeof(struct _ecezo_context));

    // initialize the MRAA contexts

    // uart, default should be 8N1
    if (!(dev->uart = mraa_uart_init(uart)))
    {
        printf("%s: mraa_uart_init() failed.\n", __FUNCTION__);
        ecezo_close(dev);
        return NULL;
    }

    if (mraa_uart_set_baudrate(dev->uart, baudrate))
    {
        printf("%s: mraa_uart_set_baudrate() failed.\n", __FUNCTION__);
        ecezo_close(dev);
        return NULL;
    }

    mraa_uart_set_flowcontrol(dev->uart, false, false);

    if (generic_init(dev))
    {
        printf("%s: generic_init() failed.\n", __FUNCTION__);
        ecezo_close(dev);
        return NULL;
    }

    return dev;
}

// i2c ublox init
ecezo_context ecezo_i2c_init(unsigned int bus, uint8_t addr)
{
    // make sure MRAA is initialized
    int mraa_rv;
    if ((mraa_rv = mraa_init()) != MRAA_SUCCESS)
    {
        printf("%s: mraa_init() failed (%d).\n", __FUNCTION__, mraa_rv);
        return NULL;
    }

    ecezo_context dev =
        (ecezo_context)malloc(sizeof(struct _ecezo_context));

    if (!dev)
        return NULL;

    // zero out context
    memset((void *)dev, 0, sizeof(struct _ecezo_context));

    // initialize the MRAA contexts

    if (!(dev->i2c = mraa_i2c_init(bus)))
    {
        printf("%s: mraa_i2c_init() failed.\n", __FUNCTION__);
        ecezo_close(dev);
        return NULL;
    }

    if (mraa_i2c_address(dev->i2c, addr))
    {
        printf("%s: mraa_i2c_address() failed.\n", __FUNCTION__);
        ecezo_close(dev);
        return NULL;
    }

    if (generic_init(dev))
    {
        printf("%s: generic_init() failed.\n", __FUNCTION__);
        ecezo_close(dev);
        return NULL;
    }

    return dev;
}

void ecezo_close(ecezo_context dev)
{
    assert(dev != NULL);

    if (dev->uart)
        mraa_uart_stop(dev->uart);
    if (dev->i2c)
        mraa_i2c_stop(dev->i2c);

    free(dev);
}

upm_result_t ecezo_set_continuous(const ecezo_context dev, bool enable)
{
    int rv;

    if (enable)
        rv = ecezo_send_command(dev, "C,1", NULL, 0);
    else
        rv = ecezo_send_command(dev, "C,0", NULL, 0);

    return ((rv < 0) ? UPM_ERROR_OPERATION_FAILED : UPM_SUCCESS);
}

upm_result_t ecezo_set_temperature(const ecezo_context dev, float temp)
{
    char buffer[ECEZO_MAX_BUFFER_LEN];

    snprintf(buffer, ECEZO_MAX_BUFFER_LEN, "T,%f", temp);

    int rv = ecezo_send_command(dev, buffer, NULL, 0);

    return ((rv < 0) ? UPM_ERROR_OPERATION_FAILED : UPM_SUCCESS);
}

upm_result_t ecezo_set_led(const ecezo_context dev, bool enable)
{
    int rv;

    if (enable)
        rv = ecezo_send_command(dev, "L,1", NULL, 0);
    else
        rv = ecezo_send_command(dev, "L,0", NULL, 0);

    return ((rv < 0) ? UPM_ERROR_OPERATION_FAILED : UPM_SUCCESS);
}

upm_result_t ecezo_set_k_value(const ecezo_context dev, float k)
{
    char buffer[ECEZO_MAX_BUFFER_LEN];

    // the K value must be between 0.1 and 10.0
    if (k < 0.1 || k > 10.0)
    {
        printf("%s: K value must be between 0.1 and 10.0\n", __FUNCTION__);
        return UPM_ERROR_OUT_OF_RANGE;
    }

    snprintf(buffer, ECEZO_MAX_BUFFER_LEN, "K,%f", k);

    int rv = ecezo_send_command(dev, buffer, NULL, 0);

    return ((rv < 0) ? UPM_ERROR_OPERATION_FAILED : UPM_SUCCESS);
}

upm_result_t ecezo_set_sleep(const ecezo_context dev, bool enable)
{
    int rv = 0;

    if (enable)
        rv =  ecezo_send_command(dev, "SLEEP", NULL, 0);
    else
    {
        // "WAKE" isn't a real command, but should wake the device up.
        // We ignore the return value, as it will likely be an error
        // anyway.
        ecezo_send_command(dev, "WAKE", NULL, 0);
    }

    return ((rv < 0) ? UPM_ERROR_OPERATION_FAILED : UPM_SUCCESS);
}

int ecezo_read(const ecezo_context dev, char *buffer, size_t len)
{
    assert(dev != NULL);

    upm_delay_ms(CMD_DELAY); // delay CMD_DELAY ms to make sure cmd completed

    // i2c
    if (dev->i2c)
    {
        return readBytes(dev, (uint8_t *)buffer, len);
    }
    else
    {
        // UART
        size_t bytesRead = 0;

        while(bytesRead < len)
        {
            // we read one byte at a time, exiting when either len is
            // reached, or a '\r' is found indicating the end of a
            // sentence. Most commands (except 'R') require a minimum
            // of 300ms to execute, so we wait up to CMD_DELAY ms after all
            // data (if any) is read.
            if (ecezo_data_available(dev, CMD_DELAY))
            {
                int br = mraa_uart_read(dev->uart, &buffer[bytesRead], 1);

                if (br <= 0)
                    return br;

                if (buffer[bytesRead] == '\r')
                {
                    // if we found a CR, replace it with a 0 byte
                    buffer[bytesRead++] = 0;
                    return bytesRead;
                }
                bytesRead++;
            }
            else
            {
                // timed out - ok with responses disabled
                return 0;
            }
        }
    }

    // anything else is an error
    return -1;
}

upm_result_t ecezo_write(const ecezo_context dev, char *buffer, size_t len)
{
    assert(dev != NULL);

    if (dev->uart)
    {
        if (mraa_uart_write(dev->uart, buffer, len) != (int)len)
        {
            printf("%s: mraa_uart_write() failed.\n", __FUNCTION__);
            return UPM_ERROR_OPERATION_FAILED;
        }

    }
    else
    {
        // I2C
        if (mraa_i2c_write(dev->i2c, (uint8_t *)buffer, len))
        {
            printf("%s: mraa_i2c_write() failed.\n", __FUNCTION__);
            return UPM_ERROR_OPERATION_FAILED;
        }
    }

    return UPM_SUCCESS;
}

int ecezo_send_command(const ecezo_context dev, char *cmd, char *buffer,
                       int len)
{
    assert(dev != NULL);

    if (!cmd)
        return -1;

    // Our local buffer in case one isn't supplied
    char localBuffer[ECEZO_MAX_BUFFER_LEN];

    // our read buffer ptr
    char *readBuffer = NULL;

    if (!buffer || !len)
    {
        readBuffer = localBuffer;
        len = ECEZO_MAX_BUFFER_LEN;
    }
    else
    {
        readBuffer = buffer;
    }

#if defined(ECEZO_DEBUG)
    printf("Command: %s\n", cmd);
#endif // ECEZO_DEBUG

    // our write buffer
    char writeBuffer[ECEZO_MAX_BUFFER_LEN];

    memcpy(writeBuffer, cmd, ECEZO_MAX_BUFFER_LEN - 1);
    writeBuffer[ECEZO_MAX_BUFFER_LEN - 1] = 0;

    int writelen = strlen(writeBuffer);

    if (dev->uart)
    {
        if (strlen(writeBuffer) >= ECEZO_MAX_BUFFER_LEN - 2)
        {
            // too big.  Should never happen in real life.
            printf("%s: cmd writeBuffer too big.\n", __FUNCTION__);
            return -1;
        }

        strncat(writeBuffer, "\r", 2);
    }

    // for the uart this will now include the added CR, for I2C, this
    // will now include the already existing \0 terminator.
    writelen++;

    // Let the games begin...
    int retries = 10;

    while (retries-- > 0)
    {
        if (ecezo_write(dev, writeBuffer, writelen))
        {
            printf("%s: ecezo_write() failed\n", __FUNCTION__);
            return -1;
        }

        // we wait up to CMD_DELAY ms for an error response, which should be
        // more than enough time.  No response is also ok, since we
        // disable the "*OK" response in the init.

        memset((void *)readBuffer, 0, len);
        int bytesRead = 0;
        if ((bytesRead = ecezo_read(dev, readBuffer, len)) < 0)
        {
            return -1;
        }

        // for I2C, we are done at this point
        if (dev->i2c)
            break;

        // for UART, we need some more checks
        if (bytesRead && strstr(readBuffer, "*ER"))
        {
            // need to retry the command
#if defined(ECEZO_DEBUG)
            printf("%s: *ER DETECTED, retry\n", __FUNCTION__);
#endif // ECEZO_DEBUG
            continue;
        }
        else if (bytesRead && strchr(readBuffer, '*'))
        {
            // Some other diagnostic code, output it.
#if defined(ECEZO_DEBUG)
            printf("%s: * diagnostic code detected (%s), retry\n",
                   __FUNCTION__, buffer);
#endif // ECEZO_DEBUG
            continue;
        }
        else
        {
            // we are done here
#if defined(ECEZO_DEBUG)
            printf("%s: bytesRead = %d\n", __FUNCTION__, bytesRead);
#endif // ECEZO_DEBUG

            break;
        }
    }

    if (retries <= 0)
    {
        printf("%s: read timed out and/or and retries exhausted\n",
               __FUNCTION__);
        return 0;
    }

    return len;
}

upm_result_t ecezo_update(const ecezo_context dev)
{
    assert(dev != NULL);

    // first we send a 'R' command to get a reading (takes a minimum
    // of 1 second), then we parse out the string values into the
    // context variables.

    char buffer[ECEZO_MAX_BUFFER_LEN];

    // first issue the report command
    int rv = ecezo_send_command(dev, "R", buffer, ECEZO_MAX_BUFFER_LEN);
    if (rv == 0)
    {
        printf("%s: timed out waiting for data\n", __FUNCTION__);
        return UPM_ERROR_OPERATION_FAILED;
    }
    else if (rv < 0)
    {
        printf("%s: error retrieving data\n", __FUNCTION__);
        return UPM_ERROR_OPERATION_FAILED;
    }

    // decode
    if (decode_report(dev, buffer))
    {
        printf("%s: decode_report() failed\n", __FUNCTION__);
        return UPM_ERROR_OPERATION_FAILED;
    }

    return UPM_SUCCESS;
}

float ecezo_get_ec(const ecezo_context dev)
{
    assert(dev != NULL);

    return dev->ec;
}

float ecezo_get_tds(const ecezo_context dev)
{
    assert(dev != NULL);

    return dev->tds;
}

float ecezo_get_salinity(const ecezo_context dev)
{
    assert(dev != NULL);

    return dev->salinity;
}

float ecezo_get_sg(const ecezo_context dev)
{
    assert(dev != NULL);

    return dev->sg;
}

upm_result_t ecezo_calibrate(const ecezo_context dev, ECEZO_CALIBRATION_T cal,
                             float ec)
{
    assert(dev != NULL);

    char cmdBuffer[ECEZO_MAX_BUFFER_LEN];

    switch(cal)
    {
    case ECEZO_CALIBRATE_CLEAR:
        snprintf(cmdBuffer, ECEZO_MAX_BUFFER_LEN, "cal,clear");
        break;

    case ECEZO_CALIBRATE_DRY:
        snprintf(cmdBuffer, ECEZO_MAX_BUFFER_LEN, "cal,dry");
        break;

    case ECEZO_CALIBRATE_ONE:
        snprintf(cmdBuffer, ECEZO_MAX_BUFFER_LEN, "cal,one,%f", ec);
        break;

    case ECEZO_CALIBRATE_LOW:
        snprintf(cmdBuffer, ECEZO_MAX_BUFFER_LEN, "cal,low,%f", ec);
        break;

    case ECEZO_CALIBRATE_HIGH:
        snprintf(cmdBuffer, ECEZO_MAX_BUFFER_LEN, "cal,high,%f", ec);
        break;

    default:
        // should be able to happen
        printf("%s: invalid cal parameter\n", __FUNCTION__);
        return UPM_ERROR_INVALID_PARAMETER;
    }

    return ecezo_send_command(dev, cmdBuffer, NULL, 0);
}
