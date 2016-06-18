/*
 * The MIT License (MIT)
 *
 * Author: Oussema Harbi <oussema.elharbi@gmail.com>
 * Copyright (c) <2016> <Oussema Harbi>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#pragma once

#include <mraa/i2c.hpp>

//We can use direct integer IDs,
//or we can use the typedef here to help limit the error cases
//and still support extension to support more Motors in the future
//But when using typedef, we need to cast these when sending them in i2c bus
#define    SmartDrive_Motor_ID_1     0x01
#define    SmartDrive_Motor_ID_2     0x02
#define    SmartDrive_Motor_ID_BOTH  0x03

#define    SmartDrive_Dir_Reverse  0x00
#define    SmartDrive_Dir_Forward  0x01

#define    SmartDrive_Action_Float      0x00  //stop and let the motor coast.
#define    SmartDrive_Action_Brake      0x01  //apply brakes, and resist change to tachometer
#define    SmartDrive_Action_BrakeHold  0x02  //apply brakes, and restore externally forced change to tachometer

//Next action (upon completion of current action)
#define    SmartDrive_Completion_Wait_For   0x01
#define    SmartDrive_Completion_Dont_Wait  0x00

#define    SmartDrive_DefaultAddress      0x1b
#define    SmartDrive_VOLTAGE_MULTIPLIER  212.7

//Commonly used speed constants, these are just convenience constants
//You can use any value between 0 and 100.
#define    SmartDrive_Speed_Full    90
#define    SmartDrive_Speed_Medium  60
#define    SmartDrive_Speed_Slow    25

//Different commands
#define    SmartDrive_CONTROL_SPEED    0x01
#define    SmartDrive_CONTROL_RAMP     0x02
#define    SmartDrive_CONTROL_RELATIVE 0x04
#define    SmartDrive_CONTROL_TACHO    0x08
#define    SmartDrive_CONTROL_BRK      0x10
#define    SmartDrive_CONTROL_ON       0x20
#define    SmartDrive_CONTROL_TIME     0x40
#define    SmartDrive_CONTROL_GO       0x80

#define    SmartDrive_COMMAND          0x41
#define    SmartDrive_SETPT_M1         0x42
#define    SmartDrive_SPEED_M1         0x46
#define    SmartDrive_TIME_M1          0x47
#define    SmartDrive_CMD_B_M1         0x48
#define    SmartDrive_CMD_A_M1         0x49

#define    SmartDrive_SETPT_M2         0x4A
#define    SmartDrive_SPEED_M2         0x4E
#define    SmartDrive_TIME_M2          0x4F
#define    SmartDrive_CMD_B_M2         0x50
#define    SmartDrive_CMD_A_M2         0x51

//Read registers.
#define    SmartDrive_POSITION_M1      0x52
#define    SmartDrive_POSITION_M2      0x56
#define    SmartDrive_STATUS_M1        0x5A
#define    SmartDrive_STATUS_M2        0x5B
#define    SmartDrive_TASKS_M1         0x5C
#define    SmartDrive_TASKS_M2         0x5D

//PID control registers
#define    SmartDrive_P_Kp             0x5E   //proportional gain-position
#define    SmartDrive_P_Ki             0x60   //integral gain-position
#define    SmartDrive_P_Kd             0x62   //derivative gain-position
#define    SmartDrive_S_Kp             0x64   //proportional gain-speed
#define    SmartDrive_S_Ki             0x66   //integral gain-speed
#define    SmartDrive_S_Kd             0x68   //derivative gain-speed
#define    SmartDrive_PASSCOUNT        0x6A
#define    SmartDrive_PASSTOLERANCE    0x6B

#define    SmartDrive_CHKSUM    0x6C

//Power data registers
#define    SmartDrive_BATT_VOLTAGE   0x6E
#define    SmartDrive_RESETSTATUS    0x6F
#define    SmartDrive_CURRENT_M1     0x70
#define    SmartDrive_CURRENT_M2     0x72

//Supported I2C commands
#define  CMD_R  0x52
#define  CMD_S  0x53
#define  CMD_a  0x61
#define  CMD_b  0x62
#define  CMD_c  0x63
#define  CMD_A  0x41
#define  CMD_B  0x42
#define  CMD_C  0x43

//Motor Status Masks
#define SmartDrive_MOTOR_CONTROL_ON        0x1
#define SmartDrive_MOTOR_IS_RAMPING        0x2
#define SmartDrive_MOTOR_IS_POWERED        0x4
#define SmartDrive_MOTOR_POS_CTRL_ON       0x8
#define SmartDrive_MOTOR_IN_BRAKE_MODE     0x10
#define SmartDrive_MOTOR_OVERLOADED        0x20
#define SmartDrive_MOTOR_IN_TIME_MODE      0x40
#define SmartDrive_MOTOR_IS_STALLED        0x80

namespace upm {

/**
 * @brief SmartDrive library
 * @defgroup smartdrive libupm-smartdrive
 * @ingroup i2c motor openelectrons
 */

/**
 * @library smartdrive
 * @sensor smartdrive
 * @comname SmartDrive advanced motor controller
 * @altname smartdrive
 * @type motor
 * @man openelectrons
 * @con i2c
 *
 * @brief API for the SmartDrive advanced motor controller from OpenElectronis
 *
 * SmartDrive is a multiplexer to control high current DC motors
 *
 * This module has been tested on the SmartDrive.
 *
 * @snippet smartdrive.cxx Interesting
 */
//Class definition
class SmartDrive {

public:
    /**
     * Initialize the class with the i2c address of your SmartDrive
     * @param SmartDrive_address Address of your SmartDrive.
     */
    SmartDrive(int i2c_bus, int address = SmartDrive_DefaultAddress);

    /**
     * Writes a specified command on the command register of the SmartDrive
     * @param cmd The command you wish the SmartDrive to execute.
     */
    void command(uint8_t cmd);

    /**
     * Reads the battery voltage. Multiplier constant not yet verified
     */
    float GetBattVoltage();

    /**
     * Reads the tacheometer position of the specified motor
     * @param motor_id Number of the motor you wish to read.
     */
    uint32_t ReadTachometerPosition(int motor_id);

    /**
     * Turns the specified motor(s) forever
     * @param motor_id Number of the motor(s) you wish to turn.
     * @param direction The direction you wish to turn the motor(s).
     * @param speed The speed at which you wish to turn the motor(s).
     */
    void Run_Unlimited(int motor_id, int direction, uint8_t speed);

    /**
     * Stops the specified motor(s)
     * @param motor_id Number of the motor(s) you wish to turn.
     * @param next_action How you wish to stop the motor(s).
     */
    void StopMotor(int motor_id, int next_action );

    /**
     * Turns the specified motor(s) for a given amount of seconds
     * @param motor_id Number of the motor(s) you wish to turn.
     * @param direction The direction you wish to turn the motor(s).
     * @param speed The speed at which you wish to turn the motor(s).
     * @param duration The time in seconds you wish to turn the motor(s).
     * @param wait_for_completion Tells the program when to handle the next line of code.
     * @param next_action How you wish to stop the motor(s).
     */
    void Run_Seconds(int motor_id, int direction, uint8_t speed, uint8_t duration, bool wait_for_completion, int next_action );

    /**
     * Waits until the specified time for the motor(s) to run is completed
     * @param motor_id Number of the motor(s) to wait for.
     */
    void WaitUntilTimeDone(int motor_id);

    /**
     * Checks to ensure the specified time for the motor(s) to run is completed.
     * @param motor_id Number of the motor(s) to check.
     */
    bool IsTimeDone(int motor_id);

    /**
     * Turns the specified motor(s) for given relative tacheometer count
     * @param motor_id Number of the motor(s) you wish to turn.
     * @param direction The direction you wish to turn the motor(s).
     * @param speed The speed at which you wish to turn the motor(s).
     * @param degrees The relative tacheometer count you wish to turn the motor(s).
     * @param wait_for_completion Tells the program when to handle the next line of code.
     * @param next_action How you wish to stop the motor(s).
     */
    void Run_Degrees(int motor_id, int direction, uint8_t speed, uint32_t degrees, bool wait_for_completion, int next_action);

    /**
     * Turns the specified motor(s) for given relative tacheometer count
     * @param motor_id Number of the motor(s) you wish to turn.
     * @param direction The direction you wish to turn the motor(s).
     * @param speed The speed at which you wish to turn the motor(s).
     * @param rotations The relative amount of rotations you wish to turn the motor(s).
     * @param wait_for_completion Tells the program when to handle the next line of code.
     * @param next_action How you wish to stop the motor(s).
     */
    void Run_Rotations(int motor_id, int direction, uint8_t speed, uint32_t rotations, bool wait_for_completion, int next_action);

    /**
     * Turns the specified motor(s) for given absolute tacheometer count
     * @param motor_id Number of the motor(s) you wish to turn.
     * @param direction The direction you wish to turn the motor(s).
     * @param speed The speed at which you wish to turn the motor(s).
     * @param tacho_count The absolute tacheometer count you wish to turn the motor(s).
     * @param wait_for_completion Tells the program when to handle the next line of code.
     * @param next_action How you wish to stop the motor(s).
     */
    void Run_Tacho(int motor_id, uint8_t speed, uint32_t tacho_count, bool wait_for_completion, int next_action);

    /**
     * Waits until the specified tacheomter count for the motor(s) to run is reached.
     * @param motor_id Number of the motor(s) to wait for.
     */
    void WaitUntilTachoDone(int motor_id);

    /**
     * Checks to ensure the specified tacheomter count for the motor(s) to run is reached.
     * @param motor_id Number of the motor(s) to check.
     */
    bool IsTachoDone(int motor_id);

    /**
     * Writes user specified values to the PID control registers
     * @param Kp_tacho Proportional-gain of the tacheometer position of the motor.
     * @param Ki_tacho Integral-gain of the tacheometer position of the motor.
     * @param Kd_tacho Derivative-gain of the tacheometer position of the motor.
     * @param Kp_speed Proportional-gain of the speed of the motor.
     * @param Ki_speed Integral-gain of the speed of the motor.
     * @param Kd_speed Derivative-gain of the speed of the motor.
     */
    void SetPerformanceParameters( uint16_t Kp_tacho, uint16_t Ki_tacho, uint16_t Kd_tacho, uint16_t Kp_speed, uint16_t Ki_speed, uint16_t Kd_speed, uint8_t passcount, uint8_t tolerance);

    /**
     * Reads the values of the PID control registers
     */
    void ReadPerformanceParameters();

    /**
     * Read the status of a motor, and return it in a uint8_t
     * param motor_id Number fo the motor to check
     */
    uint8_t GetMotorStatus(int motor_id);

    /**
     * Print the detailed status of the motor
     * @param motor_id Number fo the motor to check
     */
    void PrintMotorStatus(int motor_id);

private:
    void writeByte(uint8_t addr, uint8_t value);
    void writeArray(uint8_t* array);
    uint8_t readByte(uint8_t addr);
    uint16_t readInteger(uint8_t addr);
    uint32_t readLongSigned(uint8_t addr);

private:
    int m_smartdrive_control_address;
    mraa::I2c m_i2c_smartdrive_control;

};

}
