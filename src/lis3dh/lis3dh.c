/*
 * Author: Alex Tereschenko <alext.mkrs@gmail.com>
 * Copyright (c) 2018 Alex Tereschenko.
 *
 * Based on LIS2DS12 module by
 * Author: Jon Trulson <jtrulson@ics.com>
 * Copyright (c) 2017 Intel Corporation.
 *
 * The MIT License
 *
 * This program and the accompanying materials are made available under the
 * terms of the The MIT License which is available at
 * https://opensource.org/licenses/MIT.
 *
 * SPDX-License-Identifier: MIT
 */

#include <assert.h>
#include <unistd.h>

#include "lis3dh.h"
#include "upm_utilities.h"

// Macro for converting a uint8_t low/high pair into a float
#define INT16_TO_FLOAT(h, l) (float) ((int16_t)((l) | ((h) << 8)))

// Some useful macros to save on typing and text wrapping
#undef _SHIFT
#define _SHIFT(x) (_LIS3DH_##x##_SHIFT)

#undef _MASK
#define _MASK(x) (_LIS3DH_##x##_MASK)

#undef _SHIFTMASK
#define _SHIFTMASK(x) (_MASK(x) << _SHIFT(x))

// SPI CS on and off functions
static void
_csOn(const lis3dh_context dev)
{
    assert(dev != NULL);

    if (dev->gpioCS) {
        mraa_gpio_write(dev->gpioCS, 0);
    }
}

static void
_csOff(const lis3dh_context dev)
{
    assert(dev != NULL);

    if (dev->gpioCS) {
        mraa_gpio_write(dev->gpioCS, 1);
    }
}

// Init
lis3dh_context
lis3dh_init(int bus, int addr, int cs)
{
    lis3dh_context dev = (lis3dh_context) malloc(sizeof(struct _lis3dh_context));

    if (!dev) {
        return NULL;
    }

    // Zero out context
    memset((void*) dev, 0, sizeof(struct _lis3dh_context));

    // Make sure MRAA is initialized
    if (mraa_init() != MRAA_SUCCESS) {
        printf("%s: mraa_init() failed\n", __FUNCTION__);
        lis3dh_close(dev);
        return NULL;
    }

    if (addr < 0) {
        // SPI
        if (!(dev->spi = mraa_spi_init(bus))) {
            printf("%s: mraa_spi_init() for bus %d failed\n", __FUNCTION__, bus);
            lis3dh_close(dev);
            return NULL;
        }

        // Only create CS context if we are actually using a valid pin.
        // A hardware controlled pin should specify CS as -1.
        if (cs >= 0) {
            if (!(dev->gpioCS = mraa_gpio_init(cs))) {
                printf("%s: mraa_gpio_init() for CS pin %d failed\n", __FUNCTION__, cs);
                lis3dh_close(dev);
                return NULL;
            }
            mraa_gpio_dir(dev->gpioCS, MRAA_GPIO_OUT);
        }

        mraa_spi_mode(dev->spi, MRAA_SPI_MODE0);
        if (mraa_spi_frequency(dev->spi, 5000000)) {
            printf("%s: mraa_spi_frequency() failed\n", __FUNCTION__);
            lis3dh_close(dev);
            return NULL;
        }
    } else {
        // I2C
        if (!(dev->i2c = mraa_i2c_init(bus))) {
            printf("%s: mraa_i2c_init() for bus %d failed\n", __FUNCTION__, bus);
            lis3dh_close(dev);
            return NULL;
        }

        if (mraa_i2c_address(dev->i2c, addr)) {
            printf("%s: mraa_i2c_address() for address 0x%x failed\n", __FUNCTION__, addr);
            lis3dh_close(dev);
            return NULL;
        }
    }

    // Check the chip id
    uint8_t chipID = lis3dh_get_chip_id(dev);
    if (chipID != LIS3DH_CHIPID) {
        printf("%s: invalid chip id: %02x, expected %02x\n", __FUNCTION__, chipID, LIS3DH_CHIPID);
        lis3dh_close(dev);
        return NULL;
    }

    // Call devinit with default options
    if (lis3dh_devinit(dev, LIS3DH_ODR_100HZ, LIS3DH_FS_2G, true)) {
        printf("%s: lis3dh_devinit() failed\n", __FUNCTION__);
        lis3dh_close(dev);
        return NULL;
    }

    return dev;
}

void
lis3dh_close(lis3dh_context dev)
{
    assert(dev != NULL);

    lis3dh_uninstall_isr(dev, LIS3DH_INTERRUPT_INT1);
    lis3dh_uninstall_isr(dev, LIS3DH_INTERRUPT_INT2);

    if (dev->i2c) {
        mraa_i2c_stop(dev->i2c);
    }

    if (dev->spi) {
        mraa_spi_stop(dev->spi);
    }

    if (dev->gpioCS) {
        mraa_gpio_close(dev->gpioCS);
    }

    free(dev);
}

upm_result_t
lis3dh_devinit(const lis3dh_context dev, LIS3DH_ODR_T odr, LIS3DH_FS_T fs, bool high_res)
{
    assert(dev != NULL);

    // Set high resolution mode, ODR and FS using passed values.
    // Also unconditionally enable X, Y and Z axes, temperature sensor (and ADC),
    // BDU mode as well as disable output high-pass filter.
    if (lis3dh_enable_lp_mode(dev, false) ||
        lis3dh_enable_hr_mode(dev, high_res) ||
        lis3dh_enable_axes(dev, true, true, true) ||
        lis3dh_enable_bdu_mode(dev, true) ||
        lis3dh_set_odr(dev, odr) ||
        lis3dh_set_full_scale(dev, fs) ||
        lis3dh_enable_hp_filtering(dev, false) ||
        lis3dh_enable_temperature(dev, true)) {

        printf("%s: failed to set configuration parameters\n", __FUNCTION__);
        return UPM_ERROR_OPERATION_FAILED;
    }

    // Settle
    upm_delay_ms(50);

    return UPM_SUCCESS;
}

upm_result_t
lis3dh_enable_axes(const lis3dh_context dev,
                   bool x_axis_enable,
                   bool y_axis_enable,
                   bool z_axis_enable)
{
    assert(dev != NULL);

    uint8_t reg = lis3dh_read_reg(dev, LIS3DH_REG_CTRL_REG1);

    // X axis
    if (x_axis_enable) {
        reg |= LIS3DH_CTRL_REG1_XEN;
    } else {
        reg &= ~LIS3DH_CTRL_REG1_XEN;
    }

    // Y axis
    if (y_axis_enable) {
        reg |= LIS3DH_CTRL_REG1_YEN;
    } else {
        reg &= ~LIS3DH_CTRL_REG1_YEN;
    }

    // Z axis
    if (z_axis_enable) {
        reg |= LIS3DH_CTRL_REG1_ZEN;
    } else {
        reg &= ~LIS3DH_CTRL_REG1_ZEN;
    }

    if (lis3dh_write_reg(dev, LIS3DH_REG_CTRL_REG1, reg)) {
        printf("%s: failed to enable axes\n", __FUNCTION__);
        return UPM_ERROR_OPERATION_FAILED;
    }

    return UPM_SUCCESS;
}

upm_result_t
lis3dh_enable_bdu_mode(const lis3dh_context dev, bool bdu_enable)
{
    assert(dev != NULL);

    uint8_t reg = lis3dh_read_reg(dev, LIS3DH_REG_CTRL_REG4);

    if (bdu_enable) {
        reg |= LIS3DH_CTRL_REG4_BDU;
    } else {
        reg &= ~LIS3DH_CTRL_REG4_BDU;
    }

    if (lis3dh_write_reg(dev, LIS3DH_REG_CTRL_REG4, reg)) {
        printf("%s: failed to set BDU mode\n", __FUNCTION__);
        return UPM_ERROR_OPERATION_FAILED;
    }

    return UPM_SUCCESS;
}

upm_result_t
lis3dh_enable_lp_mode(const lis3dh_context dev, bool lp_enable)
{
    assert(dev != NULL);

    uint8_t reg = lis3dh_read_reg(dev, LIS3DH_REG_CTRL_REG1);

    if (lp_enable) {
        // Check whether high resolution mode is enabled - enabling both LP and HR is not allowed
        uint8_t tmp_reg = lis3dh_read_reg(dev, LIS3DH_REG_CTRL_REG4);
        if (tmp_reg & LIS3DH_CTRL_REG4_HR) {
            printf("%s: can't enable low power mode, high resolution mode is already enabled\n",
                   __FUNCTION__);
            return UPM_ERROR_INVALID_PARAMETER;
        } else {
            // We are good - enable low power mode
            reg |= LIS3DH_CTRL_REG1_LPEN;
            // Set temperatureFactor according to LP mode bit width (8b).
            // This is needed to account for left alignment of the temperature data.
            // We have to shift the data right (== divide by a factor in case of float)
            // to eliminate "dead" bits.
            dev->temperatureFactor = 256;
        }
    } else {
        reg &= ~LIS3DH_CTRL_REG1_LPEN;
        // Set temperatureFactor according to Normal mode bit width (10b)
        dev->temperatureFactor = 64;
    }

    if (lis3dh_write_reg(dev, LIS3DH_REG_CTRL_REG1, reg)) {
        printf("%s: failed to set low power mode\n", __FUNCTION__);
        return UPM_ERROR_OPERATION_FAILED;
    }

    return UPM_SUCCESS;
}

upm_result_t
lis3dh_enable_hr_mode(const lis3dh_context dev, bool hr_enable)
{
    assert(dev != NULL);

    uint8_t reg = lis3dh_read_reg(dev, LIS3DH_REG_CTRL_REG4);

    if (hr_enable) {
        // Check whether low power mode is enabled - enabling both LP and HR is not allowed
        uint8_t tmp_reg = lis3dh_read_reg(dev, LIS3DH_REG_CTRL_REG1);
        if (tmp_reg & LIS3DH_CTRL_REG1_LPEN) {
            printf("%s: can't enable high resolution mode, low power mode is already enabled\n",
                   __FUNCTION__);
            return UPM_ERROR_INVALID_PARAMETER;
        } else {
            // We are good - enable high resolution mode
            reg |= LIS3DH_CTRL_REG4_HR;
        }
    } else {
        reg &= ~LIS3DH_CTRL_REG4_HR;
    }

    // Set the temperature sensor scaling factor appropriately.
    // Its max is 10 bit for both normal and HR modes.
    dev->temperatureFactor = 64;

    if (lis3dh_write_reg(dev, LIS3DH_REG_CTRL_REG4, reg)) {
        printf("%s: failed to set high resolution mode\n", __FUNCTION__);
        return UPM_ERROR_OPERATION_FAILED;
    }

    return UPM_SUCCESS;
}

upm_result_t
lis3dh_enable_normal_mode(const lis3dh_context dev)
{
    assert(dev != NULL);

    // There's no special mode bit for Normal - just disable LP and HR
    if (lis3dh_enable_lp_mode(dev, false) || lis3dh_enable_hr_mode(dev, false)) {
        printf("%s: failed to enable normal mode\n", __FUNCTION__);
        return UPM_ERROR_OPERATION_FAILED;
    }

    return UPM_SUCCESS;
}

upm_result_t
lis3dh_enable_hp_filtering(const lis3dh_context dev, bool filter)
{
    assert(dev != NULL);

    uint8_t reg = lis3dh_read_reg(dev, LIS3DH_REG_CTRL_REG2);

    if (filter) {
        reg |= LIS3DH_CTRL_REG2_FDS;
    } else {
        reg &= ~LIS3DH_CTRL_REG2_FDS;
    }

    if (lis3dh_write_reg(dev, LIS3DH_REG_CTRL_REG2, reg)) {
        printf("%s: failed to set HP filter mode\n", __FUNCTION__);
        return UPM_ERROR_OPERATION_FAILED;
    }

    return UPM_SUCCESS;
}

upm_result_t
lis3dh_enable_interrupt_latching(const lis3dh_context dev, bool int1_latch, bool int2_latch)
{
    assert(dev != NULL);

    uint8_t reg = lis3dh_read_reg(dev, LIS3DH_REG_CTRL_REG5);

    if (int1_latch) {
        reg |= LIS3DH_CTRL_REG5_LIR_INT1;
    } else {
        reg &= ~LIS3DH_CTRL_REG5_LIR_INT1;
    }

    if (int2_latch) {
        reg |= LIS3DH_CTRL_REG5_LIR_INT2;
    } else {
        reg &= ~LIS3DH_CTRL_REG5_LIR_INT2;
    }

    if (lis3dh_write_reg(dev, LIS3DH_REG_CTRL_REG5, reg)) {
        printf("%s: failed to set interrupt latching mode\n", __FUNCTION__);
        return UPM_ERROR_OPERATION_FAILED;
    }

    return UPM_SUCCESS;
}

upm_result_t
lis3dh_enable_adc(const lis3dh_context dev, bool adc_enable)
{
    assert(dev != NULL);

    // BDU mode is a prerequisite
    if (adc_enable && lis3dh_enable_bdu_mode(dev, true)) {
        printf("%s: failed to enable BDU mode - a prerequisite for enabling ADC\n", __FUNCTION__);
        return UPM_ERROR_OPERATION_FAILED;
    }

    uint8_t reg = lis3dh_read_reg(dev, LIS3DH_REG_TEMP_CFG_REG);

    if (adc_enable) {
        reg |= LIS3DH_TEMP_CFG_REG_ADC_EN;
    } else {
        reg &= ~LIS3DH_TEMP_CFG_REG_ADC_EN;
    }

    if (lis3dh_write_reg(dev, LIS3DH_REG_TEMP_CFG_REG, reg)) {
        printf("%s: failed to set ADC mode\n", __FUNCTION__);
        return UPM_ERROR_OPERATION_FAILED;
    }

    return UPM_SUCCESS;
}

upm_result_t
lis3dh_enable_temperature(const lis3dh_context dev, bool temperature_enable)
{
    assert(dev != NULL);

    // ADC must be enabled for temperature readings to work
    if (temperature_enable && lis3dh_enable_adc(dev, true)) {
        printf("%s: failed to enable ADC - a prerequisite for enabling temperature sensor\n",
               __FUNCTION__);
        return UPM_ERROR_OPERATION_FAILED;
    }

    uint8_t reg = lis3dh_read_reg(dev, LIS3DH_REG_TEMP_CFG_REG);

    if (temperature_enable) {
        reg |= LIS3DH_TEMP_CFG_REG_TEMP_EN;
    } else {
        reg &= ~LIS3DH_TEMP_CFG_REG_TEMP_EN;
    }

    if (lis3dh_write_reg(dev, LIS3DH_REG_TEMP_CFG_REG, reg)) {
        printf("%s: failed to set temperature sensor mode\n", __FUNCTION__);
        return UPM_ERROR_OPERATION_FAILED;
    }

    return UPM_SUCCESS;
}

upm_result_t
lis3dh_set_odr(const lis3dh_context dev, LIS3DH_ODR_T odr)
{
    assert(dev != NULL);

    bool lp_mode = false;
    uint8_t reg = lis3dh_read_reg(dev, LIS3DH_REG_CTRL_REG1);

    // Zero out ODR bits
    reg &= ~_SHIFTMASK(CTRL_REG1_ODR);

    // We encoded an extra bit in LIS3DH_ODR_T indicating an LP mode. Check for it here.
    if ((int) odr > (int) _MASK(CTRL_REG1_ODR)) {
        lp_mode = true;
    }

    // Mask it off and set it
    odr &= _MASK(CTRL_REG1_ODR);
    reg |= (odr << _SHIFT(CTRL_REG1_ODR));

    // Set the LPEN bit appropriately
    lis3dh_enable_lp_mode(dev, lp_mode);

    // Commit our changes
    if (lis3dh_write_reg(dev, LIS3DH_REG_CTRL_REG1, reg)) {
        printf("%s: failed to set ODR configuration\n", __FUNCTION__);
        return UPM_ERROR_OPERATION_FAILED;
    }

    return UPM_SUCCESS;
}

upm_result_t
lis3dh_set_full_scale(const lis3dh_context dev, LIS3DH_FS_T fs)
{
    assert(dev != NULL);

    uint8_t reg = lis3dh_read_reg(dev, LIS3DH_REG_CTRL_REG4);

    // Mask out FS bits, add our own
    reg &= ~_SHIFTMASK(CTRL_REG4_FS);
    reg |= (fs << _SHIFT(CTRL_REG4_FS));

    if (lis3dh_write_reg(dev, LIS3DH_REG_CTRL_REG4, reg)) {
        printf("%s: failed to set FS configuration\n", __FUNCTION__);
        return UPM_ERROR_OPERATION_FAILED;
    }

    // Basic sensitivity in g/LSB, calculated for a full 16b resolution.
    switch (fs) {
        case LIS3DH_FS_2G:
            // (2*2) / 2^16
            dev->accScale = 0.000061;
            break;

        case LIS3DH_FS_4G:
            // (4*2) / 2^16
            dev->accScale = 0.000122;
            break;

        case LIS3DH_FS_8G:
            // (8*2) / 2^16
            dev->accScale = 0.000244;
            break;

        case LIS3DH_FS_16G:
            // (16*2) / 2^16
            dev->accScale = 0.000488;
            break;
    }

    return UPM_SUCCESS;
}

upm_result_t
lis3dh_set_interrupt_active_high(const lis3dh_context dev, bool high)
{
    assert(dev != NULL);

    uint8_t reg = lis3dh_read_reg(dev, LIS3DH_REG_CTRL_REG6);

    if (high) {
        reg &= ~LIS3DH_CTRL_REG6_INT_POLARITY;
    } else {
        reg |= LIS3DH_CTRL_REG6_INT_POLARITY;
    }

    if (lis3dh_write_reg(dev, LIS3DH_REG_CTRL_REG6, reg)) {
        printf("%s: failed to set interrupt polarity mode\n", __FUNCTION__);
        return UPM_ERROR_OPERATION_FAILED;
    }

    return UPM_SUCCESS;
}

upm_result_t
lis3dh_set_int1_config(const lis3dh_context dev, uint8_t cfg)
{
    assert(dev != NULL);

    if (lis3dh_write_reg(dev, LIS3DH_REG_CTRL_REG3, cfg)) {
        printf("%s: failed to set interrupt 1 configuration\n", __FUNCTION__);
        return UPM_ERROR_OPERATION_FAILED;
    }

    return UPM_SUCCESS;
}

upm_result_t
lis3dh_set_int2_config(const lis3dh_context dev, uint8_t cfg)
{
    assert(dev != NULL);

    if (lis3dh_write_reg(dev, LIS3DH_REG_CTRL_REG6, cfg)) {
        printf("%s: failed to set interrupt 2 configuration\n", __FUNCTION__);
        return UPM_ERROR_OPERATION_FAILED;
    }

    return UPM_SUCCESS;
}

uint8_t
lis3dh_read_reg(const lis3dh_context dev, uint8_t reg)
{
    assert(dev != NULL);

    if (dev->spi) {
        // Needed for read
        reg |= 0x80;
        uint8_t pkt[2] = { reg, 0 };

        _csOn(dev);
        if (mraa_spi_transfer_buf(dev->spi, pkt, pkt, 2)) {
            _csOff(dev);
            printf("%s: mraa_spi_transfer_buf() failed\n", __FUNCTION__);
            return 0xFF;
        }
        _csOff(dev);

        return pkt[1];
    } else {
        return (uint8_t) mraa_i2c_read_byte_data(dev->i2c, reg);
    }
}

int
lis3dh_read_regs(const lis3dh_context dev, uint8_t reg, uint8_t* buffer, int len)
{
    assert(dev != NULL);

    if (dev->spi) {
        // Needed for read with address autoincrement
        reg |= 0xC0;

        uint8_t sbuf[len + 1];
        memset((char*) sbuf, 0, len + 1);
        sbuf[0] = reg;

        _csOn(dev);
        if (mraa_spi_transfer_buf(dev->spi, sbuf, sbuf, len + 1)) {
            _csOff(dev);
            printf("%s: mraa_spi_transfer_buf() failed\n", __FUNCTION__);
            return -1;
        }
        _csOff(dev);

        // Now copy it into user buffer
        for (int i = 0; i < len; i++) {
            buffer[i] = sbuf[i + 1];
        }
    } else {
        // Needed for read with address autoincrement
        reg |= 0x80;
        if (mraa_i2c_read_bytes_data(dev->i2c, reg, buffer, len) != len) {
            return -1;
        }
    }

    return len;
}

upm_result_t
lis3dh_write_reg(const lis3dh_context dev, uint8_t reg, uint8_t val)
{
    assert(dev != NULL);

    if (dev->spi) {
        // Mask off 0x80 for writing
        reg &= 0x7F;
        uint8_t pkt[2] = { reg, val };

        _csOn(dev);
        if (mraa_spi_transfer_buf(dev->spi, pkt, NULL, 2)) {
            _csOff(dev);
            printf("%s: mraa_spi_transfer_buf() failed.", __FUNCTION__);

            return UPM_ERROR_OPERATION_FAILED;
        }
        _csOff(dev);
    } else {
        if (mraa_i2c_write_byte_data(dev->i2c, val, reg)) {
            printf("%s: mraa_i2c_write_byte_data() failed.", __FUNCTION__);
            return UPM_ERROR_OPERATION_FAILED;
        }
    }

    return UPM_SUCCESS;
}

upm_result_t
lis3dh_update(const lis3dh_context dev)
{
    assert(dev != NULL);

    // Max axes data length, 2 bytes per axis * 3 axes
    const int bufLen = 6;
    // Max temperature data length
    const int temperatureBufLen = 2;
    // We reuse the same array when reading acceleration and then temperature data
    uint8_t buf[bufLen];

    if (lis3dh_read_regs(dev, LIS3DH_REG_OUT_X_L, buf, bufLen) != bufLen) {
        printf("%s: lis3dh_read_regs() failed to read %d bytes of axes data\n", __FUNCTION__, bufLen);
        return UPM_ERROR_OPERATION_FAILED;
    }

    // X                       MSB     LSB
    dev->accX = INT16_TO_FLOAT(buf[1], buf[0]);

    // Y
    dev->accY = INT16_TO_FLOAT(buf[3], buf[2]);

    // Z
    dev->accZ = INT16_TO_FLOAT(buf[5], buf[4]);

    // Get the temperature
    if (lis3dh_read_regs(dev, LIS3DH_REG_OUT_ADC3_L, buf, temperatureBufLen) != temperatureBufLen) {
        printf("%s: lis3dh_read_regs() failed to read %d bytes of temperature data\n",
               __FUNCTION__,
               temperatureBufLen);
        return UPM_ERROR_OPERATION_FAILED;
    }

    dev->temperature = INT16_TO_FLOAT(buf[1], buf[0]);

    return UPM_SUCCESS;
}

uint8_t
lis3dh_get_chip_id(const lis3dh_context dev)
{
    assert(dev != NULL);

    return lis3dh_read_reg(dev, LIS3DH_REG_WHO_AM_I);
}

void
lis3dh_get_accelerometer(const lis3dh_context dev, float* x, float* y, float* z)
{
    assert(dev != NULL);

    if (x) {
        *x = dev->accX * dev->accScale;
    }

    if (y) {
        *y = dev->accY * dev->accScale;
    }

    if (z) {
        *z = dev->accZ * dev->accScale;
    }
}

float
lis3dh_get_temperature(const lis3dh_context dev)
{
    assert(dev != NULL);

    return (dev->temperature / dev->temperatureFactor);
}

uint8_t
lis3dh_get_status(const lis3dh_context dev)
{
    assert(dev != NULL);

    return lis3dh_read_reg(dev, LIS3DH_REG_STATUS_REG);
}

uint8_t
lis3dh_get_status_aux(const lis3dh_context dev)
{
    assert(dev != NULL);

    return lis3dh_read_reg(dev, LIS3DH_REG_STATUS_REG_AUX);
}

upm_result_t
lis3dh_install_isr(const lis3dh_context dev,
                   LIS3DH_INTERRUPT_PINS_T intr,
                   int gpio,
                   mraa_gpio_edge_t level,
                   void (*isr)(void*),
                   void* arg)
{
    assert(dev != NULL);

    // Delete any existing ISR and GPIO context for this interrupt
    lis3dh_uninstall_isr(dev, intr);

    mraa_gpio_context gpio_isr = NULL;

    // Create GPIO context
    if (!(gpio_isr = mraa_gpio_init(gpio))) {
        printf("%s: mraa_gpio_init() failed\n", __FUNCTION__);
        return UPM_ERROR_OPERATION_FAILED;
    }

    mraa_gpio_dir(gpio_isr, MRAA_GPIO_IN);

    if (mraa_gpio_isr(gpio_isr, level, isr, arg)) {
        mraa_gpio_close(gpio_isr);
        printf("%s: mraa_gpio_isr() failed\n", __FUNCTION__);
        return UPM_ERROR_OPERATION_FAILED;
    }

    switch (intr) {
        case LIS3DH_INTERRUPT_INT1:
            dev->gpioINT1 = gpio_isr;
            break;

        case LIS3DH_INTERRUPT_INT2:
            dev->gpioINT2 = gpio_isr;
            break;
    }

    return UPM_SUCCESS;
}

void
lis3dh_uninstall_isr(const lis3dh_context dev, LIS3DH_INTERRUPT_PINS_T intr)
{
    assert(dev != NULL);

    switch (intr) {
        case LIS3DH_INTERRUPT_INT1:
            if (dev->gpioINT1) {
                mraa_gpio_isr_exit(dev->gpioINT1);
                mraa_gpio_close(dev->gpioINT1);
                dev->gpioINT1 = NULL;
            }
            break;

        case LIS3DH_INTERRUPT_INT2:
            if (dev->gpioINT2) {
                mraa_gpio_isr_exit(dev->gpioINT2);
                mraa_gpio_close(dev->gpioINT2);
                dev->gpioINT2 = NULL;
            }
            break;
    }
}
