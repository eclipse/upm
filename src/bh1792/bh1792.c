/*
* The MIT License (MIT)
*
* Author: Assam Boudjelthia
* Copyright (c) 2018 Rohm Semiconductor.
*
* This program and the accompanying materials are made available under the
* terms of the The MIT License which is available at
* https://opensource.org/licenses/MIT.
*
* SPDX-License-Identifier: MIT
*/

#include <stdio.h>
#include "bh1792.h"

/**
 * @brief Reads the value of a register
 *
 * @param dev The sensor context
 * @param reg Register address
 * @param data Pointer of uint8_t to save register value
 * @return UPM result
 */
static upm_result_t bh1792_read_register(bh1792_context dev, uint8_t reg,
                                            uint8_t *data);

/**
 * @brief Reads the values of a set of registers
 *
 * @param dev The sensor context
 * @param reg Register address
 * @param data Pointer of uint8_t to save registers values
 * @return UPM result
 */
static upm_result_t bh1792_read_registers(bh1792_context dev, uint8_t reg,
                                            uint8_t *data, uint8_t len);

/**
 * @brief Writes a value to a register
 *
 * @param dev The sensor context
 * @param value Value to write
 * @param reg Register address
 * @return UPM result
 */
static upm_result_t bh1792_write_register(bh1792_context dev, uint8_t value,
                                            uint8_t reg);

/**
 * @brief Sets a bit on in a register with a bit mask
 *
 * @param dev The sensor context
 * @param reg Register address
 * @param bit_mask Mask to use
 * @return UPM result
 */
static upm_result_t bh1792_set_bit_on(bh1792_context dev, uint8_t reg,
                                        uint8_t bit_mask);

/**
 * @brief Sets a bit off in a register with a bit mask
 *
 * @param dev The sensor context
 * @param reg Register address
 * @param bit_mask Mask to use
 * @return UPM result
 */
static upm_result_t bh1792_set_bit_off(bh1792_context dev, uint8_t reg,
                                        uint8_t bit_mask);

/**
 * @brief Sets the value of a register as a given pattern with a bit mask
 *
 * @param dev The sensor context
 * @param reg Register address
 * @param value Value to write
 * @param bit_mask Mask to use
 * @return UPM result
 */
static upm_result_t bh1792_set_bits_with_mask(bh1792_context dev, uint8_t reg,
                                            uint8_t value, uint8_t bit_mask);

/**
 * @brief Writes sync bit each 1 second. This function is used in a thread.
 *
 * @param dev The sensor context
 */
static void *bh1792_sync_measurement(void *args);

/**
 * @brief Sets the measurement time for synchronized mode
 *
 * @param dev The sensor context
 * @param meas_mode Measurement mode choice, take a value of MEAS_MODES
 * @return UPM result
 */
static upm_result_t bh1792_set_sync_meas_mode(bh1792_context dev, uint16_t meas_freq);

/**
 * @brief Selects the LED light type to use for measurement
 *
 * @param dev The sensor context
 * @param led_type Can take a value of GREEN or IR
 * @return UPM result
 */
static upm_result_t bh1792_select_led_type(bh1792_context dev, LED_TYPES led_type);

/**
 * @brief Sets the interrupt mode to be used, this is called when choosing
 * operation mode
 *
 * @param dev The sensor context
 * @param int_mode Interrupt mode, one of INTERRUPT_MODES values
 * @return UPM result
 */
static upm_result_t bh1792_set_interrupt_mode(bh1792_context dev, INTERRUPT_MODES int_mode);

/**
 * @brief Sets the RDY bit of the sensor
 *
 * @param dev The sensor context
 * @return UPM result
 */
static upm_result_t bh1792_set_rdy_bit(bh1792_context dev);


static upm_result_t bh1792_read_register(bh1792_context dev, uint8_t reg,
                                            uint8_t *data)
{
    if(!dev)
        return UPM_ERROR_OPERATION_FAILED;

    uint8_t value = mraa_i2c_read_byte_data(dev->i2c, reg);
    if(value < 0) {
        printf("%s: mraa_i2c_read_byte_data() failed\n", __FUNCTION__);
        return UPM_ERROR_OPERATION_FAILED;
    }

    *data = value;
    return UPM_SUCCESS;
}

static upm_result_t bh1792_read_registers(bh1792_context dev, uint8_t reg,
                                            uint8_t *data, uint8_t len)
{
    if(!dev)
        return UPM_ERROR_OPERATION_FAILED;

    uint8_t value;

    for(int i = 0; i < len; i++) {
        if(bh1792_read_register(dev, reg + i, &value) != UPM_SUCCESS) {
            return UPM_ERROR_OPERATION_FAILED;
        }

        *(data + i) = value;
    }

    return UPM_SUCCESS;
}

static upm_result_t bh1792_write_register(bh1792_context dev, uint8_t value,
                                            uint8_t reg)
{
    if(!dev)
        return UPM_ERROR_OPERATION_FAILED;

    if (mraa_i2c_write_byte_data(dev->i2c, value, reg) != MRAA_SUCCESS) {
        printf("%s: mraa_i2c_write_byte_data() failed\n", __FUNCTION__);
        return UPM_ERROR_OPERATION_FAILED;
    }

    return UPM_SUCCESS;
}

static upm_result_t bh1792_set_bit_on(bh1792_context dev, uint8_t reg,
                                        uint8_t bit_mask)
{
    if(!dev)
        return UPM_ERROR_OPERATION_FAILED;

    uint8_t reg_value;
    int status;

    status = bh1792_read_register(dev, reg, &reg_value);
    if(status < 0) {
        printf("%s: Failed to read register 0x%2X\n", __FUNCTION__, reg);
        return UPM_ERROR_OPERATION_FAILED;
    }

    reg_value |= bit_mask;
    return bh1792_write_register(dev, reg_value, reg);
}

static upm_result_t bh1792_set_bit_off(bh1792_context dev, uint8_t reg,
                                        uint8_t bit_mask)
{
    if(!dev)
        return UPM_ERROR_OPERATION_FAILED;

    uint8_t reg_value;
    int status;

    status = bh1792_read_register(dev, reg, &reg_value);
    if(status < 0) {
        printf("%s: Failed to read register 0x%2X\n", __FUNCTION__, reg);
        return UPM_ERROR_OPERATION_FAILED;
    }

    reg_value &= ~bit_mask;
    return bh1792_write_register(dev, reg_value, reg);
}

static upm_result_t bh1792_set_bits_with_mask(bh1792_context dev, uint8_t reg,
                                            uint8_t value, uint8_t bit_mask)
{
    if(!dev)
        return UPM_ERROR_OPERATION_FAILED;

    uint8_t reg_value;
    int status;

    status = bh1792_read_register(dev, reg, &reg_value);
    if(status < 0) {
        printf("%s: Failed to read register 0x%2X\n", __FUNCTION__, reg);
        return UPM_ERROR_OPERATION_FAILED;
    }

    reg_value &= ~bit_mask;
    reg_value |= value;
    return bh1792_write_register(dev, reg_value, reg);
}

bh1792_context bh1792_init(int bus, int addr)
{
    bh1792_context dev = (bh1792_context)malloc(sizeof(struct _bh1792_context));
    if (!dev)
        return NULL;

    dev->i2c = NULL;
    dev->interrupt = NULL;

    if (mraa_init() != MRAA_SUCCESS) {
        printf("%s: mraa_init() failed.\n", __FUNCTION__);
        bh1792_close(dev);
        return NULL;
    }

    if (!(dev->i2c = mraa_i2c_init(bus))) {
        printf("%s: mraa_i2c_init() failed.\n", __FUNCTION__);
        bh1792_close(dev);
        return NULL;
    }

    if (mraa_i2c_address(dev->i2c, addr)) {
        printf("%s: mraa_i2c_address() failed.\n", __FUNCTION__);
        bh1792_close(dev);
        return NULL;
    }

    if(bh1792_check_who_am_i(dev) != UPM_SUCCESS)
        return NULL;

    dev->enabled = false;
    dev->isrEnabled = false;
    dev->sync_thread_alive = false;

    return dev;
}

void bh1792_close(bh1792_context dev)
{
    if(dev) {
        bh1792_stop_measurement(dev);
        if(dev->isrEnabled)
            bh1792_remove_isr(dev);

        if (dev->i2c)
            mraa_i2c_stop(dev->i2c);

        if(dev->sync_thread_alive) {
            dev->sync_thread_alive = false;
            pthread_join(dev->sync_thread, NULL);
        }

        free(dev);
    }
}

upm_result_t bh1792_check_who_am_i(bh1792_context dev)
{
    if(!dev)
        return UPM_ERROR_OPERATION_FAILED;

    uint8_t who_am_i;
    if(bh1792_read_register(dev, BH1792_MANUFACTURER_REG, &who_am_i) != UPM_SUCCESS)
        return UPM_ERROR_OPERATION_FAILED;

    if(who_am_i != BH1792_MANUFACTURER_REG_MANUFACTURER_ID) {
        printf("%s: wrong manufacturer ID\n", __FUNCTION__);
        return UPM_ERROR_OPERATION_FAILED;
    }

    if(bh1792_read_register(dev, BH1792_PARTID_REG, &who_am_i) != UPM_SUCCESS)
        return UPM_ERROR_OPERATION_FAILED;

    if(who_am_i != BH1792_PARTID_REG_PART_ID) {
        printf("%s: wrong part ID\n", __FUNCTION__);
        return UPM_ERROR_OPERATION_FAILED;
    }

    return UPM_SUCCESS;
}

static upm_result_t bh1792_set_rdy_bit(bh1792_context dev)
{
    return bh1792_set_bit_on(dev, BH1792_MEAS_CONTROL1, BH1792_MEAS_CONTROL1_RDY);
}

bool bh1792_is_enabled(bh1792_context dev)
{
    if(!dev)
        return false;

    return dev->enabled;
}

upm_result_t bh1792_soft_reset(bh1792_context dev)
{
    return bh1792_set_bit_on(dev, BH1792_RESET, BH1792_RESET_SWRESET);
}

static upm_result_t bh1792_select_led_type(bh1792_context dev, LED_TYPES led_type)
{
    if(!dev)
        return UPM_ERROR_OPERATION_FAILED;

    uint8_t status;

    if(led_type == GREEN) {
        status = bh1792_set_bit_off(dev, BH1792_MEAS_CONTROL1,
                                    BH1792_MEAS_CONTROL1_SEL_ADC_MASK);
        if(status != UPM_SUCCESS)
            return UPM_ERROR_OPERATION_FAILED;

    } else {
        if(dev->op_mode == SYNCHRONIZED) {
            printf("%s: IR LED only for single & non-sync modes\n", __FUNCTION__);
            return UPM_ERROR_OPERATION_FAILED;
        }

        status = bh1792_set_bit_on(dev, BH1792_MEAS_CONTROL1,
                                BH1792_MEAS_CONTROL1_SEL_ADC_MASK);
        if(status != UPM_SUCCESS)
            return UPM_ERROR_OPERATION_FAILED;
    }

    dev->led_type = led_type;

    return UPM_SUCCESS;
}

static upm_result_t bh1792_set_sync_meas_mode(bh1792_context dev, uint16_t meas_freq)
{
    if(!dev)
        return UPM_ERROR_OPERATION_FAILED;

    uint8_t status;

    MEAS_MODES meas_mode;
    switch(meas_freq) {
        case 32:
            meas_mode = MSR_32Hz;
            break;
        case 64:
            meas_mode = MSR_64Hz;
            break;
        case 128:
            meas_mode = MSR_128Hz;
            break;
        case 256:
            meas_mode = MSR_256Hz;
            break;
        case 1024:
            meas_mode = MSR_1024Hz;
            break;
        default:
            return UPM_ERROR_INVALID_PARAMETER;
    }

    status = bh1792_set_bits_with_mask(dev, BH1792_MEAS_CONTROL1, meas_mode,
                                    BH1792_MEAS_CONTROL1_MSR_MASK);
    if(status != UPM_SUCCESS)
        return UPM_ERROR_OPERATION_FAILED;
    dev->meas_freq = meas_freq;

    return UPM_SUCCESS;
}

upm_result_t bh1792_get_meas_time_ms(bh1792_context dev, float *meas_time_ms)
{
    if(!dev)
        return UPM_ERROR_OPERATION_FAILED;

    uint8_t meas_time_reg;

    if(bh1792_read_register(dev, BH1792_MEAS_CONTROL1, &meas_time_reg) != UPM_SUCCESS)
        return UPM_ERROR_OPERATION_FAILED;
    switch(meas_time_reg & BH1792_MEAS_CONTROL1_MSR_MASK) {
        case MSR_32Hz:
            *meas_time_ms = 1000 / (float)32;
            break;
        case MSR_64Hz:
            *meas_time_ms = 1000 / (float)64;
            break;
        case MSR_128Hz:
            *meas_time_ms = 1000 / (float)128;
            break;
        case MSR_256Hz:
            *meas_time_ms = 1000 / (float)256;
            break;
        case MSR_1024Hz:
            *meas_time_ms = 1000 / (float)1024;
            break;
        case MSR_SINGLE:
        case MSR_NON_SYNC:
            *meas_time_ms = 1000 / (float)4;
            break;
        default:
            return UPM_ERROR_OPERATION_FAILED;
    }

    return UPM_SUCCESS;
}

upm_result_t bh1792_set_green_leds_current(bh1792_context dev, uint8_t current)
{
    if(!dev)
        return UPM_ERROR_OPERATION_FAILED;

    if(current > LED_CURRENT_MAX)
        return UPM_ERROR_OUT_OF_RANGE;

    uint8_t status;

    status = bh1792_set_bits_with_mask(dev, BH1792_MEAS_CONTROL2,
                    current & BH1792_MEAS_CONTROL2_LED_CURRENT1_MASK,
                    BH1792_MEAS_CONTROL2_LED_CURRENT1_MASK);
    if(status != UPM_SUCCESS)
        return UPM_ERROR_OPERATION_FAILED;
    dev->green_current = current;

    return UPM_SUCCESS;
}

upm_result_t bh1792_get_green_leds_current(bh1792_context dev, uint8_t *current)
{
    if(!dev)
        return UPM_ERROR_OPERATION_FAILED;

    uint8_t data;

    if(bh1792_read_register(dev, BH1792_MEAS_CONTROL2, &data) != UPM_SUCCESS)
        return UPM_ERROR_OPERATION_FAILED;

    *current = data & BH1792_MEAS_CONTROL2_LED_CURRENT1_MASK;
    return UPM_SUCCESS;
}

upm_result_t bh1792_set_ir_led_current(bh1792_context dev, uint8_t current)
{
    if(!dev)
        return UPM_ERROR_OPERATION_FAILED;

    if(current > LED_CURRENT_MAX)
        return UPM_ERROR_OUT_OF_RANGE;

    uint8_t status;

    status = bh1792_set_bits_with_mask(dev, BH1792_MEAS_CONTROL3,
                    current & BH1792_MEAS_CONTROL3_LED_CURRENT2_MASK,
                    BH1792_MEAS_CONTROL3_LED_CURRENT2_MASK);
    if(status != UPM_SUCCESS)
        return UPM_ERROR_OPERATION_FAILED;
    dev->ir_current = current;

    return UPM_SUCCESS;
}

upm_result_t bh1792_get_ir_led_current(bh1792_context dev, uint8_t *current)
{
    if(!dev)
        return UPM_ERROR_OPERATION_FAILED;

    uint8_t data;

    if(bh1792_read_register(dev, BH1792_MEAS_CONTROL3, &data) != UPM_SUCCESS)
        return UPM_ERROR_OPERATION_FAILED;

    *current = data & BH1792_MEAS_CONTROL3_LED_CURRENT2_MASK;
    return UPM_SUCCESS;
}

upm_result_t bh1792_set_ir_threshold(bh1792_context dev, uint16_t threshold)
{
    if(!dev)
        return UPM_ERROR_OPERATION_FAILED;

    if(bh1792_write_register(dev, threshold, BH1792_MEAS_CONTROL4_L) != UPM_SUCCESS ||
        bh1792_write_register(dev, threshold >> 8, BH1792_MEAS_CONTROL4_H) != UPM_SUCCESS) {
        printf("%s: Failed to write high threshold\n", __FUNCTION__);
        return UPM_ERROR_OPERATION_FAILED;
    }

    dev->threshold = threshold;

    return UPM_SUCCESS;
}

upm_result_t bh1792_get_ir_threshold(bh1792_context dev, uint16_t *threshold)
{
    if(!dev)
        return UPM_ERROR_OPERATION_FAILED;

    uint8_t data[2];

    if(bh1792_read_register(dev, BH1792_MEAS_CONTROL4_H, &data[1]) != UPM_SUCCESS ||
        bh1792_read_register(dev, BH1792_MEAS_CONTROL4_L, &data[0]) != UPM_SUCCESS)
        return UPM_ERROR_OPERATION_FAILED;

    *threshold = data[1] << 8 | data[0];
    return UPM_SUCCESS;
}

upm_result_t bh1792_get_fifo_size(bh1792_context dev, uint8_t *count)
{
    if(!dev)
        return UPM_ERROR_OPERATION_FAILED;

    uint8_t fifo_count;

    if(bh1792_read_register(dev, BH1792_FIFO_LEV, &fifo_count) != UPM_SUCCESS)
        return UPM_ERROR_OPERATION_FAILED;

    *count = fifo_count & BH1792_FIFO_LEV_LEVEL_MASK;
    return UPM_SUCCESS;
}

static upm_result_t bh1792_set_interrupt_mode(bh1792_context dev, INTERRUPT_MODES int_mode)
{
    if(!dev)
        return UPM_ERROR_OPERATION_FAILED;

    if(bh1792_set_bits_with_mask(dev, BH1792_MEAS_CONTROL5, int_mode,
                            BH1792_MEAS_CONTROL5_INT_SEL_MASK) != UPM_SUCCESS)
        return UPM_ERROR_OPERATION_FAILED;
    dev->interrupt_mode = int_mode;

    return UPM_SUCCESS;
}

upm_result_t bh1792_disable_interrupt(bh1792_context dev)
{
    return bh1792_set_bits_with_mask(dev, BH1792_MEAS_CONTROL5, 0,
                                BH1792_MEAS_CONTROL5_INT_SEL_MASK);
}

upm_result_t bh1792_clear_interrupt(bh1792_context dev)
{
    uint8_t data;
    return bh1792_read_register(dev, BH1792_INT_CLEAR, &data);
}

static void *bh1792_sync_measurement(void *args)
{
    bh1792_context dev = (bh1792_context)args;

    dev->sync_thread_alive = true;
    while(dev->sync_thread_alive) {
        bh1792_set_bit_on(dev, BH1792_MEAS_SYNC, BH1792_MEAS_SYNC_MEAS_SYNC);
        usleep(ONE_SEC_IN_MIRCO_SEC);
    }

    return (void *)UPM_SUCCESS;
}

upm_result_t bh1792_start_measurement(bh1792_context dev)
{
    if(!dev)
        return UPM_ERROR_OPERATION_FAILED;

    if(dev->op_mode == SYNCHRONIZED) {
        if(dev->sync_thread_alive) {
            dev->sync_thread_alive = false;
            pthread_join(dev->sync_thread, NULL);
        }
        pthread_create(&dev->sync_thread, NULL, &bh1792_sync_measurement, dev);
    }

    if(bh1792_set_bit_on(dev, BH1792_MEAS_START, BH1792_MEAS_START_MEAS_ST) != UPM_SUCCESS)
        return UPM_ERROR_OPERATION_FAILED;

    dev->enabled = true;

    return UPM_SUCCESS;
}

upm_result_t bh1792_stop_measurement(bh1792_context dev)
{
    if(!dev)
        return UPM_ERROR_OPERATION_FAILED;

    if(bh1792_soft_reset(dev) != UPM_SUCCESS)
        return UPM_SUCCESS;

    dev->enabled = false;

    return UPM_SUCCESS;
}

upm_result_t bh1792_restart_measurement(bh1792_context dev)
{
    if(!dev)
        return UPM_ERROR_OPERATION_FAILED;

    uint8_t status;

    status = bh1792_stop_measurement(dev);
    if(status != UPM_SUCCESS)
        return UPM_ERROR_OPERATION_FAILED;

    if(status != UPM_SUCCESS)
        return UPM_ERROR_OPERATION_FAILED;

    switch(dev->op_mode) {
        case SYNCHRONIZED:
            status = bh1792_enable_sync_mode(dev, dev->meas_freq, dev->green_current);
            break;
        case NON_SYNCHRONIZED:
            status = bh1792_enable_non_sync_mode(dev, dev->ir_current, dev->threshold);
            break;
        case SINGLE_GREEN:
            status = bh1792_enable_single_mode(dev, GREEN, dev->green_current);
            break;
        case SINGLE_IR:
            status = bh1792_enable_single_mode(dev, IR, dev->ir_current);
            break;
    }

    if(status != UPM_SUCCESS)
        return UPM_ERROR_OPERATION_FAILED;

    return bh1792_start_measurement(dev);
}

upm_result_t bh1792_get_fifo_data(bh1792_context dev, uint16_t fifo_led_off[],
                                    uint16_t fifo_led_on[])
{
    if(!dev)
        return UPM_ERROR_OPERATION_FAILED;

    if(dev->op_mode != SYNCHRONIZED)
        return UPM_ERROR_NO_DATA;

    uint8_t data[4];
    for(int i = 0; i < FIFO_WATERMARK; i++) {
        if(bh1792_read_registers(dev, BH1792_FIFO_DATA0_L, data, 4) != UPM_SUCCESS)
            return UPM_ERROR_OPERATION_FAILED;

        fifo_led_off[i] = data[1] << 8 | data[0];
        fifo_led_on[i] = data[3] << 8 | data[2];
    }

    return UPM_SUCCESS;
}

upm_result_t bh1792_discard_fifo_data(bh1792_context dev)
{
    if(!dev)
        return UPM_ERROR_OPERATION_FAILED;

    uint8_t data[4];

    for(int i = 0; i < FIFO_WATERMARK; i++)
        bh1792_read_registers(dev, BH1792_FIFO_DATA0_L, data, 4);

    return UPM_SUCCESS;
}

upm_result_t bh1792_get_green_data(bh1792_context dev, uint16_t *green_led_off,
                                        uint16_t *green_led_on)
{
    if(!dev)
        return UPM_ERROR_OPERATION_FAILED;

    if(dev->op_mode == NON_SYNCHRONIZED || dev->op_mode == SINGLE_IR)
        return UPM_ERROR_NO_DATA;

    uint8_t data[4];

    if(bh1792_read_registers(dev, BH1792_DATAOUT_LEDOFF_L, data, 4) != UPM_SUCCESS)
        return UPM_ERROR_OPERATION_FAILED;

    *green_led_off = data[1] << 8 | data[0];
    *green_led_on = data[3] << 8 | data[2];

    return UPM_SUCCESS;
}

upm_result_t bh1792_get_ir_data(bh1792_context dev, uint16_t *ir_led_off,
                                        uint16_t *ir_led_on)
{
    if(!dev)
        return UPM_ERROR_OPERATION_FAILED;

    if(dev->op_mode != NON_SYNCHRONIZED && dev->op_mode != SINGLE_IR)
        return UPM_ERROR_NO_DATA;

    uint8_t data[4];

    if(bh1792_read_registers(dev, BH1792_IRDATA_LEDOFF_L, data, 4) != UPM_SUCCESS)
        return UPM_ERROR_OPERATION_FAILED;

    *ir_led_off = data[1] << 8 | data[0];
    *ir_led_on = data[3] << 8 | data[2];

    return UPM_SUCCESS;
}

upm_result_t bh1792_enable_sync_mode(bh1792_context dev, uint16_t meas_freq,
                                        uint8_t green_current)
{
    if(!dev)
        return UPM_ERROR_OPERATION_FAILED;

    uint8_t status;

    if(bh1792_set_rdy_bit(dev) != UPM_SUCCESS)
        return UPM_ERROR_OPERATION_FAILED;

    if(bh1792_set_green_leds_current(dev, green_current) != UPM_SUCCESS)
        return UPM_ERROR_OPERATION_FAILED;

    if(bh1792_select_led_type(dev, GREEN) != UPM_SUCCESS)
        return UPM_ERROR_OPERATION_FAILED;

    status = bh1792_set_sync_meas_mode(dev, meas_freq);
    if(status != UPM_SUCCESS)
        return status;

    dev->op_mode = SYNCHRONIZED;
    return bh1792_set_interrupt_mode(dev, WATER_MARK);
}

upm_result_t bh1792_enable_non_sync_mode(bh1792_context dev, uint8_t ir_current,
                                        uint16_t threshold)
{
    if(!dev)
        return UPM_ERROR_OPERATION_FAILED;

    if(bh1792_set_rdy_bit(dev) != UPM_SUCCESS)
        return UPM_ERROR_OPERATION_FAILED;

    if(bh1792_set_ir_led_current(dev, ir_current) != UPM_SUCCESS)
        return UPM_ERROR_OPERATION_FAILED;

    if(bh1792_select_led_type(dev, IR) != UPM_SUCCESS)
        return UPM_ERROR_OPERATION_FAILED;
    if(bh1792_set_sync_meas_mode(dev, MSR_NON_SYNC) != UPM_SUCCESS)
        return UPM_ERROR_OPERATION_FAILED;

    dev->op_mode = NON_SYNCHRONIZED;
    return bh1792_set_interrupt_mode(dev, IR_THRESHOLD_JUDGE);
}

upm_result_t bh1792_enable_single_mode(bh1792_context dev, LED_TYPES led_type,
                                        uint8_t current)
{
    if(!dev)
        return UPM_ERROR_OPERATION_FAILED;

    if(bh1792_set_rdy_bit(dev) != UPM_SUCCESS)
        return UPM_ERROR_OPERATION_FAILED;

    if(led_type == GREEN) {
        if(bh1792_set_green_leds_current(dev, current) != UPM_SUCCESS)
            return UPM_ERROR_OPERATION_FAILED;
    } else {
        if(bh1792_set_ir_led_current(dev, current) != UPM_SUCCESS)
            return UPM_ERROR_OPERATION_FAILED;
    }

    if(bh1792_select_led_type(dev, led_type) != UPM_SUCCESS)
        return UPM_ERROR_OPERATION_FAILED;

    if(bh1792_set_sync_meas_mode(dev, MSR_SINGLE) != UPM_SUCCESS)
        return UPM_ERROR_OPERATION_FAILED;

    if(led_type == GREEN)
        dev->op_mode = SINGLE_GREEN;
    else
        dev->op_mode = SINGLE_IR;

    return bh1792_set_interrupt_mode(dev, ON_COMPLETE);
}

upm_result_t bh1792_install_isr(bh1792_context dev, mraa_gpio_edge_t edge,
                                int pin, void (*isr)(void *), void *isr_args)
{
    if(!dev)
        return UPM_ERROR_OPERATION_FAILED;

    if(dev->isrEnabled)
        bh1792_remove_isr(dev);

    mraa_gpio_context isr_gpio = NULL;

    if (!(isr_gpio = mraa_gpio_init(pin))) {
        printf("%s: mraa_gpio_init() failed.\n", __FUNCTION__);
        return UPM_ERROR_OPERATION_FAILED;
    }

    mraa_gpio_dir(isr_gpio, MRAA_GPIO_IN);

    if (mraa_gpio_isr(isr_gpio, edge, isr, isr_args) != MRAA_SUCCESS) {
        mraa_gpio_close(isr_gpio);
        printf("%s: mraa_gpio_isr() failed.\n", __FUNCTION__);
        return UPM_ERROR_OPERATION_FAILED;
    }

    dev->interrupt = isr_gpio;
    dev->isrEnabled = true;

    return UPM_SUCCESS;
}

upm_result_t bh1792_install_isr_falling_edge(bh1792_context dev, int pin,
                                void (*isr)(void *), void *isr_args)
{
    return bh1792_install_isr(dev, MRAA_GPIO_EDGE_FALLING, pin, isr, isr_args);
}

void bh1792_remove_isr(bh1792_context dev)
{
    if(dev) {
        mraa_gpio_isr_exit(dev->interrupt);
        mraa_gpio_close(dev->interrupt);
        dev->interrupt = NULL;
        dev->isrEnabled = false;
    }
}

upm_result_t bh1792_registers_dump(bh1792_context dev, char *dump)
{
    if(!dev)
        return UPM_ERROR_OPERATION_FAILED;

    uint8_t reg_values[10];
    int count = 0;
    int len = 8;

    if(bh1792_read_registers(dev, BH1792_MEAS_CONTROL1, reg_values, len) != UPM_SUCCESS)
        return UPM_ERROR_OPERATION_FAILED;
    count += snprintf(dump, 6, "0x41 ");
    for(int i = 0; i < len; i++)
        count += snprintf(dump + count, 4, "%02X ", reg_values[i]);
    snprintf(dump + count - 1, 3, "\n");

    len = 1;
    if(bh1792_read_registers(dev, BH1792_FIFO_LEV, reg_values, len) != UPM_SUCCESS)
        return UPM_ERROR_OPERATION_FAILED;
    count += snprintf(dump + count, 6, "0x4B ");
    for(int i = 0; i < len; i++)
        count += snprintf(dump + count, 4, "%02X ", reg_values[i]);

    return UPM_SUCCESS;
}
