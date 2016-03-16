/*
 * Author: Brendan Le Foll <brendan.le.foll@intel.com>
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

static void
handleResponses(uint8_t* buf, int length)
{
    awaitingReponse->m_results = new char(length);
    memcpy((void*)awaitingReponse->m_results, (void*)buf, length);
    pthread_cond_broadcast(&(awaitingReponse->m_responseCond));
}

void
CurieImu::readAccelerometer(int *xVal, int *yVal, int *zVal)
{
  char message[4];
  message[0] = FIRMATA_START_SYSEX;
  message[1] = FIRMATA_CURIE_IMU;
  message[2] = FIRMATA_CURIE_IMU_READ_ACCEL;
  message[3] = FIRMATA_END_SYSEX;

  pthread_mutex_lock(&m_responseLock);

  mraa_firmata_response_stop(m_firmata);
  mraa_firmata_response(m_firmata, handleResponses);
  mraa_firmata_write_sysex(m_firmata, &message[0], 4);

  awaitingReponse = this;
  pthread_cond_wait(&m_responseCond, &m_responseLock);

  *xVal = ((m_results[3] & 0x7f) | ((m_results[4] & 0x7f) << 7));
  *yVal = ((m_results[5] & 0x7f) | ((m_results[6] & 0x7f) << 7));
  *zVal = ((m_results[7] & 0x7f) | ((m_results[8] & 0x7f) << 7));

  delete m_results;
  pthread_mutex_unlock(&m_responseLock);

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

  pthread_mutex_lock(&m_responseLock);

  mraa_firmata_response_stop(m_firmata);
  mraa_firmata_response(m_firmata, handleResponses);
  mraa_firmata_write_sysex(m_firmata, &message[0], 4);

  awaitingReponse = this;
  pthread_cond_wait(&m_responseCond, &m_responseLock);

  *xVal = ((m_results[3] & 0x7f) | ((m_results[4] & 0x7f) << 7));
  *yVal = ((m_results[5] & 0x7f) | ((m_results[6] & 0x7f) << 7));
  *zVal = ((m_results[7] & 0x7f) | ((m_results[8] & 0x7f) << 7));

  delete m_results;
  pthread_mutex_unlock(&m_responseLock);

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

    pthread_mutex_lock(&m_responseLock);

    mraa_firmata_response_stop(m_firmata);
    mraa_firmata_response(m_firmata, handleResponses);
    mraa_firmata_write_sysex(m_firmata, &message[0], 4);

    awaitingReponse = this;
    pthread_cond_wait(&m_responseCond, &m_responseLock);

    int16_t result;
    result = ((m_results[3] & 0x7f) | ((m_results[4] & 0x7f) << 7));
    result += ((m_results[5] & 0x7f) | ((m_results[6] & 0x7f) << 7)) << 8;

    delete m_results;
    pthread_mutex_unlock(&m_responseLock);

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

  pthread_mutex_lock(&m_responseLock);

  mraa_firmata_response_stop(m_firmata);
  mraa_firmata_response(m_firmata, handleResponses);
  mraa_firmata_write_sysex(m_firmata, &message[0], 4);

  awaitingReponse = this;
  pthread_cond_wait(&m_responseCond, &m_responseLock);

  *xA = ((m_results[3] & 0x7f) | ((m_results[4] & 0x7f) << 7));
  *yA = ((m_results[5] & 0x7f) | ((m_results[6] & 0x7f) << 7));
  *zA = ((m_results[7] & 0x7f) | ((m_results[8] & 0x7f) << 7));
  *xG = ((m_results[9] & 0x7f) | ((m_results[10] & 0x7f) << 7));
  *yG = ((m_results[11] & 0x7f) | ((m_results[12] & 0x7f) << 7));
  *zG = ((m_results[13] & 0x7f) | ((m_results[13] & 0x7f) << 7));

  delete m_results;
  pthread_mutex_unlock(&m_responseLock);

  return;
}
