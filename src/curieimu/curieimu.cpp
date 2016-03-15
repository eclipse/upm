/*
 * Author: Brendan Le Foll <brendan.le.foll@intel.com>
 * Author: Ron Evans (@deadprogram)
 * Author: Justin Zemlyansky (@JustInDevelopment)
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

#include <iostream>
#include <string>
#include <string.h>
#include <stdexcept>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>

#include "curieimu.hpp"

using namespace upm;

static CurieImu* awaitingReponse;

CurieImu::CurieImu (int subplatformoffset)
{
    m_firmata = mraa_firmata_init(FIRMATA_CURIE_IMU);
    if (m_firmata == NULL) {
        throw std::invalid_argument(std::string(__FUNCTION__) +
                                    ": mraa_firmata_init() failed");
        return;
    }

    if (pthread_mutex_init(&m_responseLock, NULL)) {
        throw std::runtime_error(std::string(__FUNCTION__) +
                                 ": pthread_mutex_init(m_responseLock) failed");
        return;
    }

    if (pthread_cond_init(&m_responseCond, NULL)) {
        throw std::runtime_error(std::string(__FUNCTION__) +
                                 ": pthread_cond_init(m_responseCond) failed");
        return;
    }
}

CurieImu::~CurieImu()
{
    pthread_mutex_destroy(&m_responseLock);
    pthread_cond_destroy(&m_responseCond);
}

void
CurieImu::lock()
{
    pthread_mutex_lock(&m_responseLock);
}

void
CurieImu::unlock()
{
    pthread_mutex_unlock(&m_responseLock);
}

void
CurieImu::waitForResponse()
{
    awaitingReponse = this;
    pthread_cond_wait(&m_responseCond, &m_responseLock);
}

void
CurieImu::proceed()
{
    pthread_cond_broadcast(&m_responseCond);
}

void
CurieImu::setResults(uint8_t* buf, int length)
{
    m_results = new char(length);
    memcpy((void*)m_results, (void*)buf, length);
}

/*
* Handles a single syncronous response being returned from Firmata
*
* @param buffer the data beinig returned from Firmata
* @param length length of results buffer
*/
static void
handleSyncResponse(uint8_t* buf, int length)
{
    awaitingReponse->setResults(buf, length);
    awaitingReponse->proceed();
}

/*
* Handles asyncronous responses being returned from Firmata
*
* @param buffer the data beinig returned from Firmata
* @param length length of results buffer
*/
static void
handleAsyncResponses(uint8_t* buf, int length)
{
    awaitingReponse->setResults(buf, length);
    awaitingReponse->processResponse();
}

void
CurieImu::processResponse()
{
    switch(m_results[2]) {
        case FIRMATA_CURIE_IMU_SHOCK_DETECT:
        {
            IMUDataItem* item = new IMUDataItem();
            item->axis = m_results[3];
            item->direction = m_results[4];
            m_shockData.push(item);
            break;
        }
        case FIRMATA_CURIE_IMU_STEP_COUNTER:
        {
            int count = ((m_results[3] & 0x7f) | ((m_results[4] & 0x7f) << 7));
            m_stepData.push(count);
            break;
        }
        case FIRMATA_CURIE_IMU_TAP_DETECT:
        {
            IMUDataItem* item = new IMUDataItem();
            item->axis = m_results[3];
            item->direction = m_results[4];
            m_tapData.push(item);
            break;
        }
    }

    return;
}

void
CurieImu::readAccelerometer(int *xVal, int *yVal, int *zVal)
{
    char message[4];
    message[0] = FIRMATA_START_SYSEX;
    message[1] = FIRMATA_CURIE_IMU;
    message[2] = FIRMATA_CURIE_IMU_READ_ACCEL;
    message[3] = FIRMATA_END_SYSEX;

    lock();

    mraa_firmata_response_stop(m_firmata);
    mraa_firmata_response(m_firmata, handleSyncResponse);
    mraa_firmata_write_sysex(m_firmata, &message[0], 4);

    waitForResponse();

    *xVal = ((m_results[3] & 0x7f) | ((m_results[4] & 0x7f) << 7));
    *yVal = ((m_results[5] & 0x7f) | ((m_results[6] & 0x7f) << 7));
    *zVal = ((m_results[7] & 0x7f) | ((m_results[8] & 0x7f) << 7));

    delete m_results;
    unlock();

    return;
}

void
CurieImu::readGyro(int *xVal, int *yVal, int *zVal)
{
    char message[4];
    message[0] = FIRMATA_START_SYSEX;
    message[1] = FIRMATA_CURIE_IMU;
    message[2] = FIRMATA_CURIE_IMU_READ_GYRO;
    message[3] = FIRMATA_END_SYSEX;

    lock();

    mraa_firmata_response_stop(m_firmata);
    mraa_firmata_response(m_firmata, handleSyncResponse);
    mraa_firmata_write_sysex(m_firmata, &message[0], 4);

    waitForResponse();

    *xVal = ((m_results[3] & 0x7f) | ((m_results[4] & 0x7f) << 7));
    *yVal = ((m_results[5] & 0x7f) | ((m_results[6] & 0x7f) << 7));
    *zVal = ((m_results[7] & 0x7f) | ((m_results[8] & 0x7f) << 7));

    delete m_results;
    unlock();

    return;
}

int16_t
CurieImu::getTemperature()
{
    char message[4];
    message[0] = FIRMATA_START_SYSEX;
    message[1] = FIRMATA_CURIE_IMU;
    message[2] = FIRMATA_CURIE_IMU_READ_TEMP;
    message[3] = FIRMATA_END_SYSEX;

    lock();

    mraa_firmata_response_stop(m_firmata);
    mraa_firmata_response(m_firmata, handleSyncResponse);
    mraa_firmata_write_sysex(m_firmata, &message[0], 4);

    waitForResponse();

    int16_t result;
    result = ((m_results[3] & 0x7f) | ((m_results[4] & 0x7f) << 7));
    result += ((m_results[5] & 0x7f) | ((m_results[6] & 0x7f) << 7)) << 8;

    delete m_results;
    unlock();

    return result;
}

void
CurieImu::readMotion(int *xA, int *yA, int *zA, int *xG, int *yG, int *zG)
{
    char message[4];
    message[0] = FIRMATA_START_SYSEX;
    message[1] = FIRMATA_CURIE_IMU;
    message[2] = FIRMATA_CURIE_IMU_READ_MOTION;
    message[3] = FIRMATA_END_SYSEX;

    lock();

    mraa_firmata_response_stop(m_firmata);
    mraa_firmata_response(m_firmata, handleSyncResponse);
    mraa_firmata_write_sysex(m_firmata, &message[0], 4);

    waitForResponse();

    *xA = ((m_results[3] & 0x7f) | ((m_results[4] & 0x7f) << 7));
    *yA = ((m_results[5] & 0x7f) | ((m_results[6] & 0x7f) << 7));
    *zA = ((m_results[7] & 0x7f) | ((m_results[8] & 0x7f) << 7));
    *xG = ((m_results[9] & 0x7f) | ((m_results[10] & 0x7f) << 7));
    *yG = ((m_results[11] & 0x7f) | ((m_results[12] & 0x7f) << 7));
    *zG = ((m_results[13] & 0x7f) | ((m_results[13] & 0x7f) << 7));

    delete m_results;
    unlock();

    return;
}

void
CurieImu::enableShockDetection(bool enable)
{
    char message[5];
    message[0] = FIRMATA_START_SYSEX;
    message[1] = FIRMATA_CURIE_IMU;
    message[2] = FIRMATA_CURIE_IMU_SHOCK_DETECT;
    message[3] = enable;
    message[4] = FIRMATA_END_SYSEX;

    lock();

    mraa_firmata_response_stop(m_firmata);
    mraa_firmata_response(m_firmata, handleAsyncResponses);
    mraa_firmata_write_sysex(m_firmata, &message[0], 5);

    awaitingReponse = this;
    unlock();

    return;
}

bool
CurieImu::isShockDetected()
{
    return (m_shockData.size() > 0);
}

void
CurieImu::getShockDetectData(int *axis, int *direction)
{
    if (m_shockData.size() > 0) {
        IMUDataItem* item = m_shockData.front();
        *axis = item->axis;
        *direction = item->direction;
        m_shockData.pop();
        delete item;
    }
}

void
CurieImu::enableStepCounter(bool enable)
{
    char message[5];
    message[0] = FIRMATA_START_SYSEX;
    message[1] = FIRMATA_CURIE_IMU;
    message[2] = FIRMATA_CURIE_IMU_STEP_COUNTER;
    message[3] = enable;
    message[4] = FIRMATA_END_SYSEX;

    lock();

    mraa_firmata_response_stop(m_firmata);
    mraa_firmata_response(m_firmata, handleAsyncResponses);
    mraa_firmata_write_sysex(m_firmata, &message[0], 5);

    awaitingReponse = this;
    unlock();

    return;
}

bool
CurieImu::isStepDetected()
{
    return (m_stepData.size() > 0);
}

void
CurieImu::getStepCount(int *count)
{
    if (m_stepData.size() > 0) {
        *count = m_stepData.front();
        m_stepData.pop();
    }
}

void
CurieImu::enableTapDetection(bool enable)
{
    char message[5];
    message[0] = FIRMATA_START_SYSEX;
    message[1] = FIRMATA_CURIE_IMU;
    message[2] = FIRMATA_CURIE_IMU_TAP_DETECT;
    message[3] = enable;
    message[4] = FIRMATA_END_SYSEX;

    lock();

    mraa_firmata_response_stop(m_firmata);
    mraa_firmata_response(m_firmata, handleAsyncResponses);
    mraa_firmata_write_sysex(m_firmata, &message[0], 5);

    awaitingReponse = this;
    unlock();

    return;
}

bool
CurieImu::isTapDetected()
{
    return (m_tapData.size() > 0);
}

void
CurieImu::getTapDetectData(int *axis, int *direction)
{
    if (m_tapData.size() > 0) {
        IMUDataItem* item = m_tapData.front();
        *axis = item->axis;
        *direction = item->direction;
        m_tapData.pop();
        delete item;
    }
}
