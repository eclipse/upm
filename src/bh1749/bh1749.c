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
#include "bh1749.h"

#define DEFAULT_OP_MODE INT_JUDGE_1
#define DEFAULT_MEAS_TIME MEAS_240MS
#define DEFAULT_RGB_GAIN RGB_GAIN_1X
#define DEFAULT_IR_GAIN IR_GAIN_1X
#define DEFAULT_INT_SOURCE RED
#define DEFUALT_THRESHOLD_HIGH 511

/**
 * @brief Reads the value of a register
 *
 * @param dev The sensor context
 * @param reg Register address
 * @param data Pointer of uint8_t to save register value
 * @return UPM result
 */
static upm_result_t bh1749_read_register(bh1749_context dev, uint8_t reg, uint8_t *data)
{
    uint8_t value = mraa_i2c_read_byte_data(dev->i2c, reg);
    if(value < 0) {
        printf("%s: mraa_i2c_read_byte_data() failed\n", __FUNCTION__);
        return UPM_ERROR_OPERATION_FAILED;
    }

    *data = value;
    return UPM_SUCCESS;
}

/**
 * @brief Reads the values of a set of registers
 *
 * @param dev The sensor context
 * @param reg Register address
 * @param data Pointer of uint8_t to save registers values
 * @return UPM result
 */
static upm_result_t bh1749_read_registers(bh1749_context dev, uint8_t reg, uint8_t *data, uint8_t len)
{
    uint8_t value;

    for(int i = 0; i < len; i++) {
        if(bh1749_read_register(dev, reg + i, &value) != UPM_SUCCESS) {
            return UPM_ERROR_OPERATION_FAILED;
        }

        *(data + i) = value;
    }

    return UPM_SUCCESS;
}

/**
 * @brief Writes a value to a register
 *
 * @param dev The sensor context
 * @param value Value to write
 * @param reg Register address
 * @return UPM result
 */
static upm_result_t bh1749_write_register(bh1749_context dev, uint8_t value, uint8_t reg)
{
    if (mraa_i2c_write_byte_data(dev->i2c, value, reg) != MRAA_SUCCESS) {
        printf("%s: mraa_i2c_write_byte_data() failed\n", __FUNCTION__);
        return UPM_ERROR_OPERATION_FAILED;
    }

    return UPM_SUCCESS;
}

/**
 * @brief Sets a bit on in a register with a bit mask
 *
 * @param dev The sensor context
 * @param reg Register address
 * @param bit_mask Mask to use
 * @return UPM result
 */
static upm_result_t bh1749_set_bit_on(bh1749_context dev, uint8_t reg, uint8_t bit_mask)
{
    uint8_t reg_value;
    int status;

    status = bh1749_read_register(dev, reg, &reg_value);
    if(status < 0) {
        printf("%s: Failed to read register 0x%2X\n", __FUNCTION__, reg);
        return UPM_ERROR_OPERATION_FAILED;
    }

    reg_value |= bit_mask;
    return bh1749_write_register(dev, reg_value, reg);
}

/**
 * @brief Sets a bit off in a register with a bit mask
 *
 * @param dev The sensor context
 * @param reg Register address
 * @param bit_mask Mask to use
 * @return UPM result
 */
static upm_result_t bh1749_set_bit_off(bh1749_context dev, uint8_t reg, uint8_t bit_mask)
{
    uint8_t reg_value;
    int status;

    status = bh1749_read_register(dev, reg, &reg_value);
    if(status < 0) {
        printf("%s: Failed to read register 0x%2X\n", __FUNCTION__, reg);
        return UPM_ERROR_OPERATION_FAILED;
    }

    reg_value &= ~bit_mask;
    return bh1749_write_register(dev, reg_value, reg);
}

/**
 * @brief Sets the value of a register as a given pattern with a bit mask
 *
 * @param dev The sensor context
 * @param reg Register address
 * @param value Value to write
 * @param bit_mask Mask to use
 * @return UPM result
 */
static upm_result_t bh1749_set_bits_with_mask(bh1749_context dev, uint8_t reg, uint8_t value, uint8_t bit_mask)
{
    uint8_t reg_value;
    int status;

    status = bh1749_read_register(dev, reg, &reg_value);
    if(status < 0) {
        printf("%s: Failed to read register 0x%2X\n", __FUNCTION__, reg);
        return UPM_ERROR_OPERATION_FAILED;
    }

    reg_value &= ~bit_mask;
    reg_value |= value;
    return bh1749_write_register(dev, reg_value, reg);
}

upm_result_t bh1749_check_who_am_i(bh1749_context dev)
{
    uint8_t partId;
    if(bh1749_read_register(dev, BH1749_SYSTEM_CONTROL, &partId) != UPM_SUCCESS)
        return UPM_ERROR_OPERATION_FAILED;

    partId &= BH1749_SYSTEM_CONTROL_PART_MASK;
    if(partId != BH1749_SYSTEM_CONTROL_PART_ID) {
        printf("%s: wrong manufacturer ID\n", __FUNCTION__);
        return UPM_ERROR_OPERATION_FAILED;
    }

    return UPM_SUCCESS;
}

bh1749_context bh1749_init(int bus, int addr)
{
    bh1749_context dev = (bh1749_context)malloc(sizeof(struct _bh1749_context));
    if (!dev)
        return NULL;

    dev->i2c = NULL;
    dev->interrupt = NULL;

    if (mraa_init() != MRAA_SUCCESS) {
        printf("%s: mraa_init() failed.\n", __FUNCTION__);
        bh1749_close(dev);
        return NULL;
    }

    if (!(dev->i2c = mraa_i2c_init(bus))) {
        printf("%s: mraa_i2c_init() failed.\n", __FUNCTION__);
        bh1749_close(dev);
        return NULL;
    }

    if (mraa_i2c_address(dev->i2c, addr)) {
        printf("%s: mraa_i2c_address() failed.\n", __FUNCTION__);
        bh1749_close(dev);
        return NULL;
    }

    if(bh1749_check_who_am_i(dev) != UPM_SUCCESS)
        return NULL;

    dev->enabled = false;
    dev->isrEnabled = false;
    if(bh1749_sensor_init(dev, DEFAULT_OP_MODE, DEFAULT_MEAS_TIME,
        DEFAULT_RGB_GAIN, DEFAULT_IR_GAIN, DEFAULT_INT_SOURCE) != UPM_SUCCESS)
        return NULL;
    bh1749_set_threshold_high(dev, DEFUALT_THRESHOLD_HIGH);

    return dev;
}

void bh1749_close(bh1749_context dev)
{
    if(dev->isrEnabled)
        bh1749_remove_isr(dev);

    if (dev->i2c)
        mraa_i2c_stop(dev->i2c);

    free(dev);
}

upm_result_t bh1749_enable(bh1749_context dev)
{
    int status;

    status = bh1749_set_bit_on(dev, BH1749_MODE_CONTROL2,
                BH1749_MODE_CONTROL2_RGB_MEASUREMENT_MASK);
    if(status != UPM_SUCCESS) {
        printf("%s: Failed to bh1749_enable RGB measurement\n", __FUNCTION__);
        return UPM_ERROR_OPERATION_FAILED;
    }
    dev->enabled = true;

    return UPM_SUCCESS;
}

upm_result_t bh1749_disable(bh1749_context dev)
{
    int status;

    status = bh1749_set_bit_off(dev, BH1749_MODE_CONTROL2,
                BH1749_MODE_CONTROL2_RGB_MEASUREMENT_MASK);
    if(status != UPM_SUCCESS) {
        printf("%s: Failed to bh1749_disable RGB measurement\n", __FUNCTION__);
        return UPM_ERROR_OPERATION_FAILED;
    }
    dev->enabled = false;

    return UPM_SUCCESS;
}

upm_result_t bh1749_sensor_init(bh1749_context dev, OPERATING_MODES opMode,
                        MEAS_TIMES measTime,
                        RGB_GAINS rgbGain,
                        IR_GAINS irGain,
                        INT_SOURCES intSource)
{
    if(bh1749_set_operating_mode(dev, opMode) != UPM_SUCCESS)
        return UPM_ERROR_OPERATION_FAILED;
    if(bh1749_set_measurement_time(dev, measTime) != UPM_SUCCESS)
        return UPM_ERROR_OPERATION_FAILED;
    if(bh1749_set_rgb_gain(dev, rgbGain) != UPM_SUCCESS)
        return UPM_ERROR_OPERATION_FAILED;
    if(bh1749_set_ir_gain(dev, irGain) != UPM_SUCCESS)
        return UPM_ERROR_OPERATION_FAILED;
    return bh1749_set_int_source(dev, intSource);
}

upm_result_t bh1749_set_operating_mode(bh1749_context dev, OPERATING_MODES opMode)
{
    if(bh1749_write_register(dev, opMode, BH1749_PERSISTENCE) != UPM_SUCCESS) {
        printf("%s: Setting operating mode failed\n", __FUNCTION__);
        return UPM_ERROR_OPERATION_FAILED;
    }
    dev->operating_mode = opMode;

    return UPM_SUCCESS;
}

upm_result_t bh1749_get_operating_mode(bh1749_context dev, uint8_t *opMode)
{
    uint8_t value;

    if(bh1749_read_register(dev, BH1749_PERSISTENCE, &value) != UPM_SUCCESS) {
        printf("%s: Failed to read operating mode\n", __FUNCTION__);
        return UPM_ERROR_OPERATION_FAILED;
    }

    value &= BH1749_PERSISTENCE_MODE_MASK;
    if(value != INT_JUDGE_0 && value != INT_JUDGE_1 &&
        value != INT_JUDGE_4 && value != INT_JUDGE_4) {
            printf("%s: Returned invalid mode\n", __FUNCTION__);
            return UPM_ERROR_OPERATION_FAILED;
    }

    *opMode = value;

    return UPM_SUCCESS;
}

upm_result_t bh1749_set_measurement_time(bh1749_context dev, MEAS_TIMES measTime)
{
    if(bh1749_set_bits_with_mask(dev, BH1749_MODE_CONTROL1, measTime,
                        BH1749_MODE_CONTROL1_ODR_MASK) != UPM_SUCCESS) {
        printf("%s: Failed setting measurement time\n", __FUNCTION__);
        return UPM_ERROR_OPERATION_FAILED;
    }
    dev->meas_time = measTime;

    return UPM_SUCCESS;
}

upm_result_t bh1749_get_measurement_time(bh1749_context dev, uint8_t *meas_time)
{
    uint8_t time_reg;

    if(bh1749_read_register(dev, BH1749_MODE_CONTROL1, &time_reg) != UPM_SUCCESS) {
        printf("%s: Failed to read measurement time\n", __FUNCTION__);
        return UPM_ERROR_OPERATION_FAILED;
    }

    time_reg &= BH1749_MODE_CONTROL1_ODR_MASK;
    if(time_reg == MEAS_35MS)
        *meas_time = 35;
    else if(time_reg == MEAS_120MS)
        *meas_time = 120;
    else if(time_reg == MEAS_240MS)
        *meas_time = 240;
    else {
        printf("%s: Returned invalid time\n", __FUNCTION__);
        return UPM_ERROR_OPERATION_FAILED;
    }

    return UPM_SUCCESS;
}

upm_result_t bh1749_set_rgb_gain(bh1749_context dev, RGB_GAINS rgbGain)
{
    if(bh1749_set_bits_with_mask(dev, BH1749_MODE_CONTROL1, rgbGain,
                        BH1749_MODE_CONTROL1_RGB_GAIN_MASK) != UPM_SUCCESS) {
        printf("%s: Failed setting RGB gain\n", __FUNCTION__);
        return UPM_ERROR_OPERATION_FAILED;
    }
    dev->rgb_gain = rgbGain;

    return UPM_SUCCESS;
}

upm_result_t bh1749_get_rgb_gain(bh1749_context dev, uint8_t *gain)
{
    uint8_t rgb_gain;

    if(bh1749_read_register(dev, BH1749_MODE_CONTROL1, &rgb_gain) != UPM_SUCCESS) {
        printf("%s: Failed to read rgb gain\n", __FUNCTION__);
        return UPM_ERROR_OPERATION_FAILED;
    }

    rgb_gain &= BH1749_MODE_CONTROL1_RGB_GAIN_MASK;
    if(rgb_gain == RGB_GAIN_1X)
        *gain = 1;
    else if(rgb_gain == RGB_GAIN_32X)
        *gain = 32;
    else {
        printf("%s: Returned invalid gain\n", __FUNCTION__);
        return UPM_ERROR_OPERATION_FAILED;
    }

    return UPM_SUCCESS;
}

upm_result_t bh1749_set_ir_gain(bh1749_context dev, IR_GAINS irGain)
{
    if(bh1749_set_bits_with_mask(dev, BH1749_MODE_CONTROL1, irGain,
                        BH1749_MODE_CONTROL1_IR_GAIN_MASK) != UPM_SUCCESS) {
        printf("%s: Failed setting IR gain\n", __FUNCTION__);
        return UPM_ERROR_OPERATION_FAILED;
    }
    dev->ir_gain = irGain;

    return UPM_SUCCESS;
}

upm_result_t bh1749_get_ir_gain(bh1749_context dev, uint8_t *gain)
{
    uint8_t ir_gain;

    if(bh1749_read_register(dev, BH1749_MODE_CONTROL1, &ir_gain) != UPM_SUCCESS) {
        printf("%s: Failed to read rgb gain\n", __FUNCTION__);
        return UPM_ERROR_OPERATION_FAILED;
    }

    ir_gain &= BH1749_MODE_CONTROL1_IR_GAIN_MASK;
    if(ir_gain == IR_GAIN_1X)
        *gain = 1;
    else if(ir_gain == IR_GAIN_32X)
        *gain = 32;
    else {
        printf("%s: Returned invalid gain\n", __FUNCTION__);
        return UPM_ERROR_OPERATION_FAILED;
    }

    return UPM_SUCCESS;
}

upm_result_t bh1749_set_int_source(bh1749_context dev, INT_SOURCES intSource)
{
    if(bh1749_set_bits_with_mask(dev, BH1749_INTERRUPT, intSource,
                        BH1749_INTERRUPT_SOURCE_MASK) != UPM_SUCCESS) {
        printf("%s: Failed setting interrupt source gain\n", __FUNCTION__);
        return UPM_ERROR_OPERATION_FAILED;
    }
    dev->int_src = intSource;

    return UPM_SUCCESS;
}

char bh1749_get_interrupt_source_char(bh1749_context dev)
{
    char intSourceChar = ' ';
    uint8_t intSource;

    if(bh1749_read_register(dev, BH1749_INTERRUPT, &intSource) != UPM_SUCCESS) {
        printf("%s: Failed to read interrupt source\n", __FUNCTION__);
        return intSourceChar;
    }

    intSource &= BH1749_INTERRUPT_SOURCE_MASK;
    if(intSource == RED)
        intSourceChar = 'r';
    else if(intSource == GREEN)
        intSourceChar = 'g';
    else if(intSource == BLUE)
        intSourceChar = 'b';
    else {
        printf("%s: Returned invalid interrupt source\n", __FUNCTION__);
        return intSourceChar;
    }

    return intSourceChar;
}

upm_result_t bh1749_enable_interrupt(bh1749_context dev)
{
    if(bh1749_set_bit_on(dev, BH1749_INTERRUPT, BH1749_INTERRUPT_EN_MASK) != UPM_SUCCESS)
        return UPM_ERROR_OPERATION_FAILED;
    return bh1749_reset_interrupt(dev);
}

upm_result_t bh1749_disable_interrupt(bh1749_context dev)
{
    return bh1749_set_bit_off(dev, BH1749_INTERRUPT, BH1749_INTERRUPT_EN_MASK);
}

upm_result_t bh1749_reset_interrupt(bh1749_context dev)
{
    return bh1749_set_bit_on(dev, BH1749_SYSTEM_CONTROL, BH1749_SYSTEM_CONTROL_INT_RESET);
}

bool bh1749_is_interrupted(bh1749_context dev)
{
    uint8_t intStatus;

    if(bh1749_read_register(dev, BH1749_INTERRUPT, &intStatus) != UPM_SUCCESS) {
        printf("%s: Failed to read interrupt status\n", __FUNCTION__);
        return false;
    }

    intStatus &= BH1749_INTERRUPT_STATUS_MASK;
    if(intStatus != BH1749_INTERRUPT_STATUS_ACTIVE)
            return false;

    return true;
}

bool bh1749_is_interrupt_enabled(bh1749_context dev)
{
    uint8_t intStatus;

    if(bh1749_read_register(dev, BH1749_INTERRUPT, &intStatus) != UPM_SUCCESS) {
        printf("%s: Failed to read interrupt enabled\n", __FUNCTION__);
        return false;
    }

    intStatus &= BH1749_INTERRUPT_EN_MASK;
    if(intStatus != BH1749_INTERRUPT_EN_ENABLE)
        return false;

    return true;
}

upm_result_t bh1749_soft_reset(bh1749_context dev)
{
    return bh1749_set_bit_on(dev, BH1749_SYSTEM_CONTROL, BH1749_SYSTEM_CONTROL_SW_RESET_MASK);
}

upm_result_t bh1749_set_threshold_high(bh1749_context dev, uint16_t threshold)
{
    if(bh1749_write_register(dev, threshold, BH1749_TH_LSBS) != UPM_SUCCESS ||
        bh1749_write_register(dev, threshold >> 8, BH1749_TH_MSBS) != UPM_SUCCESS) {
        printf("%s: Failed to write high threshold\n", __FUNCTION__);
        return UPM_ERROR_OPERATION_FAILED;
    }
    dev->int_thh = threshold;

    return UPM_SUCCESS;
}

upm_result_t bh1749_get_threshold_high(bh1749_context dev, uint16_t *threshold)
{
    uint8_t data[2];

    if(bh1749_read_register(dev, BH1749_TH_MSBS, &data[0]) ||
        bh1749_read_register(dev, BH1749_TH_LSBS, &data[1]) != UPM_SUCCESS)
        return UPM_ERROR_OPERATION_FAILED;

    *threshold = data[0] << 8 | data[1];
    return UPM_SUCCESS;
}

upm_result_t bh1749_set_threshold_low(bh1749_context dev, uint16_t threshold)
{
    if(bh1749_write_register(dev, threshold, BH1749_TL_LSBS) != UPM_SUCCESS ||
        bh1749_write_register(dev, threshold >> 8, BH1749_TL_MSBS) != UPM_SUCCESS) {
        printf("%s: Failed to write low threshold\n", __FUNCTION__);
        return UPM_ERROR_OPERATION_FAILED;
    }
    dev->int_thl = threshold;

    return UPM_SUCCESS;
}

upm_result_t bh1749_get_threshold_low(bh1749_context dev, uint16_t *threshold)
{

    uint8_t data[2];

    if(bh1749_read_register(dev, BH1749_TL_MSBS, &data[0]) ||
        bh1749_read_register(dev, BH1749_TL_LSBS, &data[1]) != UPM_SUCCESS)
        return UPM_ERROR_OPERATION_FAILED;

    *threshold = data[0] << 8 | data[1];
    return UPM_SUCCESS;
}

upm_result_t bh1749_get_red(bh1749_context dev, uint16_t *red)
{
    uint8_t data[2];

    if(bh1749_read_register(dev, BH1749_RED_DATA_MSBS, &data[0]) != UPM_SUCCESS ||
        bh1749_read_register(dev, BH1749_RED_DATA_LSBS, &data[1]) != UPM_SUCCESS)
        return -UPM_ERROR_OPERATION_FAILED;

    *red = data[0] << 8 | data[1];


    return UPM_SUCCESS;
}

upm_result_t bh1749_get_green(bh1749_context dev, uint16_t *green)
{
    uint8_t data[2];

    if(bh1749_read_register(dev, BH1749_GREEN_DATA_MSBS, &data[0]) != UPM_SUCCESS ||
        bh1749_read_register(dev, BH1749_GREEN_DATA_LSBS, &data[1]) != UPM_SUCCESS)
        return -UPM_ERROR_OPERATION_FAILED;

    *green = data[0] << 8 | data[1];

    return UPM_SUCCESS;
}

upm_result_t bh1749_get_blue(bh1749_context dev, uint16_t *blue)
{
    uint8_t data[2];

    if(bh1749_read_register(dev, BH1749_BLUE_DATA_MSBS, &data[0]) != UPM_SUCCESS ||
        bh1749_read_register(dev, BH1749_BLUE_DATA_LSBS, &data[1]) != UPM_SUCCESS)
        return -UPM_ERROR_OPERATION_FAILED;

    *blue = data[0] << 8 | data[1];

    return UPM_SUCCESS;
}

upm_result_t bh1749_get_ir(bh1749_context dev, uint16_t *ir)
{
    uint8_t data[2];

    if(bh1749_read_register(dev, BH1749_IR_DATA_MSBS, &data[0]) != UPM_SUCCESS ||
        bh1749_read_register(dev, BH1749_IR_DATA_LSBS, &data[1]) != UPM_SUCCESS)
        return -UPM_ERROR_OPERATION_FAILED;

    *ir = data[0] << 8 | data[1];

    return UPM_SUCCESS;
}

upm_result_t bh1749_get_green2(bh1749_context dev, uint16_t *green2)
{
    uint8_t data[2];

    if(bh1749_read_register(dev, BH1749_GREEN2_DATA_MSBS, &data[0]) != UPM_SUCCESS ||
        bh1749_read_register(dev, BH1749_GREEN2_DATA_LSBS, &data[1]) != UPM_SUCCESS)
        return -UPM_ERROR_OPERATION_FAILED;

    *green2 = data[0] << 8 | data[1];

    return UPM_SUCCESS;
}

upm_result_t bh1749_get_measurements(bh1749_context dev, uint16_t *result)
{
    uint16_t value;
    int step = 0;

    if(bh1749_get_red(dev, &value) != UPM_SUCCESS)
        return UPM_ERROR_OPERATION_FAILED;
    *(result + step++) = value;
    if(bh1749_get_green(dev, &value) != UPM_SUCCESS)
        return UPM_ERROR_OPERATION_FAILED;
    *(result + step++) = value;
    if(bh1749_get_blue(dev, &value) != UPM_SUCCESS)
        return UPM_ERROR_OPERATION_FAILED;
    *(result + step++) = value;
    if(bh1749_get_ir(dev, &value) != UPM_SUCCESS)
        return UPM_ERROR_OPERATION_FAILED;
    *(result + step++) = value;
    if(bh1749_get_green2(dev, &value) != UPM_SUCCESS)
        return UPM_ERROR_OPERATION_FAILED;
    *(result + step++) = value;

    return UPM_SUCCESS;
}

upm_result_t bh1749_install_isr(bh1749_context dev, mraa_gpio_edge_t edge, int pin,
                        void (*isr)(void *), void *isr_args)
{
    if(dev->isrEnabled)
        bh1749_remove_isr(dev);

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

void bh1749_remove_isr(bh1749_context dev)
{
    mraa_gpio_isr_exit(dev->interrupt);
    mraa_gpio_close(dev->interrupt);
    dev->interrupt = NULL;
    dev->isrEnabled = false;
}

upm_result_t bh1749_registers_dump(bh1749_context dev, char *dump)
{
    uint8_t reg_values[10];
    int count = 0;
    int len = 3;

    if(bh1749_read_registers(dev, BH1749_SYSTEM_CONTROL, reg_values, len) != UPM_SUCCESS)
        return UPM_ERROR_OPERATION_FAILED;
    count += snprintf(dump, 6, "0x40 ");
    for(int i = 0; i < len; i++)
        count += snprintf(dump + count, 4, "%02X ", reg_values[i]);
    snprintf(dump + count - 1, 3, "\n");

    len = 6;
    if(bh1749_read_registers(dev, BH1749_INTERRUPT, reg_values, len) != UPM_SUCCESS)
        return UPM_ERROR_OPERATION_FAILED;
    count += snprintf(dump + count, 6, "0x60 ");
    for(int i = 0; i < len; i++)
        count += snprintf(dump + count, 4, "%02X ", reg_values[i]);

    return UPM_SUCCESS;
}
