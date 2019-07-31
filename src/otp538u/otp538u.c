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

#include <assert.h>
#include <string.h>

#include <upm_utilities.h>
#include <upm_platform.h>

#include "otp538u.h"

#include "thermopile_vt_table.h"
#include "thermister_rt_table.h"


// To save memory space, we disable debugging on non-linux platforms
// by default (Zephyr).  For other systems, enable as desired/possible.
#if defined(UPM_PLATFORM_LINUX)
# define OTP538U_DEBUG_ENABLED
#endif // UPM_PLATFORM_LINUX

otp538u_context otp538u_init(int pinA, int pinO, float aref)
{
    otp538u_context dev =
        (otp538u_context)malloc(sizeof(struct _otp538u_context));

    if (!dev)
        return NULL;

    memset((void *)dev, 0, sizeof(struct _otp538u_context));

    // make sure MRAA is initialized
    int mraa_rv;
    if ((mraa_rv = mraa_init()) != MRAA_SUCCESS)
    {
        printf("%s: mraa_init() failed (%d).\n", __FUNCTION__, mraa_rv);
        otp538u_close(dev);
        return NULL;
    }

    // initialize the MRAA contexts

    if (!(dev->aioA = mraa_aio_init(pinA)))
    {
        printf("%s: mraa_aio_init(pinA) failed.\n", __FUNCTION__);
        otp538u_close(dev);

        return NULL;
    }

    if (!(dev->aioO = mraa_aio_init(pinO)))
    {
        printf("%s: mraa_aio_init(pinO) failed.\n", __FUNCTION__);
        otp538u_close(dev);

        return NULL;
    }

    // for subplatforms like the Arduino 101 with Firmata, we need to
    // limit ADC resolution to 10b currently.  For this sensor
    // unfortunately, this means readings will be less accurate.  This
    // sensor really does need to measure with about 1mV accuracy.
    const int adcHighRes = 4095;
    const int adcLowRes = 1023;

    bool isSubplatform = false;

    dev->debug = false;

    if (pinA >= 512 || pinO >= 512)
        isSubplatform = true;

    // this is the internal voltage reference on the Grove IR temp
    // sensor module for the thermistor.

    dev->internalVRef = 2.5;

    // analog reference in use
    dev->aref = aref;

    // This is the value of the output resistor of the Grove IR
    // temp sensor's SIG2 output (ambient)
    dev->vResistance = 2000000;      // 2M ohms

    // This was the default offset voltage in the seeedstudio code.  You
    // can adjust as neccessary depending on your calibration.
    dev->offsetVoltage = 0.014;

    // We need around 1mV resolution (preferred), so use 12 bit
    // resolution (4096) if we can.
    //
    // This logic is over complicated due to the fact that it is
    // currently difficult to determine exactly what the capabilities of
    // the platform (sub or otherwise) actually are.  So for
    // subplatforms, we always limit to 1024.  Otherwise, we try 12b if
    // the mraa_adc_raw_bits() says we can, though this isn't
    // particularly accurate either, as it reports that the G2 can do
    // 12b, when in reality it can not.  We are just lucky that it works
    // anyway (ie: will give 12b resolution, though underneath it's just
    // scaling the real 10b value.).  Sigh.  But trying 12b resolution
    // on the 101 via firmata will definitely break things, so don't
    // even try until whatever the problem it has with 12b is fixed.
    if (isSubplatform)
    {
        dev->adcResolution = adcLowRes; // 10b
    }
    else
    {
        if (mraa_adc_raw_bits() == 12)
            dev->adcResolution = adcHighRes; // 12b
        else
            dev->adcResolution = adcLowRes; // 10b
    }

    // notify the user
    if (dev->adcResolution == adcLowRes)
        printf("Using 10 bit ADC resolution.  Values will be less accurate.\n");

    // enable 12 bit resolution, if we can
    if (dev->adcResolution == adcHighRes)
        mraa_aio_set_bit(dev->aioA, 12);

    if (dev->adcResolution == adcHighRes)
        mraa_aio_set_bit(dev->aioO, 12);

    if (isSubplatform)
    {
        // The first analog read always seems to return 0 on the 101
        // with firmata, so just do a couple of reads here and discard
        // them.  Then sleep for half a second.  THIS IS A HACK.  The
        // real problem should be fixed elsewhere (Firmata?).
        mraa_aio_read(dev->aioA);
        mraa_aio_read(dev->aioO);

        upm_delay_ms(500);
    }

    return dev;
}

void otp538u_close(otp538u_context dev)
{
    assert(dev != NULL);

    if (dev->aioA)
        mraa_aio_close(dev->aioA);
    if (dev->aioO)
        mraa_aio_close(dev->aioO);

    free(dev);
}

upm_result_t otp538u_get_ambient_temperature(const otp538u_context dev,
                                             float *temperature)
{
    assert(dev != NULL);

    const int samples = 5;
    int val = 0;
    float temp = 0;
    float res = 0;

    for (int i=0; i<samples; i++)
    {
        val = mraa_aio_read(dev->aioA);
        if (val == -1)
        {
            printf("%s: mraa_aio_read() failed.\n", __FUNCTION__);
            return UPM_ERROR_OPERATION_FAILED;
        }

        temp += (float)val;
        upm_delay_ms(10);
    }
    temp = temp / samples;

    float volts = temp * dev->aref / dev->adcResolution;

#if defined(OTP538U_DEBUG_ENABLED)
    if (dev->debug)
    {
        printf("\tAMB sample %f aref %f volts %f\n", temp, dev->aref,
               volts);
    }
#endif // OTP538U_DEBUG_ENABLED

    // compute the resistance of the thermistor
    res = dev->vResistance * volts / (dev->internalVRef - volts);

#if defined(OTP538U_DEBUG_ENABLED)
    if (dev->debug)
    {
        printf("\tAMB computed resistance: %f\n", res);
    }
#endif // OTP538U_DEBUG_ENABLED

    // look it up in the thermistor (RT) resistence/temperature table
    int rawslot;
    int j;
    for (j=0; j<otp538u_rt_table_max; j++)
        if (otp538u_rt_table[j] < res)
        {
            rawslot = j;
            break;
        }

    if (j >= otp538u_rt_table_max)
    {
        printf("%s: Ambient temperature out of range (high)\n", __FUNCTION__);
        return UPM_ERROR_OUT_OF_RANGE;
    }

    // we need to compensate for the fact that we are supporting
    // temperature values less than 0 (-20C), so adjust correspondingly
    // so that we obtain the correct temperature 'slot'.  This will be
    // our base temperature.
    int slot = rawslot - 20;

    // too cold
    if (slot < 0)
    {
        printf("%s: Ambient temperature out of range (low)\n", __FUNCTION__);
        return UPM_ERROR_OUT_OF_RANGE;
    }

    // now compute the ambient temperature
    float ambientTemp = slot - 1 +
        (otp538u_rt_table[rawslot - 1]-res) / (otp538u_rt_table[rawslot - 1] -
                                               otp538u_rt_table[rawslot]);

    *temperature = ambientTemp;
    return UPM_SUCCESS;
}

upm_result_t otp538u_get_object_temperature(const otp538u_context dev,
                                            float *temperature)
{
    assert(dev != NULL);

    const int samples = 5;
    const float reference_vol = 0.5; // what is this value? (from seeedstudio)
    const float tempIncrement = 10.0;
    int val = 0;
    float temp = 0;

    float ambTemp = 0.0;
    if (otp538u_get_ambient_temperature(dev, &ambTemp))
        return UPM_ERROR_OPERATION_FAILED;

    for (int i=0; i<samples; i++)
    {
        val = mraa_aio_read(dev->aioO);
        if (val == -1)
        {
            printf("%s: mraa_aio_read() failed.\n", __FUNCTION__);
            return UPM_ERROR_OPERATION_FAILED;
        }
        temp += val;
        upm_delay_ms(10);
    }

    temp = temp / samples;

#if defined(OTP538U_DEBUG_ENABLED)
    if (dev->debug)
        printf("\tOBJ sample %f ", temp);
#endif // OTP538U_DEBUG_ENABLED

    float volts = temp * dev->aref / dev->adcResolution;

#if defined(OTP538U_DEBUG_ENABLED)
    if (dev->debug)
        printf("VOLTS: %f ", volts);
#endif // OTP538U_DEBUG_ENABLED

    float sensorVolts = volts - (reference_vol + dev->offsetVoltage);

#if defined(OTP538U_DEBUG_ENABLED)
    if (dev->debug)
        printf("Sensor Voltage (computed): %f\n", sensorVolts);
#endif // OTP538U_DEBUG_ENABLED

    // search the VT (voltage/temperature) table to find the object
    // temperature.
    int slot;
    // add +2 to compensate for the -20C and -10C slots below zero
    int voltOffset = (int)(ambTemp / 10) + 1 + 2;
    float voltage = sensorVolts * 10.0;
    for (slot=0; slot<(otp538u_vt_table_max - 1); slot++)
    {
        if ( (voltage > otp538u_vt_table[slot][voltOffset]) &&
             (voltage < otp538u_vt_table[slot+1][voltOffset]) )
        {
            break;
        }
    }

    if (slot >= (otp538u_vt_table_max - 1))
    {
        printf("%s: Object temperature out of range (high)\n", __FUNCTION__);
        return UPM_ERROR_OUT_OF_RANGE;
    }

    float objTemp = ((float)tempIncrement * voltage) /
        ( otp538u_vt_table[slot + 1][voltOffset] -
          otp538u_vt_table[slot][voltOffset] );

#if defined(OTP538U_DEBUG_ENABLED)
    if (dev->debug)
    {
        printf("\tVoltage (%f): TABLE VALUE [%d][%d] = %f\n", voltage,
               slot, voltOffset, otp538u_vt_table[slot][voltOffset]);
    }
#endif // OTP538U_DEBUG_ENABLED

    *temperature = ambTemp + objTemp;
    return UPM_SUCCESS;
}

void otp538u_set_voltage_offset(const otp538u_context dev, float offset)
{
    assert(dev != NULL);

    dev->offsetVoltage = offset;
}

void otp538u_set_output_resistence(const otp538u_context dev,
                                   int resistance)
{
    assert(dev != NULL);

    dev->vResistance = resistance;
}

void otp538u_set_ivref(const otp538u_context dev, float vref)
{
    assert(dev != NULL);

    dev->internalVRef = vref;
}

void otp538u_set_debug(const otp538u_context dev, bool enable)
{
    assert(dev != NULL);

    dev->debug = enable;

#if !defined(UPM_PLATFORM_LINUX) && !defined(OTP538U_DEBUG_ENABLED)
    if (enable)
        printf("%s: Debugging not enabled at compilation time.\n",
               __FUNCTION__);
#endif // !UPM_PLATFORM_LINUX && !OTP538U_DEBUG_ENABLED
}
