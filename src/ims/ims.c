/*
 * Author: Noel Eck <noel.eck@intel.com>
 * Copyright (c) 2015 Intel Corporation.
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

#include <stdlib.h>
#include <syslog.h>

#include "ims.h"

ims_context* ims_init(int16_t i2c_bus, int16_t i2c_address)
{
    /* Allocate space for the sensor structure */
    ims_context* dev = (ims_context*) malloc(sizeof(ims_context));
    if(dev == NULL)
    {
        syslog(LOG_CRIT, "%s: malloc() failed\n", __FUNCTION__);
        goto ims_init_fail;
    }

    /* Initialize mraa */
    mraa_result_t result = mraa_init();
    if (result != MRAA_SUCCESS)
    {
        syslog(LOG_ERR, "%s: mraa_init() failed (%d)\n", __FUNCTION__, result);
        goto ims_init_fail;
    }

    /* Init i2c */
    dev->_i2c_context = mraa_i2c_init(i2c_bus);
    if(dev->_i2c_context == NULL)
    {
        syslog(LOG_ERR, "%s: mraa_i2c_init() failed\n", __FUNCTION__);
        goto ims_init_fail;
    }

    /* Set the i2c slave address for this device */
    if (mraa_i2c_address(dev->_i2c_context, i2c_address) != MRAA_SUCCESS)
    {
        syslog(LOG_ERR, "%s: mraa_i2c_address() failed\n", __FUNCTION__);
        goto ims_init_fail;
    }

    /* This device must run at 100kHz */
    if (mraa_i2c_frequency(dev->_i2c_context, MRAA_I2C_STD))
    {
        syslog(LOG_ERR, "%s: mraa_i2c_frequency() failed.  %s\n",
                __FUNCTION__,
                "This device requires I2C standard mode (100 kb/s)");
        goto ims_init_fail;
    }

    return dev;

/* Handle all failing cases here */
ims_init_fail:
    /* Free structure memory if allocated */
    if (dev != NULL)
        free(dev);
    return NULL;
}

void ims_close(ims_context* dev)
{
    if (dev == NULL) return;

    /* Cleanup the I2C context */
    mraa_i2c_stop(dev->_i2c_context);
    free(dev);
}

upm_result_t ims_read(const ims_context* dev, IMS_RD_COMMAND cmd, uint16_t* rd_data)
{
    int32_t val = 0;
    uint8_t readbuf[2] = {0, 0};
    switch (cmd)
    {
        /* One byte reads */
        case IMS_GET_ADDRESS:
        case IMS_GET_VERSION:
        case IMS_GET_BUSY:
            val = mraa_i2c_read_bytes_data(dev->_i2c_context, cmd, readbuf, 1);
            *rd_data = readbuf[0];
            break;
        /* Two byte reads */
        case IMS_GET_CAPACITANCE:
        case IMS_GET_LIGHT:
        case IMS_GET_TEMPERATURE:
            val = mraa_i2c_read_bytes_data(dev->_i2c_context, cmd, readbuf, 2);
            *rd_data = readbuf[1];
            *rd_data += ((uint16_t)readbuf[0] << 8) & 0xFF00;
            break;
        default:
            syslog(LOG_ERR, "%s: Invalid read command: 0x%02x\n", __FUNCTION__, cmd);
            return UPM_ERROR_INVALID_PARAMETER;
    }

    /* val will be < 0 on failing case */
    if (val < 0)
    {
        syslog(LOG_ERR, "%s: mraa_i2c_read_byte/word_data() failed\n", __FUNCTION__);
        return UPM_ERROR_NO_DATA;
    }

    return UPM_SUCCESS;
}

upm_result_t ims_write(const ims_context* dev, IMS_WR_COMMAND cmd, uint8_t wr_data)
{
    mraa_result_t mr;
    switch (cmd)
    {
        /* One byte write */
        case IMS_SET_ADDRESS:
            mr = mraa_i2c_write_byte_data(dev->_i2c_context, wr_data, cmd);
            break;
        /* Zero byte write */
        case IMS_MEASURE_LIGHT:
        case IMS_RESET:
        case IMS_SLEEP:
            mr = mraa_i2c_write(dev->_i2c_context, (const uint8_t *)&cmd, 1);
            break;
        default:
            syslog(LOG_ERR, "%s: Invalid read command: 0x%02x\n", __FUNCTION__, cmd);
            return UPM_ERROR_INVALID_PARAMETER;
    }

    if (mr != MRAA_SUCCESS) return UPM_ERROR_OPERATION_FAILED;

    return UPM_SUCCESS;
}

upm_result_t ims_get_version(const ims_context* dev, uint16_t* rd_data)
{
    return ims_read(dev, IMS_GET_VERSION, rd_data);
}

upm_result_t ims_get_moisture(const ims_context* dev, uint16_t* rd_data)
{
    return ims_read(dev, IMS_GET_CAPACITANCE, rd_data);
}

upm_result_t ims_get_light(const ims_context* dev, uint16_t* rd_data)
{
    /* Initiate a light measurement */
    upm_result_t res = ims_write(dev, IMS_MEASURE_LIGHT, 0);
    if (res != UPM_SUCCESS) return res;

    /* Technical data for the IMS specifies a 3 second wait.  Check the BUSY
     * command every 100 ms for 3 seconds.  The sensor will return quickly in
     * bright light and much slower in less light. */
    int retry = 30;
    *rd_data = 1;
    while ((retry-- > 0) && (*rd_data != 0))
    {
        // Always delay at least 100 ms
        upm_delay_ms(100);
        res = ims_read(dev, IMS_GET_BUSY, rd_data);
        if (res != UPM_SUCCESS) return res;
    }

    /* If the sensor is STILL not ready, go ahead and perform the read.
     * From testing, this appears to happen only in complete darkness,
     * at which point the sensor get light read returns 0xffff anyway.*/

    return ims_read(dev, IMS_GET_LIGHT, rd_data);
}

upm_result_t ims_get_temperature(const ims_context* dev, uint16_t* rd_data)
{
    return ims_read(dev, IMS_GET_TEMPERATURE, rd_data);
}

upm_result_t ims_reset(const ims_context* dev)
{
    return ims_write(dev, IMS_RESET, 0);
}

upm_result_t ims_reset_i2c_address(const ims_context* dev, uint8_t address_new)
{
    upm_result_t res = ims_write(dev, IMS_SET_ADDRESS, address_new);
    if (res != UPM_SUCCESS) return res;

    return ims_reset(dev);
}

upm_result_t ims_sleep(const ims_context* dev)
{
    return ims_write(dev, IMS_SLEEP, 0);
}
