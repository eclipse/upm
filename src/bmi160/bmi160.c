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

#include <unistd.h>
#include <string.h>
#include <assert.h>
#include <mraa/i2c.h>
#include <mraa/spi.h>
#include <mraa/gpio.h>

#include "bmi160.h"

#include <upm_utilities.h>

// we have to do it the old skool way.  Note, this also means that
// only one instance of the bmi160 driver can be active at a time.
static mraa_i2c_context i2cContext = NULL;
static mraa_spi_context spiContext = NULL;
/* this is used for chip-select when using SPI */
static mraa_gpio_context gpioContext = NULL;

// whether we are doing I2C or SPI
static bool isSPI = false;

// Our bmi160 info structure
struct bmi160_t s_bmi160;

// For SPI, these are our CS on/off functions, if needed
static void bmi160_cs_on()
{
    if (gpioContext)
        mraa_gpio_write(gpioContext, 0);
}

static void bmi160_cs_off()
{
    if (gpioContext)
        mraa_gpio_write(gpioContext, 1);
}

// i2c bus read and write functions for use with the bmi driver code
s8 bmi160_bus_read(u8 dev_addr, u8 reg_addr, u8 *reg_data, u8 cnt)
{
    if (isSPI)
    {
        if (!spiContext)
        {
            printf("%s: spiContext is NULL.\n", __FUNCTION__);
            return 1;
        }

        reg_addr |= 0x80; // needed for read

        uint8_t sbuf[cnt + 1];
        memset((char *)sbuf, 0, cnt + 1);
        sbuf[0] = reg_addr;

        bmi160_cs_on();

        if (mraa_spi_transfer_buf(spiContext, sbuf, sbuf, cnt + 1))
        {
            bmi160_cs_off();
            printf("%s: mraa_spi_transfer_buf() failed.\n", __FUNCTION__);
            return 1;
        }
        bmi160_cs_off();

      // now copy it into user buffer
        int i;
        for (i=0; i<cnt; i++)
            reg_data[i] = sbuf[i + 1];

        return 0;
    }

    // doing I2C

    if (!i2cContext)
    {
        printf("%s: i2cContext is NULL.\n", __FUNCTION__);
        return 1;
    }

    if (mraa_i2c_read_bytes_data(i2cContext, reg_addr, reg_data, cnt) < 0)
    {
        printf("%s: mraa_i2c_read_bytes() failed.\n", __FUNCTION__);
        return 1;
    }

    return 0;
}

s8 bmi160_bus_write(u8 dev_addr, u8 reg_addr, u8 *reg_data, u8 cnt)
{
    if (isSPI)
    {
        if (!spiContext)
        {
            printf("%s: spiContext is NULL.\n", __FUNCTION__);
            return 1;
        }

        reg_addr &= 0x7f; // mask off 0x80 for writing

        uint8_t sbuf[cnt + 1];
        memset((char *)sbuf, 0, cnt + 1);
        sbuf[0] = reg_addr;

        // copy in the data to write...
        int i;
        for (i=0; i<cnt; i++)
            sbuf[i + 1] = reg_data[i];

        bmi160_cs_on();

        if (mraa_spi_transfer_buf(spiContext, sbuf, sbuf, cnt + 1))
        {
            bmi160_cs_off();
            printf("%s: mraa_spi_transfer_buf() failed.\n", __FUNCTION__);
            return 1;
        }
        bmi160_cs_off();

        return 0;
    }

    // I2C...
    if (!i2cContext)
    {
        printf("%s: i2cContext is NULL.\n", __FUNCTION__);
        return 1;
    }

    uint8_t buffer[cnt + 1];

    buffer[0] = reg_addr;
    int i;
    for (i=0; i<cnt; i++)
        buffer[i+1] = reg_data[i];

    mraa_result_t rv = mraa_i2c_write(i2cContext, buffer, cnt+1);

    if (rv != MRAA_SUCCESS)
    {
        printf("%s: mraa_i2c_write() failed.\n", __FUNCTION__);
        return 1;
    }

    return 0;
}

// delay for some milliseconds
void bmi160_delay_ms(u32 msek)
{
  upm_delay_ms(msek);
}


bmi160_context bmi160_init(unsigned int bus, int address, int cs_pin,
                           bool enable_mag)
{
    bmi160_context dev =
        (bmi160_context)malloc(sizeof(struct _bmi160_context));

    if (!dev)
        return NULL;

    // zero out context
    memset((void *)dev, 0, sizeof(struct _bmi160_context));

    // make sure MRAA is initialized
    int mraa_rv;
    if ((mraa_rv = mraa_init()) != MRAA_SUCCESS)
    {
        printf("%s: mraa_init() failed (%d).\n", __FUNCTION__, mraa_rv);
        bmi160_close(dev);
        return NULL;
    }

    if (address > 0)
    {
        // we are doing I2C
        isSPI = false;

        if (!(i2cContext = mraa_i2c_init(bus)))
        {
            printf("%s: mraa_i2c_init() failed.\n", __FUNCTION__);
            bmi160_close(dev);
            return NULL;
        }

        if (mraa_i2c_address(i2cContext, address) != MRAA_SUCCESS)
        {
            printf("%s: mraa_i2c_address() failed.\n", __FUNCTION__);
            bmi160_close(dev);
            return NULL;
        }
    }
    else
    {
        // we are doing SPI
        isSPI = true;

        if (!(spiContext = mraa_spi_init(bus)))
        {
            printf("%s: mraa_spi_init() failed.\n", __FUNCTION__);
            bmi160_close(dev);
            return NULL;
        }

        // Only create cs context if we are actually using a valid pin.
        // A hardware controlled pin should specify cs as -1.
        if (cs_pin >= 0)
        {
            if (!(gpioContext = mraa_gpio_init(cs_pin)))
            {
                printf("%s: mraa_gpio_init() failed.\n", __FUNCTION__);
                bmi160_close(dev);
                return NULL;
            }

            mraa_gpio_dir(gpioContext, MRAA_GPIO_OUT);
            bmi160_cs_off();
        }

        if (mraa_spi_mode(spiContext, MRAA_SPI_MODE0))
        {
            printf("%s: mraa_spi_mode() failed.\n", __FUNCTION__);
            bmi160_close(dev);
            return NULL;
        }

        if (mraa_spi_frequency(spiContext, 5000000))
        {
            printf("%s: mraa_spi_frequency() failed.\n", __FUNCTION__);
            bmi160_close(dev);
            return NULL;
        }
    }

    // init the driver interface functions
    s_bmi160.bus_write = bmi160_bus_write;
    s_bmi160.bus_read = bmi160_bus_read;
    s_bmi160.delay_msec = bmi160_delay_ms;
    if (isSPI)
        s_bmi160.dev_addr = 0;
    else
        s_bmi160.dev_addr = address & 0xff;

    // Init our driver interface pointers
    if (bmi160_init_bus(&s_bmi160))
    {
        printf("%s: bmi160_bus_init() failed.\n", __FUNCTION__);
        bmi160_close(dev);
        return NULL;
    }

    // bmi160_init_bus will read the chip Id and deposit into our
    // interface struct.  So, check it out and make sure it's correct.
    if (s_bmi160.chip_id != BMI160_CHIP_ID)
    {
        printf("%s: Error: expected chip id %02x, but got %02x.\n",
               __FUNCTION__, BMI160_CHIP_ID, s_bmi160.chip_id);
        bmi160_close(dev);
        return NULL;
    }

    dev->accelScale = 1.0;
    dev->gyroScale = 1.0;
    dev->magEnabled = false;

    // This should be interesting...
    const u32 C_BMI160_THIRTY_U8X = 30;

    bmi160_enable_magnetometer(dev, enable_mag);

    /* Set the accel mode as Normal write in the register 0x7E */
    bmi160_set_command_register(ACCEL_MODE_NORMAL);

    /* bmi160_delay_ms in ms */
    bmi160_delay_ms(C_BMI160_THIRTY_U8X);

    /* Set the gyro mode as Normal write in the register 0x7E */
    bmi160_set_command_register(GYRO_MODE_NORMAL);

    /* bmi160_delay_ms in ms */
    bmi160_delay_ms(C_BMI160_THIRTY_U8X);

    /* Set the accel bandwidth as OSRS4 */
    bmi160_set_accel_bw(BMI160_ACCEL_OSR4_AVG1);
    bmi160_delay_ms(BMI160_GEN_READ_WRITE_DELAY);

    /* Set the gryo bandwidth as Normal */
    bmi160_set_gyro_bw(BMI160_GYRO_NORMAL_MODE);
    bmi160_delay_ms(BMI160_GEN_READ_WRITE_DELAY);

    /* set gyro data rate as 200Hz */
    bmi160_set_gyro_output_data_rate(BMI160_GYRO_OUTPUT_DATA_RATE_200HZ);
    bmi160_delay_ms(BMI160_GEN_READ_WRITE_DELAY);

    /* set accel data rate as 200Hz */
    bmi160_set_accel_output_data_rate(BMI160_ACCEL_OUTPUT_DATA_RATE_200HZ,
                                      BMI160_ACCEL_OSR4_AVG1);
    bmi160_delay_ms(BMI160_GEN_READ_WRITE_DELAY);

    bmi160_set_accelerometer_scale(dev, BMI160_ACC_RANGE_2G);
    bmi160_set_gyroscope_scale(dev, BMI160_GYRO_RANGE_125);

    return dev;
}

void bmi160_close(bmi160_context dev)
{
    assert(dev != NULL);

    if (i2cContext)
        mraa_i2c_stop(i2cContext);
    i2cContext = NULL;

    if (spiContext)
        mraa_spi_stop(spiContext);
    spiContext = NULL;

    if (gpioContext)
        mraa_gpio_close(gpioContext);
    gpioContext = NULL;

    free(dev);
}

void bmi160_update(const bmi160_context dev)
{
    assert(dev != NULL);

    struct bmi160_gyro_t gyroxyz;
    struct bmi160_accel_t accelxyz;
    struct bmi160_mag_xyz_s32_t magxyz;

    // read gyro data
    bmi160_read_gyro_xyz(&gyroxyz);

    // read accel data
    bmi160_read_accel_xyz(&accelxyz);

    // read mag data
    if (dev->magEnabled)
        bmi160_bmm150_mag_compensate_xyz(&magxyz);

    // read the sensor time
    u32 v_sensor_time;
    bmi160_get_sensor_time(&v_sensor_time);
    dev->sensorTime = (unsigned int)v_sensor_time;

    dev->accelX = (float)accelxyz.x;
    dev->accelY = (float)accelxyz.y;
    dev->accelZ = (float)accelxyz.z;

    dev->gyroX = (float)gyroxyz.x;
    dev->gyroY = (float)gyroxyz.y;
    dev->gyroZ = (float)gyroxyz.z;

    if (dev->magEnabled)
    {
        dev->magX = (float)magxyz.x;
        dev->magY = (float)magxyz.y;
        dev->magZ = (float)magxyz.z;
    }
}

void bmi160_set_accelerometer_scale(const bmi160_context dev,
                                    BMI160_ACC_RANGE_T scale)
{
    assert(dev != NULL);

    s8 v_range = BMI160_ACCEL_RANGE_2G;
    // store scaling factor

    switch (scale)
    {
    case BMI160_ACC_RANGE_2G:
        v_range = BMI160_ACCEL_RANGE_2G;
        dev->accelScale = 16384.0;
        break;

    case BMI160_ACC_RANGE_4G:
        v_range = BMI160_ACCEL_RANGE_4G;
        dev->accelScale = 8192.0;
        break;

    case BMI160_ACC_RANGE_8G:
        v_range = BMI160_ACCEL_RANGE_8G;
        dev->accelScale = 4096.0;
        break;

    case BMI160_ACC_RANGE_16G:
        v_range = BMI160_ACCEL_RANGE_16G;
        dev->accelScale = 2048.0;
        break;

    default: // should never occur, but...
        dev->accelScale = 1.0;        // set a safe, though incorrect value
        printf("%s: internal error, unsupported scale.\n", __FUNCTION__);
        break;
    }

    bmi160_set_accel_range(v_range);

    return;
}

void bmi160_set_gyroscope_scale(const bmi160_context dev,
                                BMI160_GYRO_RANGE_T scale)
{
    assert(dev != NULL);

    u8 v_range = BMI160_GYRO_RANGE_2000_DEG_SEC;

    // store scaling factor

    switch (scale)
    {
    case BMI160_GYRO_RANGE_125:
        v_range = BMI160_GYRO_RANGE_125_DEG_SEC;
        dev->gyroScale = 262.4;
        break;

    case BMI160_GYRO_RANGE_250:
        v_range = BMI160_GYRO_RANGE_250_DEG_SEC;
        dev->gyroScale = 131.2;
        break;

    case BMI160_GYRO_RANGE_500:
        v_range = BMI160_GYRO_RANGE_500_DEG_SEC;
        dev->gyroScale = 65.6;
        break;

    case BMI160_GYRO_RANGE_1000:
        v_range = BMI160_GYRO_RANGE_1000_DEG_SEC;
        dev->gyroScale = 32.8;
        break;

    case BMI160_GYRO_RANGE_2000:
        v_range = BMI160_GYRO_RANGE_2000_DEG_SEC;
        dev->gyroScale = 16.4;
        break;

    default: // should never occur, but...
        dev->gyroScale = 1.0;        // set a safe, though incorrect value
        printf("%s: internal error, unsupported scale.\n", __FUNCTION__);
        break;
    }

    bmi160_set_gyro_range(v_range);

    return;
}

void bmi160_get_accelerometer(const bmi160_context dev, float *x, float *y,
                              float *z)
{
    assert(dev != NULL);

    if (x)
        *x = dev->accelX / dev->accelScale;

    if (y)
        *y = dev->accelY / dev->accelScale;

    if (z)
        *z = dev->accelZ / dev->accelScale;
}

void bmi160_get_gyroscope(const bmi160_context dev, float *x, float *y,
                          float *z)
{
    assert(dev != NULL);

    if (x)
        *x = dev->gyroX / dev->gyroScale;

    if (y)
        *y = dev->gyroY / dev->gyroScale;

    if (z)
        *z = dev->gyroZ / dev->gyroScale;
}

void bmi160_get_magnetometer(const bmi160_context dev, float *x, float *y,
                             float *z)
{
    assert(dev != NULL);

    if (x)
        *x = dev->magX;

    if (y)
        *y = dev->magY;

    if (z)
        *z = dev->magZ;
}

#if 0
float *bmi160_get_ccelerometer()
{
    float *values = new float[3]; // x, y, and then z

    getAccelerometer(&values[0], &values[1], &values[2]);

    return values;
}

float *bmi160_getGyroscope()
{
    float *values = new float[3]; // x, y, and then z

    getGyroscope(&values[0], &values[1], &values[2]);

    return values;
}

float *bmi160_getMagnetometer()
{
    float *values = new float[3]; // x, y, and then z

    getMagnetometer(&values[0], &values[1], &values[2]);

    return values;
}
#endif

void bmi160_enable_magnetometer(const bmi160_context dev, bool enable)
{
    assert(dev != NULL);

    // butchered from support example
    if (!enable)
    {
        bmi160_set_bmm150_mag_and_secondary_if_power_mode(MAG_SUSPEND_MODE);
        bmi160_delay_ms(BMI160_GEN_READ_WRITE_DELAY);
        bmi160_set_if_mode(0x00);
        bmi160_delay_ms(BMI160_GEN_READ_WRITE_DELAY);

        dev->magEnabled = false;
        dev->magX = 0;
        dev->magY = 0;
        dev->magZ = 0;
    }
    else
    {
        u8 v_bmm_chip_id_u8 = BMI160_INIT_VALUE;
        /* Init the magnetometer */
        bmi160_bmm150_mag_interface_init(&v_bmm_chip_id_u8);

        /* bmi160_delay_ms in ms*/
        bmi160_delay_ms(BMI160_GEN_READ_WRITE_DELAY);

        dev->magEnabled = true;
    }
}

unsigned int bmi160_get_time(const bmi160_context dev)
{
    assert(dev != NULL);

    return dev->sensorTime;
}
