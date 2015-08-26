/*
 * Author: Jon Trulson <jtrulson@ics.com>
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

#include <unistd.h>
#include <math.h>
#include <iostream>
#include <string>

#include "adafruitms1438.h"

using namespace upm;
using namespace std;


AdafruitMS1438::AdafruitMS1438(int bus, uint8_t address) :
  m_pca9685(new PCA9685(bus, address))
{
  setupPinMaps();

  // set a default period of 50Hz
  setPWMPeriod(50);

  // disable all PWM's (4 of them).  They are shared with each other
  // (stepper/DC), so just disable the DC motors here
  disableMotor(MOTOR_M1);
  disableMotor(MOTOR_M2);
  disableMotor(MOTOR_M3);
  disableMotor(MOTOR_M4);

  // Set all 'on time' registers to 0
  m_pca9685->ledOnTime(PCA9685_ALL_LED, 0);

  // set the default stepper config at 200 steps per rev
  stepConfig(STEPMOTOR_M12, 200);
  stepConfig(STEPMOTOR_M34, 200);
}

AdafruitMS1438::~AdafruitMS1438()
{
  delete m_pca9685;
}

void AdafruitMS1438::initClock(STEPMOTORS_T motor)
{
  gettimeofday(&m_stepConfig[motor].startTime, NULL);
}

uint32_t AdafruitMS1438::getMillis(STEPMOTORS_T motor)
{
  struct timeval elapsed, now;
  uint32_t elapse;

  // get current time
  gettimeofday(&now, NULL);

  struct timeval startTime = m_stepConfig[motor].startTime;

  // compute the delta since m_startTime
  if( (elapsed.tv_usec = now.tv_usec - startTime.tv_usec) < 0 )
    {
      elapsed.tv_usec += 1000000;
      elapsed.tv_sec = now.tv_sec - startTime.tv_sec - 1;
    }
  else
    {
      elapsed.tv_sec = now.tv_sec - startTime.tv_sec;
    }

  elapse = (uint32_t)((elapsed.tv_sec * 1000) + (elapsed.tv_usec / 1000));

  // never return 0
  if (elapse == 0)
    elapse = 1;

  return elapse;
}

// setup the pin mappings of the pca9685 outputs to the proper motor controls
void AdafruitMS1438::setupPinMaps()
{
  // first the dc motors
  m_dcMotors[0] = (DC_PINMAP_T){ 8, 10, 9 };
  m_dcMotors[1] = (DC_PINMAP_T){ 13, 11, 12 };
  m_dcMotors[2] = (DC_PINMAP_T){ 2, 4, 3 };
  m_dcMotors[3] = (DC_PINMAP_T){ 7, 5, 6 };

  // now the 2 steppers
  m_stepMotors[0] = (STEPPER_PINMAP_T){ 8, 10, 9,
                                        13, 11, 12 };
  m_stepMotors[1] = (STEPPER_PINMAP_T){ 2, 4, 3,
                                        7, 5, 6 };
}

void AdafruitMS1438::setPWMPeriod(float hz)
{
  // must be in sleep mode to set the prescale register
  m_pca9685->setModeSleep(true);
  m_pca9685->setPrescaleFromHz(hz);
  m_pca9685->setModeSleep(false);
}

void AdafruitMS1438::enableMotor(DCMOTORS_T motor)
{
  m_pca9685->ledFullOff(m_dcMotors[motor].pwm, false);
}

void AdafruitMS1438::disableMotor(DCMOTORS_T motor)
{
  m_pca9685->ledFullOff(m_dcMotors[motor].pwm, true);
}

void AdafruitMS1438::enableStepper(STEPMOTORS_T motor)
{
  m_pca9685->ledFullOff(m_stepMotors[motor].pwmA, false);
  m_pca9685->ledFullOff(m_stepMotors[motor].pwmB, false);
}

void AdafruitMS1438::disableStepper(STEPMOTORS_T motor)
{
  m_pca9685->ledFullOff(m_stepMotors[motor].pwmA, true);
  m_pca9685->ledFullOff(m_stepMotors[motor].pwmB, true);
}

void AdafruitMS1438::setMotorSpeed(DCMOTORS_T motor, int speed)
{
  if (speed < 0)
    speed = 0;
  
  if (speed > 100)
    speed = 100;

  float percent = float(speed) / 100.0;
  
  // make sure that the FullOn bit is turned off, or the speed setting
  // (PWM duty cycle) won't have any effect.
  m_pca9685->ledFullOn(m_dcMotors[motor].pwm, false);

  // set the PWM duty cycle
  m_pca9685->ledOffTime(m_dcMotors[motor].pwm, int(4095.0 * percent));
}

void AdafruitMS1438::setStepperSpeed(STEPMOTORS_T motor, int speed)
{
  m_stepConfig[motor].stepDelay = 60 * 1000 / 
    m_stepConfig[motor].stepsPerRev / speed;
}

void AdafruitMS1438::setMotorDirection(DCMOTORS_T motor, DIRECTION_T dir)
{
  if (dir & 0x01)
    {
      m_pca9685->ledFullOn(m_dcMotors[motor].in1, true);
      m_pca9685->ledFullOff(m_dcMotors[motor].in1, false);
    }
  else
    {
      m_pca9685->ledFullOff(m_dcMotors[motor].in1, true);
      m_pca9685->ledFullOn(m_dcMotors[motor].in1, false);
    }

  if (dir & 0x02)
    {
      m_pca9685->ledFullOn(m_dcMotors[motor].in2, true);
      m_pca9685->ledFullOff(m_dcMotors[motor].in2, false);
    }
  else
    {
      m_pca9685->ledFullOff(m_dcMotors[motor].in2, true);
      m_pca9685->ledFullOn(m_dcMotors[motor].in2, false);
    }
}

void AdafruitMS1438::setStepperDirection(STEPMOTORS_T motor, DIRECTION_T dir)
{
  switch (dir)
    {
    case DIR_CW:
      m_stepConfig[motor].stepDirection = 1;
      break;
    case DIR_CCW:
      m_stepConfig[motor].stepDirection = -1;
      break;
    default:                // default to 1 if DIR_NONE specified
      m_stepConfig[motor].stepDirection = 1;
      break;
    }
}

void AdafruitMS1438::stepConfig(STEPMOTORS_T motor, unsigned int stepsPerRev)
{
  m_stepConfig[motor].stepsPerRev = stepsPerRev;
  m_stepConfig[motor].currentStep = 0;
  m_stepConfig[motor].stepDelay = 0;
  m_stepConfig[motor].stepDirection = 1; // forward

  // now, setup the control pins - we want both FULL ON and FULL OFF.
  // Since FULL OFF has precedence, we can then control the steps by
  // just turning on/off the FULL OFF bit for the relevant outputs

  m_pca9685->ledFullOff(m_stepMotors[motor].pwmA, true);
  m_pca9685->ledFullOn(m_stepMotors[motor].pwmA, true);

  m_pca9685->ledFullOff(m_stepMotors[motor].pwmB, true);
  m_pca9685->ledFullOn(m_stepMotors[motor].pwmB, true);

  m_pca9685->ledFullOff(m_stepMotors[motor].in1A, true);
  m_pca9685->ledFullOn(m_stepMotors[motor].in1A, true);

  m_pca9685->ledFullOff(m_stepMotors[motor].in2A, true);
  m_pca9685->ledFullOn(m_stepMotors[motor].in2A, true);

  m_pca9685->ledFullOff(m_stepMotors[motor].in1B, true);
  m_pca9685->ledFullOn(m_stepMotors[motor].in1B, true);

  m_pca9685->ledFullOff(m_stepMotors[motor].in2B, true);
  m_pca9685->ledFullOn(m_stepMotors[motor].in2B, true);
}

void AdafruitMS1438::stepperStep(STEPMOTORS_T motor)
{
  int step = m_stepConfig[motor].currentStep % 4;

  //   Step I0 I1 I2 I3
  //     1  1  0  1  0
  //     2  0  1  1  0
  //     3  0  1  0  1
  //     4  1  0  0  1

  // we invert the logic since we are essentially toggling an OFF bit,
  // not an ON bit.
  switch (step)
    {
    case 0:    // 1010
      m_pca9685->ledFullOff(m_stepMotors[motor].in1A, false);
      m_pca9685->ledFullOff(m_stepMotors[motor].in2A, true);
      m_pca9685->ledFullOff(m_stepMotors[motor].in1B, false);
      m_pca9685->ledFullOff(m_stepMotors[motor].in2B, true);
      break;
    case 1:    // 0110
      m_pca9685->ledFullOff(m_stepMotors[motor].in1A, true);
      m_pca9685->ledFullOff(m_stepMotors[motor].in2A, false);
      m_pca9685->ledFullOff(m_stepMotors[motor].in1B, false);
      m_pca9685->ledFullOff(m_stepMotors[motor].in2B, true);
      break;
    case 2:    //0101
      m_pca9685->ledFullOff(m_stepMotors[motor].in1A, true);
      m_pca9685->ledFullOff(m_stepMotors[motor].in2A, false);
      m_pca9685->ledFullOff(m_stepMotors[motor].in1B, true);
      m_pca9685->ledFullOff(m_stepMotors[motor].in2B, false);
      break;
    case 3:    //1001
      m_pca9685->ledFullOff(m_stepMotors[motor].in1A, false);
      m_pca9685->ledFullOff(m_stepMotors[motor].in2A, true);
      m_pca9685->ledFullOff(m_stepMotors[motor].in1B, true);
      m_pca9685->ledFullOff(m_stepMotors[motor].in2B, false);
      break;
    }
}

void AdafruitMS1438::stepperSteps(STEPMOTORS_T motor, unsigned int steps)
{
  while (steps > 0)
    {
      if (getMillis(motor) >= m_stepConfig[motor].stepDelay)
        {
          // reset the clock
          initClock(motor);

          m_stepConfig[motor].currentStep += m_stepConfig[motor].stepDirection;

          if (m_stepConfig[motor].stepDirection == 1)
            {
              if (m_stepConfig[motor].currentStep >= 
                  m_stepConfig[motor].stepsPerRev)
                m_stepConfig[motor].currentStep = 0;
            }
          else
            {
              if (m_stepConfig[motor].currentStep <= 0)
                m_stepConfig[motor].currentStep = 
                  m_stepConfig[motor].stepsPerRev;
            }

          steps--;
          stepperStep(motor);
        }
    }
}

