/*
 * Author: Aditya Nagal <adityax.nagal@intel.com>
 * Copyright (c) 2014 Intel Corporation.
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
#include <unistd.h>
#include <stdlib.h>
#include <math.h>
#include <errno.h>

#include "bme280.h"
#include "bme280driver.h"
/************** I2C buffer length ******/
#define I2C_BUFFER_LEN 26



using namespace upm;




/*----------------------------------------------------------------------------*
 *      struct bme280_t parameters can be accessed by using bme280
 *  bme280_t having the following parameters
 *  Bus write function pointer: BME280_WR_FUNC_PTR
    Bus read function pointer: BME280_RD_FUNC_PTR
 *  Delay function pointer: delay_msec
 *  I2C address: dev_addr
 *  Chip id of the sensor: chip_id
 *---------------------------------------------------------------------------*/

struct bme280_t bme280;

static struct bme280_t *p_bme280; /**< pointer to BME280 */
mraa::I2c* BME280::m_i2c = NULL;
int BME280::m_bus = 0;

BME280::BME280 (int bus, int devAddr) {
    m_bus = bus;
    if( m_i2c == NULL)
    {
            m_i2c = new mraa::I2c(m_bus);
        m_i2c->address(BME280_I2C_ADDRESS1);
        //Based on the requirement,  configure I2C interface.
        I2C_routine();
        /*--------------------------------------------------------------------------*
         *  This function used to assign the value/reference of
         *  the following parameters
         *  I2C address
         *  Bus Write
         *  Bus read
         *  Chip id
        *-------------------------------------------------------------------------*/
        bme280_init(&bme280);
    }
}

BME280::~BME280() {
    delete m_i2c;
}

/* This function is an example for reading sensor temperature
 *  \param: None
 *  \return: compensated temperature
 */

int32_t BME280::getTemperatureInternal(void)
{
    /* The variable used to read uncompensated temperature*/
    int32_t v_data_uncomp_tem_int32 =  getTemperatureRawInternal();

/*------------------------------------------------------------------*
************ START READ TRUE PRESSURE, TEMPERATURE AND HUMIDITY DATA ********
*---------------------------------------------------------------------*/
    /* API is used to read the true temperature*/
    /* Input value as uncompensated temperature and output format*/
    int32_t v_actual_temp_int32 = bme280_compensate_temperature_int32(v_data_uncomp_tem_int32);
/*--------------------------------------------------------------------*
************ END READ TRUE TEMPERATURE  ********
*-------------------------------------------------------------------------*/

return v_actual_temp_int32;
}

/* This function is an example for reading sensor pressure
 *  \param: None
 *  \return: compensated pressure
 */

int32_t BME280::getPressureInternal(void)
{
    /* The variable used to read uncompensated pressure*/
    int32_t v_data_uncomp_pres_int32 = getPressureRawInternal();


/*------------------------------------------------------------------*
************ START READ TRUE PRESSURE DATA ********
*---------------------------------------------------------------------*/
    /* API is used to read the true pressure*/
    /* Input value as uncompensated pressure */
    uint32_t v_actual_press_uint32 = bme280_compensate_pressure_int32(v_data_uncomp_pres_int32);

/*--------------------------------------------------------------------*
************ END READ TRUE PRESSURE ********
*-------------------------------------------------------------------------*/

return v_actual_press_uint32;
}


/* This function is an example for reading sensor humidity
 *  \param: None
 *  \return: compensated humidity
 */

int32_t BME280::getHumidityInternal(void)
{
    /* The variable used to read uncompensated pressure*/
    int32_t v_data_uncomp_hum_int32 = getHumidityRawInternal();

/*------------------------------------------------------------------*
************ START READ TRUE HUMIDITY DATA ********
*---------------------------------------------------------------------*/
    /* API is used to read the true humidity*/
    /* Input value as uncompensated humidity and output format*/
     uint32_t  v_actual_humity_uint32 = bme280_compensate_humidity_int32(v_data_uncomp_hum_int32);

/*--------------------------------------------------------------------*
************ END READ TRUE HUMIDITY ********
*-------------------------------------------------------------------------*/

return v_actual_humity_uint32;
}



/* This function is an example for reading sensor temperature
 *  \param: None
 *  \return: uncompensated temperature
 */

int32_t BME280::getTemperatureRawInternal(void)
{
    /* The variable used to read uncompensated temperature*/
    int32_t v_data_uncomp_tem_int32 = BME280_INIT_VALUE;

    /*  For initialization it is required to set the mode of
     *  the sensor as "NORMAL"
     *  data acquisition/read/write is possible in this mode
     *  by using the below API able to set the power mode as NORMAL*/
    /* Set the power mode as NORMAL*/
    bme280_set_power_mode(BME280_NORMAL_MODE);
    /*  For reading the  temperature data it is required to
     *  set the OSS setting of temperature
     * In the code automated reading and writing of "BME280_CTRLHUM_REG_OSRSH"
     * register first set the "BME280_CTRLHUM_REG_OSRSH" and then read and write
     * the "BME280_CTRLMEAS_REG" register in the function*/

    /* set the temperature oversampling*/
    bme280_set_oversamp_temperature(BME280_OVERSAMP_4X);

/************************* END INITIALIZATION *************************/

/*------------------------------------------------------------------*
************ START READ UNCOMPENSATED TEMPERATURE  DATA ********
*---------------------------------------------------------------------*/
    /* API is used to read the uncompensated temperature*/
    bme280_read_uncomp_temperature(&v_data_uncomp_tem_int32);


/*--------------------------------------------------------------------*
************ END READ UNCOMPENSATED TEMPERATURE  ********
*-------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------*
************************* START DE-INITIALIZATION ***********************
*-------------------------------------------------------------------------*/
    /*  For de-initialization it is required to set the mode of
     *  the sensor as "SLEEP"
     *  the device reaches the lowest power consumption only
     *  In SLEEP mode no measurements are performed
     *  All registers are accessible
     *  by using the below API able to set the power mode as SLEEP*/
     /* Set the power mode as SLEEP*/
    bme280_set_power_mode(BME280_SLEEP_MODE);
/*---------------------------------------------------------------------*
************************* END DE-INITIALIZATION **********************
*---------------------------------------------------------------------*/
return v_data_uncomp_tem_int32;
}

/* This function is an example for reading sensor pressure
 *  \param: None
 *  \return: uncompensated pressure
 */

int32_t BME280::getPressureRawInternal(void)
{
    /* The variable used to read uncompensated pressure*/
    int32_t v_data_uncomp_pres_int32 = BME280_INIT_VALUE;


    /*  For initialization it is required to set the mode of
     *  the sensor as "NORMAL"
     *  data acquisition/read/write is possible in this mode
     *  by using the below API able to set the power mode as NORMAL*/
    /* Set the power mode as NORMAL*/
    bme280_set_power_mode(BME280_NORMAL_MODE);
    /*  For reading the pressure data it is required to
     *  set the OSS setting of humidity, pressure and temperature
     * The "BME280_CTRLHUM_REG_OSRSH" register sets the humidity
     * data acquisition options of the device.
     * changes to this registers only become effective after a write operation to
     * "BME280_CTRLMEAS_REG" register.
     * In the code automated reading and writing of "BME280_CTRLHUM_REG_OSRSH"
     * register first set the "BME280_CTRLHUM_REG_OSRSH" and then read and write
     * the "BME280_CTRLMEAS_REG" register in the function*/

    /* set the pressure oversampling*/
    bme280_set_oversamp_pressure(BME280_OVERSAMP_2X);

/************************* END INITIALIZATION *************************/

/*------------------------------------------------------------------*
************ START READ UNCOMPENSATED PRESSURE DATA ********
*---------------------------------------------------------------------*/

    /* API is used to read the uncompensated pressure*/
    bme280_read_uncomp_pressure(&v_data_uncomp_pres_int32);

/*--------------------------------------------------------------------*
************ END READ UNCOMPENSATED PRESSURE ********
*-------------------------------------------------------------------------*/


/*-----------------------------------------------------------------------*
************************* START DE-INITIALIZATION ***********************
*-------------------------------------------------------------------------*/
    /*  For de-initialization it is required to set the mode of
     *  the sensor as "SLEEP"
     *  the device reaches the lowest power consumption only
     *  In SLEEP mode no measurements are performed
     *  All registers are accessible
     *  by using the below API able to set the power mode as SLEEP*/
     /* Set the power mode as SLEEP*/
    bme280_set_power_mode(BME280_SLEEP_MODE);
/*---------------------------------------------------------------------*
************************* END DE-INITIALIZATION **********************
*---------------------------------------------------------------------*/
return v_data_uncomp_pres_int32;
}


/* This function is an example for reading sensor humidity
 *  \param: None
 *  \return: uncompensated humidity
 */

int32_t BME280::getHumidityRawInternal(void)
{
    /* The variable used to read uncompensated pressure*/
    int32_t v_data_uncomp_hum_int32 = BME280_INIT_VALUE;

    /*  For initialization it is required to set the mode of
     *  the sensor as "NORMAL"
     *  data acquisition/read/write is possible in this mode
     *  by using the below API able to set the power mode as NORMAL*/
    /* Set the power mode as NORMAL*/
    bme280_set_power_mode(BME280_NORMAL_MODE);
    /*  For reading  humidity  data it is required to
     *  set the OSS setting of humidity
     * The "BME280_CTRLHUM_REG_OSRSH" register sets the humidity
     * data acquisition options of the device.
     * changes to this registers only become effective after a write operation to
     * "BME280_CTRLMEAS_REG" register.
     * In the code automated reading and writing of "BME280_CTRLHUM_REG_OSRSH"
     * register first set the "BME280_CTRLHUM_REG_OSRSH" and then read and write
     * the "BME280_CTRLMEAS_REG" register in the function*/
    bme280_set_oversamp_humidity(BME280_OVERSAMP_1X);


/************************* END INITIALIZATION *************************/

/*------------------------------------------------------------------*
************ START READ HUMIDITY DATA ********
*---------------------------------------------------------------------*/
    /* API is used to read the uncompensated humidity*/
        bme280_read_uncomp_humidity(&v_data_uncomp_hum_int32);

/*--------------------------------------------------------------------*
************ END READ UNCOMPENSATED PRESSURE AND TEMPERATURE********
*-------------------------------------------------------------------------*/


/*-----------------------------------------------------------------------*
************************* START DE-INITIALIZATION ***********************
*-------------------------------------------------------------------------*/
    /*  For de-initialization it is required to set the mode of
     *  the sensor as "SLEEP"
     *  the device reaches the lowest power consumption only
     *  In SLEEP mode no measurements are performed
     *  All registers are accessible
     *  by using the below API able to set the power mode as SLEEP*/
     /* Set the power mode as SLEEP*/
    bme280_set_power_mode(BME280_SLEEP_MODE);
/*---------------------------------------------------------------------*
************************* END DE-INITIALIZATION **********************
*---------------------------------------------------------------------*/
return v_data_uncomp_hum_int32;
}





/*--------------------------------------------------------------------------*
*   The following function is used to map the I2C bus read, write, delay and
*   device address with global structure bme280
*-------------------------------------------------------------------------*/
int8_t BME280::I2C_routine()
{
/*--------------------------------------------------------------------------*
 *  By using bme280 the following structure parameter can be accessed
 *  Bus write function pointer: BME280_WR_FUNC_PTR
 *  Bus read function pointer: BME280_RD_FUNC_PTR
 *  Delay function pointer: delay_msec
 *  I2C address: dev_addr
 *--------------------------------------------------------------------------*/
//  bme280.bus_write = &BME280::BME280_I2C_bus_write;
    bme280.bus_write = BME280_I2C_bus_write;

    //bme280.bus_write = BME280_I2C_bus_write_dummy;
    bme280.bus_read = BME280_I2C_bus_read;
    bme280.dev_addr = BME280_I2C_ADDRESS1;
    bme280.delay_msec = BME280_delay_msek;

    return BME280_INIT_VALUE;
}



/*-------------------------------------------------------------------*
*   The device address defined in the bme280.h file
*-----------------------------------------------------------------------*/
int32_t BME280::i2c_write_string(uint8_t dev_addr,uint8_t* ptr, uint8_t cnt)
{
    mraa::Result ret;
    m_i2c->address(dev_addr);

    if((ret = m_i2c->write((const uint8_t*) (ptr), cnt)) != 0)
    {
        UPM_THROW("I2C write error");
    }
    return 0;
}

/*  \Brief: The function is used as I2C bus write
*   \Return : Status of the I2C write
*   \param dev_addr : The device address of the sensor
*   \param reg_addr : Address of the first register, will data is going to be written
*   \param reg_data : It is a value hold in the array,
*       will be used for write the value into the register
*   \param cnt : The no of byte of data to be write
*/

int8_t BME280::BME280_I2C_bus_write(uint8_t dev_addr, uint8_t reg_addr, uint8_t *reg_data, uint8_t cnt)

{
    int32_t iError = BME280_INIT_VALUE;
    static uint8_t array[I2C_BUFFER_LEN];
        for (int i=0; i<I2C_BUFFER_LEN; i++) array[i]=0;

    uint8_t stringpos = BME280_INIT_VALUE;
    array[BME280_INIT_VALUE] = reg_addr;
    for (stringpos = BME280_INIT_VALUE; stringpos < cnt; stringpos++) {
        array[stringpos + BME280_ONE_U8X] = *(reg_data + stringpos);
    }
    iError = i2c_write_string(dev_addr,array, cnt+1);
    return (int8_t)iError;
}

int32_t BME280::i2c_write_read_string(uint8_t dev_addr,uint8_t reg_addr , uint8_t * ptr, uint8_t cnt)
{
     mraa::Result ret;

    m_i2c->address(dev_addr);

    if( m_i2c->readBytesReg(reg_addr, ptr, cnt) != cnt)
    {
        UPM_THROW("bme280 register read failed");

    }
    return 0;
}

 /* \Brief: The function is used as I2C bus read
 *  \Return : Status of the I2C read
 *  \param dev_addr : The device address of the sensor
 *  \param reg_addr : Address of the first register, will data is going to be read
 *  \param reg_data : This data read from the sensor, which is hold in an array
 *  \param cnt : The no of data byte of to be read
 */
int8_t BME280::BME280_I2C_bus_read(uint8_t dev_addr, uint8_t reg_addr, uint8_t *reg_data, uint8_t cnt)
{
    int32_t iError = BME280_INIT_VALUE;
    uint8_t array[I2C_BUFFER_LEN] = {BME280_INIT_VALUE};
    uint8_t stringpos = BME280_INIT_VALUE;
    array[BME280_INIT_VALUE] = reg_addr;
    i2c_write_read_string(dev_addr,reg_addr,array,cnt);
    for (stringpos = BME280_INIT_VALUE; stringpos < cnt; stringpos++) {
        *(reg_data + stringpos) = array[stringpos];
    }
    return (int8_t)iError;
}

/*  Brief : The delay routine
 *  \param : delay in ms
*/
void BME280::BME280_delay_msek(uint16_t mseconds)
{
    struct timespec sleepTime;

    sleepTime.tv_sec = mseconds / 1000; // Number of seconds
    sleepTime.tv_nsec = ( mseconds % 1000 ) * 1000000; // Convert fractional seconds to nanoseconds

    // Iterate nanosleep in a loop until the total sleep time is the original
    // value of the seconds parameter
    while ( ( nanosleep( &sleepTime, &sleepTime ) != 0 ) && ( errno == EINTR ) );
}


/**
 * Get temperature measurement.
 */
uint16_t BME280::getTemperatureRaw (){ return BME280::getTemperatureRawInternal(); }

/**
 * Get temperature measurement.
 */
int BME280::getTemperatureCelcius (){ return (BME280::getTemperatureInternal() + 50) /100; }
/**
 * Get relative humidity measurement.
 */
uint16_t BME280::getHumidityRaw (){ return BME280::getHumidityRawInternal(); }

/**
 * Get relative humidity measurement.
 */
int BME280::getHumidityRelative (){ return (BME280::getHumidityInternal() + 500) / 1000; }

/**
 * Return pressure
 */
uint32_t BME280::getPressureRaw(){  return BME280::getPressureRawInternal(); }

/**
 * Return calculated pressure (Pa)
 */
int BME280::getPressurePa(){ return BME280::getPressureInternal(); }



