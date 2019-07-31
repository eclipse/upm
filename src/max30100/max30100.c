/*
 * Author: Noel Eck <noel.eck@intel.com>
 * Copyright (c) 2015 Intel Corporation.
 *
 * This program and the accompanying materials are made available under the
 * terms of the The MIT License which is available at
 * https://opensource.org/licenses/MIT.
 *
 * SPDX-License-Identifier: MIT
 */

#include <assert.h>
#include <stdlib.h>
#include <syslog.h>
#include <math.h>

#include "max30100.h"
#include "upm_utilities.h"

max30100_context* max30100_init(int16_t i2c_bus)
{
    /* Allocate space for the sensor structure */
    max30100_context* dev = (max30100_context*) malloc(sizeof(max30100_context));
    if(dev == NULL)
    {
        syslog(LOG_CRIT, "%s: malloc() failed\n", __FUNCTION__);
        goto max30100_init_fail;
    }

    /* Initialize mraa */
    mraa_result_t result = mraa_init();
    if (result != MRAA_SUCCESS)
    {
        syslog(LOG_ERR, "%s: mraa_init() failed (%d)\n", __FUNCTION__, result);
        goto max30100_init_fail;
    }

    /* Initialize I2C */
    dev->_i2c_context = mraa_i2c_init(i2c_bus);
    if(dev->_i2c_context == NULL)
    {
        syslog(LOG_ERR, "%s: mraa_i2c_init() failed\n", __FUNCTION__);
        goto max30100_init_fail;
    }

    /* Set the I2C slave address for this device */
    if (mraa_i2c_address(dev->_i2c_context, MAX30100_I2C_ADDRESS) != MRAA_SUCCESS)
    {
        syslog(LOG_ERR, "%s: mraa_i2c_address() failed\n", __FUNCTION__);
        goto max30100_init_fail;
    }

    /* Attempt to run the device at 100kHz */
    if (mraa_i2c_frequency(dev->_i2c_context, MRAA_I2C_STD))
        syslog(LOG_ERR, "%s: mraa_i2c_frequency() failed, device may not function correctly\n", __FUNCTION__);

    /* Start without GPIO */
    dev->_gpio_context = NULL;

    return dev;

    /* Handle all failing cases here */
max30100_init_fail:
    /* Free structure memory if allocated */
    if (dev != NULL)
        free(dev);

    return NULL;
}

void max30100_close(max30100_context* dev)
{
    assert(dev != NULL && "max30100_close: Context cannot be NULL");

    /* Cleanup the I2C context */
    mraa_i2c_stop(dev->_i2c_context);
    free(dev);
}

static void internal_uninstall_isr(max30100_context* dev)
{
    assert(dev != NULL && "internal_uninstall_isr: Context cannot be NULL");

    /* If no GPIO context exists, return */
    if (dev->_gpio_context == NULL) return;

    mraa_gpio_isr_exit(dev->_gpio_context);
    mraa_gpio_close(dev->_gpio_context);
    dev->_gpio_context = NULL;
}

static upm_result_t _internal_install_isr(max30100_context* dev, int gpio_pin,
                                 void (*isr)(void *), void *arg)
{
    /* Only allow one ISR */
    internal_uninstall_isr(dev);

    if (!(dev->_gpio_context = mraa_gpio_init(gpio_pin)))
        return UPM_ERROR_OPERATION_FAILED;

    /* Set the GPIO to input */
    if (mraa_gpio_dir(dev->_gpio_context, MRAA_GPIO_IN) != MRAA_SUCCESS)
        return UPM_ERROR_OPERATION_FAILED;

    /* MAX30100 interrupts are active low, pull GPIO high */
    if (mraa_gpio_mode(dev->_gpio_context, MRAA_GPIO_PULLUP) != MRAA_SUCCESS)
        return UPM_ERROR_OPERATION_FAILED;

    /* Install the interrupt handler */
    if (mraa_gpio_isr(dev->_gpio_context, MRAA_GPIO_EDGE_FALLING, isr, arg) != MRAA_SUCCESS)
        return UPM_ERROR_OPERATION_FAILED;

    return UPM_SUCCESS;
}

upm_result_t max30100_read(const max30100_context* dev, MAX30100_REG reg, uint8_t* rd_data)
{
    assert(dev != NULL && "max30100_read: Context cannot be NULL");
    /* Read the register */
    int tmp_val = mraa_i2c_read_byte_data(dev->_i2c_context, reg);
    if (tmp_val < 0) return UPM_ERROR_OPERATION_FAILED;

    *rd_data = (uint8_t)tmp_val;

    return UPM_SUCCESS;
}

upm_result_t max30100_write(const max30100_context* dev, MAX30100_REG reg, uint8_t wr_data)
{
    assert(dev != NULL && "max30100_write: Context cannot be NULL");

    if (mraa_i2c_write_byte_data(dev->_i2c_context, (uint8_t)wr_data, reg) != MRAA_SUCCESS)
        return UPM_ERROR_OPERATION_FAILED;

    return UPM_SUCCESS;
}

upm_result_t max30100_rd_mod_wr(const max30100_context* dev,
        MAX30100_REG reg, uint8_t value, uint8_t mask)
{
    uint8_t tmp_val = 0;

    /* Read the register */
    upm_result_t result = max30100_read(dev, reg, &tmp_val);

    if (result != UPM_SUCCESS) return result;

    /* Modify the value, firt clear the bits from mask */
    tmp_val &= (~mask);
    /* Make sure the new value doesn't have anything set outside the mask */
    value &= mask;
    /* OR in the new value */
    tmp_val |= value;

    /* Write the value back */
    return max30100_write(dev, reg, tmp_val);
}

upm_result_t max30100_get_version(const max30100_context* dev, uint16_t* version)
{
    assert(dev != NULL && "max30100_get_version: Context cannot be NULL");

    /* Read the revision ID */
    uint8_t tmp_val = 0;
    upm_result_t result = max30100_read(dev, MAX30100_REG_REV_ID, &tmp_val);
    if (result != UPM_SUCCESS) return result;

    *version = (uint8_t)tmp_val;

    result = max30100_read(dev, MAX30100_REG_PART_ID, &tmp_val);
    if (result != UPM_SUCCESS) return result;

    /* Move the PART ID to upper byte */
    *version += ((uint16_t)tmp_val << 8);

    return UPM_SUCCESS;
}

upm_result_t max30100_get_temperature(const max30100_context* dev, float* temperature)
{
    assert(dev != NULL && "max30100_get_temperature: Context cannot be NULL");
    int8_t tmp_val = 0;

    /* First, set TEMP_EN to initiate a temperature read */
    upm_result_t result = max30100_rd_mod_wr(dev, MAX30100_REG_MODE_CONFIG,
            MAX30100_TEMP_EN, MAX30100_TEMP_EN);
    if (result != UPM_SUCCESS) return result;

    /* Note, the docs for reading a temperature value states:
     * This is a self-clearing bit which, when set, initiates a single
     * temperature reading from the temperature sensor. This bit is
     * cleared automatically back to zero at the conclusion of the
     * temperature reading when the bit is set to one in heart rate
     * or SpO2 mode.
     *
     * However, the next read of the MODE CONFIG register *always* seems
     * to have TEMP_EN cleared w/o values in TINT/TFRAC until a short
     * while later.  To account for this, a delay has been added - sorry */

    upm_delay_ms(100);

    /* Read the integer portion of the temperature */
    result = max30100_read(dev, MAX30100_REG_TEMP_INTEGER, (uint8_t*)&tmp_val);
    if (result != UPM_SUCCESS) return result;

    /* cast the signed integer portion to float */
    *temperature = (float)tmp_val;

    /* This register stores the fractional temperature data in increments of
     * 0.0625C (1/16th of a degree C).  If this fractional temperature is
     * paired with a negative integer, it still adds as a positive fractional
     * value (e.g., -128 C + 0.5 C = -127.5 C). */
    result = max30100_read(dev, MAX30100_REG_TEMP_FRACTION, (uint8_t*)&tmp_val);
    if (result != UPM_SUCCESS) return result;

    /* Add the fraction */
    *temperature += ((float)tmp_val)/16.0;

    return UPM_SUCCESS;
}

upm_result_t max30100_set_mode(const max30100_context* dev, MAX30100_MODE mode)
{
    assert(dev != NULL && "max30100_set_mode: Context cannot be NULL");
    return max30100_rd_mod_wr(dev, MAX30100_REG_MODE_CONFIG, (uint8_t)mode, 0x03);
}

upm_result_t max30100_get_mode(const max30100_context* dev, MAX30100_MODE* mode)
{
    assert(dev != NULL && "max30100_get_mode: Context cannot be NULL");

    /* Read the mode configuration register */
    uint8_t data = 0;
    upm_result_t result = max30100_read(dev, MAX30100_REG_MODE_CONFIG, &data);
    if (result != UPM_SUCCESS) return result;

    *mode = (MAX30100_MODE) data & 0x7;

    return UPM_SUCCESS;
}

upm_result_t max30100_set_high_res(const max30100_context* dev, bool high_res)
{
    assert(dev != NULL && "MAX30100_set_high_res: Context cannot be NULL");
    uint8_t wr_val = high_res ? MAX30100_SPO2_HI_RES_EN : ~MAX30100_SPO2_HI_RES_EN;
    return max30100_rd_mod_wr(dev, MAX30100_REG_SPO2_CONFIG, wr_val, 0x40);
}

upm_result_t max30100_get_high_res(const max30100_context* dev, bool* high_res)
{
    assert(dev != NULL && "MAX30100_get_high_res: Context cannot be NULL");

    /* Read the SpO2 configuration register */
    uint8_t data = 0;
    upm_result_t result = max30100_read(dev, MAX30100_REG_SPO2_CONFIG, &data);
    if (result != UPM_SUCCESS) return result;

    *high_res = data & 0x40;

    return UPM_SUCCESS;
}

upm_result_t max30100_set_sample_rate(const max30100_context* dev, MAX30100_SR sample_rate)
{
    assert(dev != NULL && "MAX30100_set_sample_rate: Context cannot be NULL");
    return max30100_rd_mod_wr(dev, MAX30100_REG_SPO2_CONFIG, (uint8_t)sample_rate << 2, 0x1c);
}

upm_result_t max30100_get_sample_rate(const max30100_context* dev, MAX30100_SR* sample_rate)
{
    assert(dev != NULL && "MAX30100_get_high_res: Context cannot be NULL");

    /* Read the SpO2 configuration register */
    uint8_t data = 0;
    upm_result_t result = max30100_read(dev, MAX30100_REG_SPO2_CONFIG, &data);
    if (result != UPM_SUCCESS) return result;

    *sample_rate = (MAX30100_SR)((data >> 2) & 0x7);

    return UPM_SUCCESS;
}

upm_result_t max30100_set_pulse_width(const max30100_context* dev, MAX30100_LED_PW pulse_width)
{
    assert(dev != NULL && "MAX30100_set_pulse_width: Context cannot be NULL");
    return max30100_rd_mod_wr(dev, MAX30100_REG_SPO2_CONFIG, (uint8_t)pulse_width, 0x03);
}

upm_result_t max30100_get_pulse_width(const max30100_context* dev, MAX30100_LED_PW* pulse_width)
{
    assert(dev != NULL && "MAX30100_get_high_res: Context cannot be NULL");

    /* Read the SpO2 configuration register */
    uint8_t data = 0;
    upm_result_t result = max30100_read(dev, MAX30100_REG_SPO2_CONFIG, &data);
    if (result != UPM_SUCCESS) return result;

    *pulse_width = (MAX30100_LED_PW)(data & 0x3);

    return UPM_SUCCESS;
}

upm_result_t max30100_set_current(const max30100_context* dev,
        MAX30100_LED_CURRENT ir, MAX30100_LED_CURRENT r)
{
    assert(dev != NULL && "max30100_set_current: Context cannot be NULL");

    return max30100_write(dev, MAX30100_REG_LED_CONFIG,
            (uint8_t)((r << 4) | r));
}

upm_result_t max30100_get_current(const max30100_context* dev,
        MAX30100_LED_CURRENT* ir, MAX30100_LED_CURRENT* r)
{
    assert(dev != NULL && "max30100_get_current: Context cannot be NULL");

    /* Read the LED configuration register */
    uint8_t data = 0;
    upm_result_t result = max30100_read(dev, MAX30100_REG_LED_CONFIG, &data);
    if (result != UPM_SUCCESS) return result;

    *ir = (MAX30100_LED_CURRENT)(data & 0x0f);
    *r = (MAX30100_LED_CURRENT)((data >> 4) & 0x0f);

    return UPM_SUCCESS;
}

upm_result_t max30100_reset(const max30100_context* dev)
{
    assert(dev != NULL && "max30100_reset: Context cannot be NULL");

    /* Set the RESET bit, don't worry about read/mod/write */
    return max30100_write(dev, MAX30100_REG_MODE_CONFIG, (uint8_t)0x40);
}

upm_result_t max30100_sleep(const max30100_context* dev, bool sleep)
{
    assert(dev != NULL && "max30100_sleep: Context cannot be NULL");

    /* Read/mod/write to set the SHDN bit */
    uint8_t wr_val = sleep ? MAX30100_SHDN : (uint8_t)~MAX30100_SHDN;
    return max30100_rd_mod_wr(dev, MAX30100_REG_MODE_CONFIG, wr_val, MAX30100_SHDN);
}

static upm_result_t _read_single_sample(const max30100_context* dev, max30100_value *samp)
{
    uint8_t data[4];
    if (mraa_i2c_read_bytes_data(dev->_i2c_context,  MAX30100_REG_FIFO_DATA, data, 4) != 4)
        return UPM_ERROR_OPERATION_FAILED;

    samp->IR = ((uint16_t)data[0] << 8) | data[1];
    samp->R = ((uint16_t)data[2] << 8) | data[3];

    return UPM_SUCCESS;
}

static void _internal_sample_rdy(void *arg)
{
    max30100_context* dev = arg;

    if (dev->sample_state == MAX30100_SAMPLE_STATE_IDLE) return;

    int i = 15;
    max30100_value samp = {0, 0};
    /* If state is BUFFERED, read 16 samples, else read 1 sample */
    do
    {
        if (_read_single_sample(dev, &samp) != UPM_SUCCESS)
            goto max30100_sample_rdy_fail;

        // Call handler
        dev->func_sample_ready(samp, dev->arg);

    } while ((i-- > 0) && (dev->sample_state == MAX30100_SAMPLE_STATE_CONTINUOUS_BUFFERED));

    /* If a FIFO full interrupt generated this, clear it by reading sts */
    uint8_t tmp;
    if (dev->sample_state == MAX30100_SAMPLE_STATE_CONTINUOUS_BUFFERED)
        if(max30100_read(dev, MAX30100_REG_INTERRUPT_STATUS, &tmp) != UPM_SUCCESS)
            goto max30100_sample_rdy_fail;

    return;

    /* If a failure occurs in this method (which running on a seperate thread,
     * log an error in syslog and attempt to stop sampling
     * Handle all failing cases here */
    max30100_sample_rdy_fail:
        syslog(LOG_CRIT,
                "%s: _internal_sample_rdy() failed, attempting to restart sampling...\n",
                __FUNCTION__);

        /* Read FIFO AND status register in a last attempt to continue
         * sampling, ignore result */
        max30100_read(dev, MAX30100_REG_FIFO_WR_PTR, &tmp);
        max30100_read(dev, MAX30100_REG_INTERRUPT_STATUS, &tmp);

        return;
}

upm_result_t max30100_sample(max30100_context* dev, max30100_value *samp)
{
    assert(dev != NULL && "max30100_sample: Context cannot be NULL");

    upm_result_t result = UPM_SUCCESS;

    // Disable interrupts
    result = max30100_write(dev, MAX30100_REG_INTERRUPT_ENABLE, 0x00);
    if (result != UPM_SUCCESS) return result;

    /* Set the state to one-shot */
    dev->sample_state = MAX30100_SAMPLE_STATE_ONE_SHOT;

    /* Clear wr/rd pointers */
    result = max30100_write(dev, MAX30100_REG_FIFO_WR_PTR, 0x00);
    if (result != UPM_SUCCESS) return result;
    result = max30100_write(dev, MAX30100_REG_FIFO_RD_PTR, 0x00);
    if (result != UPM_SUCCESS) return result;

    /* Wait for a sample */
    uint8_t wr_ptr = 0;
    int retry = 50;
    while ((wr_ptr == 0) && (--retry > 0))
    {
        result = max30100_read(dev, MAX30100_REG_FIFO_WR_PTR, &wr_ptr);
        if (result != UPM_SUCCESS) return result;
    }

    /* Return timeout if retry count is zero */
    if (retry == 0) return UPM_ERROR_TIMED_OUT;

    /* Set the rd ptr to wr ptr to ensure reading the most current sample */
    result = max30100_write(dev, MAX30100_REG_FIFO_RD_PTR, wr_ptr - 1);
    if (result != UPM_SUCCESS) return result;

    /* Read the sample */
    if (_read_single_sample(dev, samp) != UPM_SUCCESS)
        return UPM_ERROR_OPERATION_FAILED;

    return UPM_SUCCESS;
}

upm_result_t max30100_sample_continuous(max30100_context* dev, int gpio_pin,
        bool buffered, func_sample_ready_handler isr, void* arg)
{
    assert(dev != NULL && "max30100_sample_continuous: Context cannot be NULL");
    uint8_t tmp;

    upm_result_t result = UPM_SUCCESS;

    // Set state to IDLE
    dev->sample_state = MAX30100_SAMPLE_STATE_IDLE;

    // Disable interrupts
    result = max30100_write(dev, MAX30100_REG_INTERRUPT_ENABLE, 0x00);
    if (result != UPM_SUCCESS) return result;

    /* Setup the external callback info */
    dev->func_sample_ready = isr;
    dev->arg = arg;
    // Register internal callback handler
    result = _internal_install_isr(dev, gpio_pin, _internal_sample_rdy, dev);
    if (result != UPM_SUCCESS) return result;

    uint8_t tmp_int_en = 0;
    if (buffered)
    {
        dev->sample_state = MAX30100_SAMPLE_STATE_CONTINUOUS_BUFFERED;

        // Set value of interrupt for FIFO_FULL
        tmp_int_en = MAX30100_EN_A_FULL;
    }
    else
    {
        dev->sample_state = MAX30100_SAMPLE_STATE_CONTINUOUS;

        // Read the mode field from the mode configuration register,
        // decide which interrupt to set
        result = max30100_read(dev, MAX30100_REG_MODE_CONFIG, &tmp);
        if (result != UPM_SUCCESS) return result;
        MAX30100_MODE mode = (MAX30100_MODE)(tmp & 0x3);

        // Set value of interrupt for HR or SpO2
        tmp_int_en = mode == MAX30100_MODE_HR_EN ? MAX30100_EN_HR_RDY :
            (mode == MAX30100_MODE_SPO2_EN ? MAX30100_EN_SPO2_RDY : 0x00);
    }

    /* Clear wr/rd pointers */
    result = max30100_write(dev, MAX30100_REG_FIFO_WR_PTR, 0x00);
    if (result != UPM_SUCCESS) return result;
    result = max30100_write(dev, MAX30100_REG_FIFO_RD_PTR, 0x00);
    if (result != UPM_SUCCESS) return result;

    /* Enable interrupt, either FIFO full, HR only, or SpO2 */
    result = max30100_write(dev, MAX30100_REG_INTERRUPT_ENABLE, tmp_int_en);
    if (result != UPM_SUCCESS) return result;

    /* Read the STATUS register to get things moving */
    result = max30100_read(dev, MAX30100_REG_INTERRUPT_STATUS, &tmp);
    if (result != UPM_SUCCESS) return result;

    return UPM_SUCCESS;
}

upm_result_t max30100_sample_stop(max30100_context* dev)
{
    assert(dev != NULL && "max30100_sample_stop: Context cannot be NULL");

    dev->sample_state = MAX30100_SAMPLE_STATE_IDLE;

    /* Uninstall sampling ISR */
    internal_uninstall_isr(dev);

//    // Disable sampling
//    upm_result_t result = max30100_write(dev, MAX30100_REG_MODE_CONFIG, MAX30100_MODE_DISABLED);
//    if (result != UPM_SUCCESS) return result;

    // Disable interrupts
    upm_result_t result = max30100_write(dev, MAX30100_REG_INTERRUPT_ENABLE, 0);
    if (result != UPM_SUCCESS) return result;

    return UPM_SUCCESS;
}

