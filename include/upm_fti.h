/*
 * Authors:
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

#ifndef UPM_FTI_H_
#define UPM_FTI_H_

#ifdef __cplusplus
extern "C" {
#endif

/**
 * The UPM Function Table Interface (FTI)
 */


/* Sensor categories */
typedef enum {
    UPM_ACCELEROMETER,
    UPM_ANGLE,
    UPM_AUDIO,
    UPM_COMPASS,
    UPM_CURRENT,
    UPM_DISPLAY,
    UPM_DISTANCE,
    UPM_ELECTRICITY,
    UPM_FLOW,
    UPM_FORCE,
    UPM_GAS,
    UPM_GYROSCOPE,
    UPM_HEART_RATE,
    UPM_HUMIDITY,
    UPM_IMU,
    UPM_JOYSTICK,
    UPM_LED,
    UPM_LIGHT,
    UPM_MOISTURE,
    UPM_NFC,
    UPM_PH,
    UPM_POTENTIOMETER,
    UPM_PRESSURE,
    UPM_RAW,
    UPM_SENSOR,
    UPM_SERVO,
    UPM_STEPPER,
    UPM_SWITCH,
    UPM_TEMPERATURE,
    UPM_TIME,
    UPM_VIBRATION,
    UPM_VIDEO,
    UPM_VOLTAGE,
    UPM_WIRELESS,
    UPM_STREAM
} upm_sensor_t;

/* Supported IO protocols via MRAA */
typedef enum {
    UPM_ANALOG,
    UPM_GPIO,
    UPM_PWM,
    UPM_I2C,
    UPM_SPI,
    UPM_UART,
    UPM_ONEWIRE
} upm_protocol_t;

/* Sensor descriptor */
typedef struct _upm_sensor_descriptor {
    const char* name;
    const char* description;
    int protocol_size;
    const upm_protocol_t* protocol;
    int category_size;
    const upm_sensor_t* category;
} upm_sensor_descriptor_t;

/* Function pointer typedef helpers */
typedef struct _upm_sensor_ft* (*func_get_upm_sensor_ft)(upm_sensor_t sensor_type);

#include <types/upm_acceleration.h>
#include <types/upm_angle.h>
#include <types/upm_audio.h>
#include <types/upm_distance.h>
#include <types/upm_heart_rate.h>
#include <types/upm_ph.h>
#include <types/upm_potentiometer.h>
#include <types/upm_servo.h>
#include <types/upm_sensor.h>
#include <types/upm_switch.h>
#include <types/upm_temperature.h>
#include <types/upm_touch.h>
#include <types/upm_voltage.h>
#include <types/upm_vibration.h>
#include <types/upm_moisture.h>
#include <types/upm_light.h>
#include <types/upm_stream.h>

#ifdef __cplusplus
}
#endif

#endif /* UPM_FTI_H_ */
