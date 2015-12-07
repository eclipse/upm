/*
****************************************************************************
* Copyright (C) 2013 - 2015 Bosch Sensortec GmbH
*
* bme280.c
* Date: 2015/03/27
* Revision: 2.0.4(Pressure and Temperature compensation code revision is 1.1
*               and Humidity compensation code revision is 1.0)
*
* Usage: Sensor Driver file for BME280 sensor
*
****************************************************************************
* License:
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted provided that the following conditions are met:
*
*   Redistributions of source code must retain the above copyright
*   notice, this list of conditions and the following disclaimer.
*
*   Redistributions in binary form must reproduce the above copyright
*   notice, this list of conditions and the following disclaimer in the
*   documentation and/or other materials provided with the distribution.
*
*   Neither the name of the copyright holder nor the names of the
*   contributors may be used to endorse or promote products derived from
*   this software without specific prior written permission.
*
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND
* CONTRIBUTORS "AS IS" AND ANY EXPRESS OR
* IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
* WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
* DISCLAIMED. IN NO EVENT SHALL COPYRIGHT HOLDER
* OR CONTRIBUTORS BE LIABLE FOR ANY
* DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY,
* OR CONSEQUENTIAL DAMAGES(INCLUDING, BUT NOT LIMITED TO,
* PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
* LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
* HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
* WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
* (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
* ANY WAY OUT OF THE USE OF THIS
* SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE
*
* The information provided is believed to be accurate and reliable.
* The copyright holder assumes no responsibility
* for the consequences of use
* of such information nor for any infringement of patents or
* other rights of third parties which may result from its use.
* No license is granted by implication or otherwise under any patent or
* patent rights of the copyright holder.
**************************************************************************/
#include <iostream>
#include <unistd.h>
#include <stdlib.h>
#include <math.h>
#include <errno.h>

#include "bme280.h"

/************** I2C buffer length ******/
#define	I2C_BUFFER_LEN 26



using namespace upm;




/*----------------------------------------------------------------------------*
 *  struct bme280_t parameters can be accessed by using bme280
 *	bme280_t having the following parameters
 *	Bus write function pointer: BME280_WR_FUNC_PTR
 	Bus read function pointer: BME280_RD_FUNC_PTR
 *	Delay function pointer: delay_msec
 *	I2C address: dev_addr
 *	Chip id of the sensor: chip_id
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
		//Based on the user need configure I2C interface.
		I2C_routine();
		/*--------------------------------------------------------------------------*
		 *  This function used to assign the value/reference of
		 *	the following parameters
		 *	I2C address
		 *	Bus Write
		 *	Bus read
		 *	Chip id
		*-------------------------------------------------------------------------*/	
		bme280_init(&bme280);
	}
}

BME280::~BME280() {
    delete m_i2c;
}

bool
BME280::isAvailable() {
    return true;
}

bool
BME280::isConfigured() {
   // return configured;
	return true ;
}


/*!
 *	@brief This function is used for initialize
 *	the bus read and bus write functions
 *  and assign the chip id and I2C address of the BME280 sensor
 *	chip id is read in the register 0xD0 bit from 0 to 7
 *
 *	 @param bme280 structure pointer.
 *
 *	@note While changing the parameter of the bme280_t
 *	@note consider the following point:
 *	Changing the reference value of the parameter
 *	will changes the local copy or local reference
 *	make sure your changes will not
 *	affect the reference value of the parameter
 *	(Better case don't change the reference value of the parameter)
 *
 *
 *
 *
 *	@return results of bus communication function
 *	@retval 0 -> Success
 *	@retval -1 -> Error
 *
 *
*/
BME280_RETURN_FUNCTION_TYPE BME280::bme280_init(struct bme280_t *bme280)
{
	/* used to return the communication result*/
	BME280_RETURN_FUNCTION_TYPE com_rslt = ERROR;
	u8 v_data_u8 = BME280_INIT_VALUE;

	p_bme280 = bme280;
	/* assign BME280 ptr */
	com_rslt = p_bme280->BME280_BUS_READ_FUNC(p_bme280->dev_addr,
	BME280_CHIP_ID_REG, &v_data_u8,
	BME280_GEN_READ_WRITE_DATA_LENGTH);
	/* read Chip Id */
	p_bme280->chip_id = v_data_u8;

	com_rslt += bme280_get_calib_param();
	/* readout bme280 calibparam structure */
	return com_rslt;
}
/*!
 *	@brief This API is used to read uncompensated temperature
 *	in the registers 0xFA, 0xFB and 0xFC
 *	@note 0xFA -> MSB -> bit from 0 to 7
 *	@note 0xFB -> LSB -> bit from 0 to 7
 *	@note 0xFC -> LSB -> bit from 4 to 7
 *
 * @param v_uncomp_temperature_s32 : The value of uncompensated temperature
 *
 *
 *
 *	@return results of bus communication function
 *	@retval 0 -> Success
 *	@retval -1 -> Error
 *
 *
*/
BME280_RETURN_FUNCTION_TYPE BME280::bme280_read_uncomp_temperature(
s32 *v_uncomp_temperature_s32)
{
	/* used to return the communication result*/
	BME280_RETURN_FUNCTION_TYPE com_rslt = ERROR;
	/* Array holding the MSB and LSb value
	a_data_u8r[0] - Temperature MSB
	a_data_u8r[1] - Temperature LSB
	a_data_u8r[2] - Temperature XLSB
	*/
	u8 a_data_u8r[BME280_TEMPERATURE_DATA_SIZE] = {
	BME280_INIT_VALUE, BME280_INIT_VALUE, BME280_INIT_VALUE};
	/* check the p_bme280 structure pointer as NULL*/
	if (p_bme280 == BME280_NULL) {
		return E_BME280_NULL_PTR;
		} else {
			com_rslt = p_bme280->BME280_BUS_READ_FUNC(
			p_bme280->dev_addr,
			BME280_TEMPERATURE_MSB_REG,
			a_data_u8r,
			BME280_TEMPERATURE_DATA_LENGTH);
			*v_uncomp_temperature_s32 = (s32)(((
			(u32) (a_data_u8r[BME280_TEMPERATURE_MSB_DATA]))
			<< BME280_SHIFT_BIT_POSITION_BY_12_BITS) |
			(((u32)(a_data_u8r[BME280_TEMPERATURE_LSB_DATA]))
			<< BME280_SHIFT_BIT_POSITION_BY_04_BITS)
			| ((u32)a_data_u8r[BME280_TEMPERATURE_XLSB_DATA] >>
			BME280_SHIFT_BIT_POSITION_BY_04_BITS));
		}
	return com_rslt;
}
/*!
 * @brief Reads actual temperature from uncompensated temperature
 * @note Returns the value in 0.01 degree Centigrade
 * Output value of "5123" equals 51.23 DegC.
 *
 *
 *
 *  @param  v_uncomp_temperature_s32 : value of uncompensated temperature
 *
 *
 *  @return Returns the actual temperature
 *
*/
s32 BME280::bme280_compensate_temperature_int32(s32 v_uncomp_temperature_s32)
{
	s32 v_x1_u32r = BME280_INIT_VALUE;
	s32 v_x2_u32r = BME280_INIT_VALUE;
	s32 temperature = BME280_INIT_VALUE;

	/* calculate x1*/
	v_x1_u32r  =
	((((v_uncomp_temperature_s32
	>> BME280_SHIFT_BIT_POSITION_BY_03_BITS) -
	((s32)p_bme280->cal_param.dig_T1
	<< BME280_SHIFT_BIT_POSITION_BY_01_BIT))) *
	((s32)p_bme280->cal_param.dig_T2)) >>
	BME280_SHIFT_BIT_POSITION_BY_11_BITS;
	/* calculate x2*/
	v_x2_u32r  = (((((v_uncomp_temperature_s32
	>> BME280_SHIFT_BIT_POSITION_BY_04_BITS) -
	((s32)p_bme280->cal_param.dig_T1))
	* ((v_uncomp_temperature_s32 >> BME280_SHIFT_BIT_POSITION_BY_04_BITS) -
	((s32)p_bme280->cal_param.dig_T1)))
	>> BME280_SHIFT_BIT_POSITION_BY_12_BITS) *
	((s32)p_bme280->cal_param.dig_T3))
	>> BME280_SHIFT_BIT_POSITION_BY_14_BITS;
	/* calculate t_fine*/
	p_bme280->cal_param.t_fine = v_x1_u32r + v_x2_u32r;
	/* calculate temperature*/
	temperature  = (p_bme280->cal_param.t_fine * 5 + 128)
	>> BME280_SHIFT_BIT_POSITION_BY_08_BITS;
	return temperature;
}
/*!
 * @brief Reads actual temperature from uncompensated temperature
 * @note Returns the value with 500LSB/DegC centred around 24 DegC
 * output value of "5123" equals(5123/500)+24 = 34.246DegC
 *
 *
 *  @param v_uncomp_temperature_s32: value of uncompensated temperature
 *
 *
 *
 *  @return Return the actual temperature as s16 output
 *
*/
s16 BME280::bme280_compensate_temperature_int32_sixteen_bit_output(
s32 v_uncomp_temperature_s32)
{
	s16 temperature = BME280_INIT_VALUE;

	bme280_compensate_temperature_int32(
	v_uncomp_temperature_s32);
	temperature  = (s16)((((
	p_bme280->cal_param.t_fine - 122880) * 25) + 128)
	>> BME280_SHIFT_BIT_POSITION_BY_08_BITS);

	return temperature;
}
/*!
 *	@brief This API is used to read uncompensated pressure.
 *	in the registers 0xF7, 0xF8 and 0xF9
 *	@note 0xF7 -> MSB -> bit from 0 to 7
 *	@note 0xF8 -> LSB -> bit from 0 to 7
 *	@note 0xF9 -> LSB -> bit from 4 to 7
 *
 *
 *
 *	@param v_uncomp_pressure_s32 : The value of uncompensated pressure
 *
 *
 *
 *	@return results of bus communication function
 *	@retval 0 -> Success
 *	@retval -1 -> Error
 *
 *
*/
BME280_RETURN_FUNCTION_TYPE BME280::bme280_read_uncomp_pressure(
s32 *v_uncomp_pressure_s32)
{
	/* used to return the communication result*/
	BME280_RETURN_FUNCTION_TYPE com_rslt = ERROR;
	/* Array holding the MSB and LSb value
	a_data_u8[0] - Pressure MSB
	a_data_u8[1] - Pressure LSB
	a_data_u8[2] - Pressure XLSB
	*/
	u8 a_data_u8[BME280_PRESSURE_DATA_SIZE] = {
	BME280_INIT_VALUE, BME280_INIT_VALUE, BME280_INIT_VALUE};
	/* check the p_bme280 structure pointer as NULL*/
	if (p_bme280 == BME280_NULL) {
		return E_BME280_NULL_PTR;
		} else {
			com_rslt = p_bme280->BME280_BUS_READ_FUNC(
			p_bme280->dev_addr,
			BME280_PRESSURE_MSB_REG,
			a_data_u8, BME280_PRESSURE_DATA_LENGTH);
			*v_uncomp_pressure_s32 = (s32)((
			((u32)(a_data_u8[BME280_PRESSURE_MSB_DATA]))
			<< BME280_SHIFT_BIT_POSITION_BY_12_BITS) |
			(((u32)(a_data_u8[BME280_PRESSURE_LSB_DATA]))
			<< BME280_SHIFT_BIT_POSITION_BY_04_BITS) |
			((u32)a_data_u8[BME280_PRESSURE_XLSB_DATA] >>
			BME280_SHIFT_BIT_POSITION_BY_04_BITS));
		}
	return com_rslt;
}
/*!
 * @brief Reads actual pressure from uncompensated pressure
 * @note Returns the value in Pascal(Pa)
 * Output value of "96386" equals 96386 Pa =
 * 963.86 hPa = 963.86 millibar
 *
 *
 *
 *  @param v_uncomp_pressure_s32 : value of uncompensated pressure
 *
 *
 *
 *  @return Return the actual pressure output as u32
 *
*/
u32 BME280::bme280_compensate_pressure_int32(s32 v_uncomp_pressure_s32)
{
	s32 v_x1_u32 = BME280_INIT_VALUE;
	s32 v_x2_u32 = BME280_INIT_VALUE;
	u32 v_pressure_u32 = BME280_INIT_VALUE;

	/* calculate x1*/
	v_x1_u32 = (((s32)p_bme280->cal_param.t_fine)
	>> BME280_SHIFT_BIT_POSITION_BY_01_BIT) - (s32)64000;
	/* calculate x2*/
	v_x2_u32 = (((v_x1_u32 >> BME280_SHIFT_BIT_POSITION_BY_02_BITS)
	* (v_x1_u32 >> BME280_SHIFT_BIT_POSITION_BY_02_BITS)
	) >> BME280_SHIFT_BIT_POSITION_BY_11_BITS)
	* ((s32)p_bme280->cal_param.dig_P6);
	/* calculate x2*/
	v_x2_u32 = v_x2_u32 + ((v_x1_u32 *
	((s32)p_bme280->cal_param.dig_P5))
	<< BME280_SHIFT_BIT_POSITION_BY_01_BIT);
	/* calculate x2*/
	v_x2_u32 = (v_x2_u32 >> BME280_SHIFT_BIT_POSITION_BY_02_BITS) +
	(((s32)p_bme280->cal_param.dig_P4)
	<< BME280_SHIFT_BIT_POSITION_BY_16_BITS);
	/* calculate x1*/
	v_x1_u32 = (((p_bme280->cal_param.dig_P3 *
	(((v_x1_u32 >> BME280_SHIFT_BIT_POSITION_BY_02_BITS) *
	(v_x1_u32 >> BME280_SHIFT_BIT_POSITION_BY_02_BITS))
	>> BME280_SHIFT_BIT_POSITION_BY_13_BITS))
	>> BME280_SHIFT_BIT_POSITION_BY_03_BITS) +
	((((s32)p_bme280->cal_param.dig_P2) *
	v_x1_u32) >> BME280_SHIFT_BIT_POSITION_BY_01_BIT))
	>> BME280_SHIFT_BIT_POSITION_BY_18_BITS;
	/* calculate x1*/
	v_x1_u32 = ((((32768 + v_x1_u32)) *
	((s32)p_bme280->cal_param.dig_P1))
	>> BME280_SHIFT_BIT_POSITION_BY_15_BITS);
	/* calculate pressure*/
	v_pressure_u32 =
	(((u32)(((s32)1048576) - v_uncomp_pressure_s32)
	- (v_x2_u32 >> BME280_SHIFT_BIT_POSITION_BY_12_BITS))) * 3125;
	if (v_pressure_u32
	< 0x80000000)
		/* Avoid exception caused by division by zero */
		if (v_x1_u32 != BME280_INIT_VALUE)
			v_pressure_u32 =
			(v_pressure_u32
			<< BME280_SHIFT_BIT_POSITION_BY_01_BIT) /
			((u32)v_x1_u32);
		else
			return BME280_INVALID_DATA;
	else
		/* Avoid exception caused by division by zero */
		if (v_x1_u32 != BME280_INIT_VALUE)
			v_pressure_u32 = (v_pressure_u32
			/ (u32)v_x1_u32) * 2;
		else
			return BME280_INVALID_DATA;

		v_x1_u32 = (((s32)p_bme280->cal_param.dig_P9) *
		((s32)(((v_pressure_u32 >> BME280_SHIFT_BIT_POSITION_BY_03_BITS)
		* (v_pressure_u32 >> BME280_SHIFT_BIT_POSITION_BY_03_BITS))
		>> BME280_SHIFT_BIT_POSITION_BY_13_BITS)))
		>> BME280_SHIFT_BIT_POSITION_BY_12_BITS;
		v_x2_u32 = (((s32)(v_pressure_u32
		>> BME280_SHIFT_BIT_POSITION_BY_02_BITS)) *
		((s32)p_bme280->cal_param.dig_P8))
		>> BME280_SHIFT_BIT_POSITION_BY_13_BITS;
		v_pressure_u32 = (u32)((s32)v_pressure_u32 +
		((v_x1_u32 + v_x2_u32 + p_bme280->cal_param.dig_P7)
		>> BME280_SHIFT_BIT_POSITION_BY_04_BITS));

	return v_pressure_u32;
}
/*!
 *	@brief This API is used to read uncompensated humidity.
 *	in the registers 0xF7, 0xF8 and 0xF9
 *	@note 0xFD -> MSB -> bit from 0 to 7
 *	@note 0xFE -> LSB -> bit from 0 to 7
 *
 *
 *
 *	@param v_uncomp_humidity_s32 : The value of uncompensated humidity
 *
 *
 *
 *	@return results of bus communication function
 *	@retval 0 -> Success
 *	@retval -1 -> Error
 *
 *
*/
BME280_RETURN_FUNCTION_TYPE BME280::bme280_read_uncomp_humidity(
s32 *v_uncomp_humidity_s32)
{
	/* used to return the communication result*/
	BME280_RETURN_FUNCTION_TYPE com_rslt = ERROR;
	/* Array holding the MSB and LSb value
	a_data_u8[0] - Humidity MSB
	a_data_u8[1] - Humidity LSB
	*/
	u8 a_data_u8[BME280_HUMIDITY_DATA_SIZE] = {
	BME280_INIT_VALUE, BME280_INIT_VALUE};
	/* check the p_bme280 structure pointer as NULL*/
	if (p_bme280 == BME280_NULL) {
		return E_BME280_NULL_PTR;
		} else {
			com_rslt = p_bme280->BME280_BUS_READ_FUNC(
			p_bme280->dev_addr,
			BME280_HUMIDITY_MSB_REG, a_data_u8,
			BME280_HUMIDITY_DATA_LENGTH);
			*v_uncomp_humidity_s32 = (s32)(
			(((u32)(a_data_u8[BME280_HUMIDITY_MSB_DATA]))
			<< BME280_SHIFT_BIT_POSITION_BY_08_BITS)|
			((u32)(a_data_u8[BME280_HUMIDITY_LSB_DATA])));
		}
	return com_rslt;
}
/*!
 * @brief Reads actual humidity from uncompensated humidity
 * @note Returns the value in %rH as unsigned 32bit integer
 * in Q22.10 format(22 integer 10 fractional bits).
 * @note An output value of 42313
 * represents 42313 / 1024 = 41.321 %rH
 *
 *
 *
 *  @param  v_uncomp_humidity_s32: value of uncompensated humidity
 *
 *  @return Return the actual relative humidity output as u32
 *
*/
u32 BME280::bme280_compensate_humidity_int32(s32 v_uncomp_humidity_s32)
{
	s32 v_x1_u32 = BME280_INIT_VALUE;

	/* calculate x1*/
	v_x1_u32 = (p_bme280->cal_param.t_fine - ((s32)76800));
	/* calculate x1*/
	v_x1_u32 = (((((v_uncomp_humidity_s32
	<< BME280_SHIFT_BIT_POSITION_BY_14_BITS) -
	(((s32)p_bme280->cal_param.dig_H4)
	<< BME280_SHIFT_BIT_POSITION_BY_20_BITS) -
	(((s32)p_bme280->cal_param.dig_H5) * v_x1_u32)) +
	((s32)16384)) >> BME280_SHIFT_BIT_POSITION_BY_15_BITS)
	* (((((((v_x1_u32 *
	((s32)p_bme280->cal_param.dig_H6))
	>> BME280_SHIFT_BIT_POSITION_BY_10_BITS) *
	(((v_x1_u32 * ((s32)p_bme280->cal_param.dig_H3))
	>> BME280_SHIFT_BIT_POSITION_BY_11_BITS) + ((s32)32768)))
	>> BME280_SHIFT_BIT_POSITION_BY_10_BITS) + ((s32)2097152)) *
	((s32)p_bme280->cal_param.dig_H2) + 8192) >> 14));
	v_x1_u32 = (v_x1_u32 - (((((v_x1_u32
	>> BME280_SHIFT_BIT_POSITION_BY_15_BITS) *
	(v_x1_u32 >> BME280_SHIFT_BIT_POSITION_BY_15_BITS))
	>> BME280_SHIFT_BIT_POSITION_BY_07_BITS) *
	((s32)p_bme280->cal_param.dig_H1))
	>> BME280_SHIFT_BIT_POSITION_BY_04_BITS));
	v_x1_u32 = (v_x1_u32 < 0 ? 0 : v_x1_u32);
	v_x1_u32 = (v_x1_u32 > 419430400 ?
	419430400 : v_x1_u32);
	return (u32)(v_x1_u32 >> BME280_SHIFT_BIT_POSITION_BY_12_BITS);
}
/*!
 * @brief Reads actual humidity from uncompensated humidity
 * @note Returns the value in %rH as unsigned 16bit integer
 * @note An output value of 42313
 * represents 42313/512 = 82.643 %rH
 *
 *
 *
 *  @param v_uncomp_humidity_s32: value of uncompensated humidity
 *
 *
 *  @return Return the actual relative humidity output as u16
 *
*/
u16 BME280::bme280_compensate_humidity_int32_sixteen_bit_output(
s32 v_uncomp_humidity_s32)
{
	u32 v_x1_u32 = BME280_INIT_VALUE;
	u16 v_x2_u32 = BME280_INIT_VALUE;

	v_x1_u32 =  bme280_compensate_humidity_int32(v_uncomp_humidity_s32);
	v_x2_u32 = (u16)(v_x1_u32 >> BME280_SHIFT_BIT_POSITION_BY_01_BIT);
	return v_x2_u32;
}
/*!
 * @brief This API used to read uncompensated
 * pressure,temperature and humidity
 *
 *
 *
 *
 *  @param  v_uncomp_pressure_s32: The value of uncompensated pressure.
 *  @param  v_uncomp_temperature_s32: The value of uncompensated temperature
 *  @param  v_uncomp_humidity_s32: The value of uncompensated humidity.
 *
 *
 *
 *	@return results of bus communication function
 *	@retval 0 -> Success
 *	@retval -1 -> Error
 *
 *
*/
BME280_RETURN_FUNCTION_TYPE BME280::bme280_read_uncomp_pressure_temperature_humidity(
s32 *v_uncomp_pressure_s32,
s32 *v_uncomp_temperature_s32, s32 *v_uncomp_humidity_s32)
{
	/* used to return the communication result*/
	BME280_RETURN_FUNCTION_TYPE com_rslt = ERROR;
	/* Array holding the MSB and LSb value of
	a_data_u8[0] - Pressure MSB
	a_data_u8[1] - Pressure LSB
	a_data_u8[1] - Pressure LSB
	a_data_u8[1] - Temperature MSB
	a_data_u8[1] - Temperature LSB
	a_data_u8[1] - Temperature LSB
	a_data_u8[1] - Humidity MSB
	a_data_u8[1] - Humidity LSB
	*/
	u8 a_data_u8[BME280_DATA_FRAME_SIZE] = {
	BME280_INIT_VALUE, BME280_INIT_VALUE,
	BME280_INIT_VALUE, BME280_INIT_VALUE,
	BME280_INIT_VALUE, BME280_INIT_VALUE,
	BME280_INIT_VALUE, BME280_INIT_VALUE};
	/* check the p_bme280 structure pointer as NULL*/
	if (p_bme280 == BME280_NULL) {
		return E_BME280_NULL_PTR;
		} else {
			com_rslt = p_bme280->BME280_BUS_READ_FUNC(
			p_bme280->dev_addr,
			BME280_PRESSURE_MSB_REG,
			a_data_u8, BME280_ALL_DATA_FRAME_LENGTH);
			/*Pressure*/
			*v_uncomp_pressure_s32 = (s32)((
			((u32)(a_data_u8[
			BME280_DATA_FRAME_PRESSURE_MSB_BYTE]))
			<< BME280_SHIFT_BIT_POSITION_BY_12_BITS) |
			(((u32)(a_data_u8[
			BME280_DATA_FRAME_PRESSURE_LSB_BYTE]))
			<< BME280_SHIFT_BIT_POSITION_BY_04_BITS) |
			((u32)a_data_u8[
			BME280_DATA_FRAME_PRESSURE_XLSB_BYTE] >>
			BME280_SHIFT_BIT_POSITION_BY_04_BITS));

			/* Temperature */
			*v_uncomp_temperature_s32 = (s32)(((
			(u32) (a_data_u8[
			BME280_DATA_FRAME_TEMPERATURE_MSB_BYTE]))
			<< BME280_SHIFT_BIT_POSITION_BY_12_BITS) |
			(((u32)(a_data_u8[
			BME280_DATA_FRAME_TEMPERATURE_LSB_BYTE]))
			<< BME280_SHIFT_BIT_POSITION_BY_04_BITS)
			| ((u32)a_data_u8[
			BME280_DATA_FRAME_TEMPERATURE_XLSB_BYTE]
			>> BME280_SHIFT_BIT_POSITION_BY_04_BITS));

			/*Humidity*/
			*v_uncomp_humidity_s32 = (s32)((
			((u32)(a_data_u8[
			BME280_DATA_FRAME_HUMIDITY_MSB_BYTE]))
			<< BME280_SHIFT_BIT_POSITION_BY_08_BITS)|
			((u32)(a_data_u8[
			BME280_DATA_FRAME_HUMIDITY_LSB_BYTE])));
		}
	return com_rslt;
}
/*!
 * @brief This API used to read true pressure, temperature and humidity
 *
 *
 *
 *
 *	@param  v_pressure_u32 : The value of compensated pressure.
 *	@param  v_temperature_s32 : The value of compensated temperature.
 *	@param  v_humidity_u32 : The value of compensated humidity.
 *
 *
 *	@return results of bus communication function
 *	@retval 0 -> Success
 *	@retval -1 -> Error
 *
 *
*/
BME280_RETURN_FUNCTION_TYPE BME280::bme280_read_pressure_temperature_humidity(
u32 *v_pressure_u32, s32 *v_temperature_s32, u32 *v_humidity_u32)
{
	/* used to return the communication result*/
	BME280_RETURN_FUNCTION_TYPE com_rslt = ERROR;
	s32 v_uncomp_pressure_s32 = BME280_INIT_VALUE;
	s32 v_uncom_temperature_s32 = BME280_INIT_VALUE;
	s32 v_uncom_humidity_s32 = BME280_INIT_VALUE;
	/* check the p_bme280 structure pointer as NULL*/
	if (p_bme280 == BME280_NULL) {
		return E_BME280_NULL_PTR;
		} else {
			/* read the uncompensated pressure,
			temperature and humidity*/
			com_rslt =
			bme280_read_uncomp_pressure_temperature_humidity(
			&v_uncomp_pressure_s32, &v_uncom_temperature_s32,
			&v_uncom_humidity_s32);
			/* read the true pressure, temperature and humidity*/
			*v_temperature_s32 =
			bme280_compensate_temperature_int32(
			v_uncom_temperature_s32);
			*v_pressure_u32 = bme280_compensate_pressure_int32(
			v_uncomp_pressure_s32);
			*v_humidity_u32 = bme280_compensate_humidity_int32(
			v_uncom_humidity_s32);
		}
	return com_rslt;
}
/*!
 *	@brief This API is used to
 *	calibration parameters used for calculation in the registers
 *
 *  parameter | Register address |   bit
 *------------|------------------|----------------
 *	dig_T1    |  0x88 and 0x89   | from 0 : 7 to 8: 15
 *	dig_T2    |  0x8A and 0x8B   | from 0 : 7 to 8: 15
 *	dig_T3    |  0x8C and 0x8D   | from 0 : 7 to 8: 15
 *	dig_P1    |  0x8E and 0x8F   | from 0 : 7 to 8: 15
 *	dig_P2    |  0x90 and 0x91   | from 0 : 7 to 8: 15
 *	dig_P3    |  0x92 and 0x93   | from 0 : 7 to 8: 15
 *	dig_P4    |  0x94 and 0x95   | from 0 : 7 to 8: 15
 *	dig_P5    |  0x96 and 0x97   | from 0 : 7 to 8: 15
 *	dig_P6    |  0x98 and 0x99   | from 0 : 7 to 8: 15
 *	dig_P7    |  0x9A and 0x9B   | from 0 : 7 to 8: 15
 *	dig_P8    |  0x9C and 0x9D   | from 0 : 7 to 8: 15
 *	dig_P9    |  0x9E and 0x9F   | from 0 : 7 to 8: 15
 *	dig_H1    |         0xA1     | from 0 to 7
 *	dig_H2    |  0xE1 and 0xE2   | from 0 : 7 to 8: 15
 *	dig_H3    |         0xE3     | from 0 to 7
 *
 *	@return results of bus communication function
 *	@retval 0 -> Success
 *	@retval -1 -> Error
 *
 *
*/
BME280_RETURN_FUNCTION_TYPE BME280::bme280_get_calib_param(void)
{
	/* used to return the communication result*/
	BME280_RETURN_FUNCTION_TYPE com_rslt = ERROR;
	u8 a_data_u8[BME280_CALIB_DATA_SIZE] = {
	BME280_INIT_VALUE, BME280_INIT_VALUE,
	BME280_INIT_VALUE, BME280_INIT_VALUE, BME280_INIT_VALUE,
	BME280_INIT_VALUE, BME280_INIT_VALUE, BME280_INIT_VALUE,
	BME280_INIT_VALUE, BME280_INIT_VALUE, BME280_INIT_VALUE,
	BME280_INIT_VALUE, BME280_INIT_VALUE, BME280_INIT_VALUE,
	BME280_INIT_VALUE, BME280_INIT_VALUE, BME280_INIT_VALUE,
	BME280_INIT_VALUE, BME280_INIT_VALUE, BME280_INIT_VALUE,
	BME280_INIT_VALUE, BME280_INIT_VALUE, BME280_INIT_VALUE,
	BME280_INIT_VALUE, BME280_INIT_VALUE, BME280_INIT_VALUE};
	/* check the p_bme280 structure pointer as NULL*/
	if (p_bme280 == BME280_NULL) {
		return E_BME280_NULL_PTR;
		} else {
			com_rslt = p_bme280->BME280_BUS_READ_FUNC(
			p_bme280->dev_addr,
			BME280_TEMPERATURE_CALIB_DIG_T1_LSB_REG,
			a_data_u8,
			BME280_PRESSURE_TEMPERATURE_CALIB_DATA_LENGTH);

			p_bme280->cal_param.dig_T1 = (u16)(((
			(u16)((u8)a_data_u8[
			BME280_TEMPERATURE_CALIB_DIG_T1_MSB])) <<
			BME280_SHIFT_BIT_POSITION_BY_08_BITS)
			| a_data_u8[BME280_TEMPERATURE_CALIB_DIG_T1_LSB]);
			p_bme280->cal_param.dig_T2 = (s16)(((
			(s16)((s8)a_data_u8[
			BME280_TEMPERATURE_CALIB_DIG_T2_MSB])) <<
			BME280_SHIFT_BIT_POSITION_BY_08_BITS)
			| a_data_u8[BME280_TEMPERATURE_CALIB_DIG_T2_LSB]);
			p_bme280->cal_param.dig_T3 = (s16)(((
			(s16)((s8)a_data_u8[
			BME280_TEMPERATURE_CALIB_DIG_T3_MSB])) <<
			BME280_SHIFT_BIT_POSITION_BY_08_BITS)
			| a_data_u8[BME280_TEMPERATURE_CALIB_DIG_T3_LSB]);
			p_bme280->cal_param.dig_P1 = (u16)(((
			(u16)((u8)a_data_u8[
			BME280_PRESSURE_CALIB_DIG_P1_MSB])) <<
			BME280_SHIFT_BIT_POSITION_BY_08_BITS)
			| a_data_u8[BME280_PRESSURE_CALIB_DIG_P1_LSB]);
			p_bme280->cal_param.dig_P2 = (s16)(((
			(s16)((s8)a_data_u8[
			BME280_PRESSURE_CALIB_DIG_P2_MSB])) <<
			BME280_SHIFT_BIT_POSITION_BY_08_BITS)
			| a_data_u8[BME280_PRESSURE_CALIB_DIG_P2_LSB]);
			p_bme280->cal_param.dig_P3 = (s16)(((
			(s16)((s8)a_data_u8[
			BME280_PRESSURE_CALIB_DIG_P3_MSB])) <<
			BME280_SHIFT_BIT_POSITION_BY_08_BITS)
			| a_data_u8[
			BME280_PRESSURE_CALIB_DIG_P3_LSB]);
			p_bme280->cal_param.dig_P4 = (s16)(((
			(s16)((s8)a_data_u8[
			BME280_PRESSURE_CALIB_DIG_P4_MSB])) <<
			BME280_SHIFT_BIT_POSITION_BY_08_BITS)
			| a_data_u8[BME280_PRESSURE_CALIB_DIG_P4_LSB]);
			p_bme280->cal_param.dig_P5 = (s16)(((
			(s16)((s8)a_data_u8[
			BME280_PRESSURE_CALIB_DIG_P5_MSB])) <<
			BME280_SHIFT_BIT_POSITION_BY_08_BITS)
			| a_data_u8[BME280_PRESSURE_CALIB_DIG_P5_LSB]);
			p_bme280->cal_param.dig_P6 = (s16)(((
			(s16)((s8)a_data_u8[
			BME280_PRESSURE_CALIB_DIG_P6_MSB])) <<
			BME280_SHIFT_BIT_POSITION_BY_08_BITS)
			| a_data_u8[BME280_PRESSURE_CALIB_DIG_P6_LSB]);
			p_bme280->cal_param.dig_P7 = (s16)(((
			(s16)((s8)a_data_u8[
			BME280_PRESSURE_CALIB_DIG_P7_MSB])) <<
			BME280_SHIFT_BIT_POSITION_BY_08_BITS)
			| a_data_u8[BME280_PRESSURE_CALIB_DIG_P7_LSB]);
			p_bme280->cal_param.dig_P8 = (s16)(((
			(s16)((s8)a_data_u8[
			BME280_PRESSURE_CALIB_DIG_P8_MSB])) <<
			BME280_SHIFT_BIT_POSITION_BY_08_BITS)
			| a_data_u8[BME280_PRESSURE_CALIB_DIG_P8_LSB]);
			p_bme280->cal_param.dig_P9 = (s16)(((
			(s16)((s8)a_data_u8[
			BME280_PRESSURE_CALIB_DIG_P9_MSB])) <<
			BME280_SHIFT_BIT_POSITION_BY_08_BITS)
			| a_data_u8[BME280_PRESSURE_CALIB_DIG_P9_LSB]);
			p_bme280->cal_param.dig_H1 =
			a_data_u8[BME280_HUMIDITY_CALIB_DIG_H1];
			com_rslt += p_bme280->BME280_BUS_READ_FUNC(
			p_bme280->dev_addr,
			BME280_HUMIDITY_CALIB_DIG_H2_LSB_REG, a_data_u8,
			BME280_HUMIDITY_CALIB_DATA_LENGTH);
			p_bme280->cal_param.dig_H2 = (s16)(((
			(s16)((s8)a_data_u8[
			BME280_HUMIDITY_CALIB_DIG_H2_MSB])) <<
			BME280_SHIFT_BIT_POSITION_BY_08_BITS)
			| a_data_u8[BME280_HUMIDITY_CALIB_DIG_H2_LSB]);
			p_bme280->cal_param.dig_H3 =
			a_data_u8[BME280_HUMIDITY_CALIB_DIG_H3];
			p_bme280->cal_param.dig_H4 = (s16)(((
			(s16)((s8)a_data_u8[
			BME280_HUMIDITY_CALIB_DIG_H4_MSB])) <<
			BME280_SHIFT_BIT_POSITION_BY_04_BITS) |
			(((u8)BME280_MASK_DIG_H4) &
			a_data_u8[BME280_HUMIDITY_CALIB_DIG_H4_LSB]));
			p_bme280->cal_param.dig_H5 = (s16)(((
			(s16)((s8)a_data_u8[
			BME280_HUMIDITY_CALIB_DIG_H5_MSB])) <<
			BME280_SHIFT_BIT_POSITION_BY_04_BITS) |
			(a_data_u8[BME280_HUMIDITY_CALIB_DIG_H4_LSB] >>
			BME280_SHIFT_BIT_POSITION_BY_04_BITS));
			p_bme280->cal_param.dig_H6 =
			(s8)a_data_u8[BME280_HUMIDITY_CALIB_DIG_H6];
		}
	return com_rslt;
}
/*!
 *	@brief This API is used to get
 *	the temperature oversampling setting in the register 0xF4
 *	bits from 5 to 7
 *
 *	value               |   Temperature oversampling
 * ---------------------|---------------------------------
 *	0x00                | Skipped
 *	0x01                | BME280_OVERSAMP_1X
 *	0x02                | BME280_OVERSAMP_2X
 *	0x03                | BME280_OVERSAMP_4X
 *	0x04                | BME280_OVERSAMP_8X
 *	0x05,0x06 and 0x07  | BME280_OVERSAMP_16X
 *
 *
 *  @param v_value_u8 : The value of temperature over sampling
 *
 *
 *
 *	@return results of bus communication function
 *	@retval 0 -> Success
 *	@retval -1 -> Error
 *
 *
*/
BME280_RETURN_FUNCTION_TYPE BME280::bme280_get_oversamp_temperature(
u8 *v_value_u8)
{
	/* used to return the communication result*/
	BME280_RETURN_FUNCTION_TYPE com_rslt = ERROR;
	u8 v_data_u8 = BME280_INIT_VALUE;
	/* check the p_bme280 structure pointer as NULL*/
	if (p_bme280 == BME280_NULL) {
		return E_BME280_NULL_PTR;
		} else {
			com_rslt = p_bme280->BME280_BUS_READ_FUNC(
			p_bme280->dev_addr,
			BME280_CTRL_MEAS_REG_OVERSAMP_TEMPERATURE__REG,
			&v_data_u8, BME280_GEN_READ_WRITE_DATA_LENGTH);
			*v_value_u8 = BME280_GET_BITSLICE(v_data_u8,
			BME280_CTRL_MEAS_REG_OVERSAMP_TEMPERATURE);

			p_bme280->oversamp_temperature = *v_value_u8;
		}
	return com_rslt;
}
/*!
 *	@brief This API is used to set
 *	the temperature oversampling setting in the register 0xF4
 *	bits from 5 to 7
 *
 *	value               |   Temperature oversampling
 * ---------------------|---------------------------------
 *	0x00                | Skipped
 *	0x01                | BME280_OVERSAMP_1X
 *	0x02                | BME280_OVERSAMP_2X
 *	0x03                | BME280_OVERSAMP_4X
 *	0x04                | BME280_OVERSAMP_8X
 *	0x05,0x06 and 0x07  | BME280_OVERSAMP_16X
 *
 *
 *  @param v_value_u8 : The value of temperature over sampling
 *
 *
 *
 *	@return results of bus communication function
 *	@retval 0 -> Success
 *	@retval -1 -> Error
 *
 *
*/
BME280_RETURN_FUNCTION_TYPE BME280::bme280_set_oversamp_temperature(
u8 v_value_u8)
{
	/* used to return the communication result*/
	BME280_RETURN_FUNCTION_TYPE com_rslt = ERROR;
	u8 v_data_u8 = BME280_INIT_VALUE;
	u8 v_prev_pow_mode_u8 = BME280_INIT_VALUE;
	u8 v_pre_ctrl_hum_value_u8 = BME280_INIT_VALUE;
	u8 v_pre_config_value_u8 = BME280_INIT_VALUE;
	/* check the p_bme280 structure pointer as NULL*/
	if (p_bme280 == BME280_NULL) {
		return E_BME280_NULL_PTR;
		} else {
			v_data_u8 = p_bme280->ctrl_meas_reg;
			v_data_u8 =
			BME280_SET_BITSLICE(v_data_u8,
			BME280_CTRL_MEAS_REG_OVERSAMP_TEMPERATURE, v_value_u8);
			com_rslt = bme280_get_power_mode(&v_prev_pow_mode_u8);
			if (v_prev_pow_mode_u8 != BME280_SLEEP_MODE) {
				com_rslt += bme280_set_soft_rst();
				p_bme280->delay_msec(BME280_3MS_DELAY);
				/* write previous value
				of configuration register*/
				v_pre_config_value_u8 = p_bme280->config_reg;
				com_rslt += bme280_write_register(
					BME280_CONFIG_REG,
				&v_pre_config_value_u8,
				BME280_GEN_READ_WRITE_DATA_LENGTH);
				/* write previous value
				of humidity oversampling*/
				v_pre_ctrl_hum_value_u8 =
				p_bme280->ctrl_hum_reg;
				com_rslt += bme280_write_register(
					BME280_CTRL_HUMIDITY_REG,
				&v_pre_ctrl_hum_value_u8,
				BME280_GEN_READ_WRITE_DATA_LENGTH);
				/* write previous and updated value
				of configuration register*/
				com_rslt += bme280_write_register(
					BME280_CTRL_MEAS_REG,
				&v_data_u8, BME280_GEN_READ_WRITE_DATA_LENGTH);
			} else {
				com_rslt = p_bme280->BME280_BUS_WRITE_FUNC(
				p_bme280->dev_addr,
				BME280_CTRL_MEAS_REG_OVERSAMP_TEMPERATURE__REG,
				&v_data_u8, BME280_GEN_READ_WRITE_DATA_LENGTH);
			}
				p_bme280->oversamp_temperature = v_value_u8;
				/* read the control measurement register value*/
				com_rslt = bme280_read_register(
					BME280_CTRL_MEAS_REG,
				&v_data_u8, BME280_GEN_READ_WRITE_DATA_LENGTH);
				p_bme280->ctrl_meas_reg = v_data_u8;
				/* read the control humidity register value*/
				com_rslt += bme280_read_register(
					BME280_CTRL_HUMIDITY_REG,
				&v_data_u8, BME280_GEN_READ_WRITE_DATA_LENGTH);
				p_bme280->ctrl_hum_reg = v_data_u8;
				/* read the control
				configuration register value*/
				com_rslt += bme280_read_register(
					BME280_CONFIG_REG,
				&v_data_u8, BME280_GEN_READ_WRITE_DATA_LENGTH);
				p_bme280->config_reg = v_data_u8;
		}
	return com_rslt;
}
/*!
 *	@brief This API is used to get
 *	the pressure oversampling setting in the register 0xF4
 *	bits from 2 to 4
 *
 *	value              | Pressure oversampling
 * --------------------|--------------------------
 *	0x00               | Skipped
 *	0x01               | BME280_OVERSAMP_1X
 *	0x02               | BME280_OVERSAMP_2X
 *	0x03               | BME280_OVERSAMP_4X
 *	0x04               | BME280_OVERSAMP_8X
 *	0x05,0x06 and 0x07 | BME280_OVERSAMP_16X
 *
 *
 *  @param v_value_u8 : The value of pressure oversampling
 *
 *
 *
 *	@return results of bus communication function
 *	@retval 0 -> Success
 *	@retval -1 -> Error
 *
 *
*/
BME280_RETURN_FUNCTION_TYPE BME280::bme280_get_oversamp_pressure(
u8 *v_value_u8)
{
	/* used to return the communication result*/
	BME280_RETURN_FUNCTION_TYPE com_rslt = ERROR;
	u8 v_data_u8 = BME280_INIT_VALUE;
	/* check the p_bme280 structure pointer as NULL*/
	if (p_bme280 == BME280_NULL) {
		return E_BME280_NULL_PTR;
		} else {
			com_rslt = p_bme280->BME280_BUS_READ_FUNC(
			p_bme280->dev_addr,
			BME280_CTRL_MEAS_REG_OVERSAMP_PRESSURE__REG,
			&v_data_u8, BME280_GEN_READ_WRITE_DATA_LENGTH);
			*v_value_u8 = BME280_GET_BITSLICE(
			v_data_u8,
			BME280_CTRL_MEAS_REG_OVERSAMP_PRESSURE);

			p_bme280->oversamp_pressure = *v_value_u8;
		}
	return com_rslt;
}
/*!
 *	@brief This API is used to set
 *	the pressure oversampling setting in the register 0xF4
 *	bits from 2 to 4
 *
 *	value              | Pressure oversampling
 * --------------------|--------------------------
 *	0x00               | Skipped
 *	0x01               | BME280_OVERSAMP_1X
 *	0x02               | BME280_OVERSAMP_2X
 *	0x03               | BME280_OVERSAMP_4X
 *	0x04               | BME280_OVERSAMP_8X
 *	0x05,0x06 and 0x07 | BME280_OVERSAMP_16X
 *
 *
 *  @param v_value_u8 : The value of pressure oversampling
 *
 *
 *
 *	@return results of bus communication function
 *	@retval 0 -> Success
 *	@retval -1 -> Error
 *
 *
*/
BME280_RETURN_FUNCTION_TYPE BME280::bme280_set_oversamp_pressure(
u8 v_value_u8)
{
	/* used to return the communication result*/
	BME280_RETURN_FUNCTION_TYPE com_rslt = ERROR;
	u8 v_data_u8 = BME280_INIT_VALUE;
	u8 v_prev_pow_mode_u8 = BME280_INIT_VALUE;
	u8 v_pre_ctrl_hum_value_u8 = BME280_INIT_VALUE;
	u8 v_pre_config_value_u8 = BME280_INIT_VALUE;
	/* check the p_bme280 structure pointer as NULL*/
	if (p_bme280 == BME280_NULL) {
		return E_BME280_NULL_PTR;
		} else {
			v_data_u8 = p_bme280->ctrl_meas_reg;
			v_data_u8 =
			BME280_SET_BITSLICE(v_data_u8,
			BME280_CTRL_MEAS_REG_OVERSAMP_PRESSURE, v_value_u8);
			com_rslt = bme280_get_power_mode(&v_prev_pow_mode_u8);
			if (v_prev_pow_mode_u8 != BME280_SLEEP_MODE) {
				com_rslt += bme280_set_soft_rst();
				p_bme280->delay_msec(BME280_3MS_DELAY);
				/* write previous value of
				configuration register*/
				v_pre_config_value_u8 = p_bme280->config_reg;
				com_rslt = bme280_write_register(
					BME280_CONFIG_REG,
				&v_pre_config_value_u8,
				BME280_GEN_READ_WRITE_DATA_LENGTH);
				/* write previous value of
				humidity oversampling*/
				v_pre_ctrl_hum_value_u8 =
				p_bme280->ctrl_hum_reg;
				com_rslt += bme280_write_register(
					BME280_CTRL_HUMIDITY_REG,
				&v_pre_ctrl_hum_value_u8,
				BME280_GEN_READ_WRITE_DATA_LENGTH);
				/* write previous and updated value of
				control measurement register*/
				bme280_write_register(
					BME280_CTRL_MEAS_REG,
				&v_data_u8, BME280_GEN_READ_WRITE_DATA_LENGTH);
			} else {
				com_rslt = p_bme280->BME280_BUS_WRITE_FUNC(
				p_bme280->dev_addr,
				BME280_CTRL_MEAS_REG_OVERSAMP_PRESSURE__REG,
				&v_data_u8, BME280_GEN_READ_WRITE_DATA_LENGTH);
			}
				p_bme280->oversamp_pressure = v_value_u8;
				/* read the control measurement register value*/
				com_rslt = bme280_read_register(
					BME280_CTRL_MEAS_REG,
				&v_data_u8, BME280_GEN_READ_WRITE_DATA_LENGTH);
				p_bme280->ctrl_meas_reg = v_data_u8;
				/* read the control humidity register value*/
				com_rslt += bme280_read_register(
					BME280_CTRL_HUMIDITY_REG,
				&v_data_u8, BME280_GEN_READ_WRITE_DATA_LENGTH);
				p_bme280->ctrl_hum_reg = v_data_u8;
				/* read the control
				configuration register value*/
				com_rslt += bme280_read_register(
					BME280_CONFIG_REG,
				&v_data_u8, BME280_GEN_READ_WRITE_DATA_LENGTH);
				p_bme280->config_reg = v_data_u8;
		}
	return com_rslt;
}
/*!
 *	@brief This API is used to get
 *	the humidity oversampling setting in the register 0xF2
 *	bits from 0 to 2
 *
 *	value               | Humidity oversampling
 * ---------------------|-------------------------
 *	0x00                | Skipped
 *	0x01                | BME280_OVERSAMP_1X
 *	0x02                | BME280_OVERSAMP_2X
 *	0x03                | BME280_OVERSAMP_4X
 *	0x04                | BME280_OVERSAMP_8X
 *	0x05,0x06 and 0x07  | BME280_OVERSAMP_16X
 *
 *
 *  @param  v_value_u8 : The value of humidity over sampling
 *
 *
 *
 *	@return results of bus communication function
 *	@retval 0 -> Success
 *	@retval -1 -> Error
 *
 *
*/
BME280_RETURN_FUNCTION_TYPE BME280::bme280_get_oversamp_humidity(
u8 *v_value_u8)
{
	/* used to return the communication result*/
	BME280_RETURN_FUNCTION_TYPE com_rslt = ERROR;
	u8 v_data_u8 = BME280_INIT_VALUE;
	/* check the p_bme280 structure pointer as NULL*/
	if (p_bme280 == BME280_NULL) {
		return E_BME280_NULL_PTR;
		} else {
			com_rslt = p_bme280->BME280_BUS_READ_FUNC(
			p_bme280->dev_addr,
			BME280_CTRL_HUMIDITY_REG_OVERSAMP_HUMIDITY__REG,
			&v_data_u8, BME280_GEN_READ_WRITE_DATA_LENGTH);
			*v_value_u8 = BME280_GET_BITSLICE(
			v_data_u8,
			BME280_CTRL_HUMIDITY_REG_OVERSAMP_HUMIDITY);

			p_bme280->oversamp_humidity = *v_value_u8;
		}
	return com_rslt;
}
/*!
 *	@brief This API is used to set
 *	the humidity oversampling setting in the register 0xF2
 *	bits from 0 to 2
 *
 *	value               | Humidity oversampling
 * ---------------------|-------------------------
 *	0x00                | Skipped
 *	0x01                | BME280_OVERSAMP_1X
 *	0x02                | BME280_OVERSAMP_2X
 *	0x03                | BME280_OVERSAMP_4X
 *	0x04                | BME280_OVERSAMP_8X
 *	0x05,0x06 and 0x07  | BME280_OVERSAMP_16X
 *
 *
 *  @param  v_value_u8 : The value of humidity over sampling
 *
 *
 *
 * @note The "BME280_CTRL_HUMIDITY_REG_OVERSAMP_HUMIDITY"
 * register sets the humidity
 * data acquisition options of the device.
 * @note changes to this registers only become
 * effective after a write operation to
 * "BME280_CTRL_MEAS_REG" register.
 * @note In the code automated reading and writing of
 *	"BME280_CTRL_HUMIDITY_REG_OVERSAMP_HUMIDITY"
 * @note register first set the
 * "BME280_CTRL_HUMIDITY_REG_OVERSAMP_HUMIDITY"
 *  and then read and write
 *  the "BME280_CTRL_MEAS_REG" register in the function.
 *
 *
 *	@return results of bus communication function
 *	@retval 0 -> Success
 *	@retval -1 -> Error
 *
 *
*/
BME280_RETURN_FUNCTION_TYPE BME280::bme280_set_oversamp_humidity(
u8 v_value_u8)
{
	/* used to return the communication result*/
	BME280_RETURN_FUNCTION_TYPE com_rslt = ERROR;
	u8 v_data_u8 = BME280_INIT_VALUE;
	u8 pre_ctrl_meas_value = BME280_INIT_VALUE;
	u8 v_pre_config_value_u8 = BME280_INIT_VALUE;
	u8 v_prev_pow_mode_u8 = BME280_INIT_VALUE;
	/* check the p_bme280 structure pointer as NULL*/
	if (p_bme280 == BME280_NULL) {
		return E_BME280_NULL_PTR;
		} else {
			/* write humidity oversampling*/
			v_data_u8 = p_bme280->ctrl_hum_reg;
			v_data_u8 =
			BME280_SET_BITSLICE(v_data_u8,
			BME280_CTRL_HUMIDITY_REG_OVERSAMP_HUMIDITY, v_value_u8);
			com_rslt = bme280_get_power_mode(&v_prev_pow_mode_u8);
			if (v_prev_pow_mode_u8 != BME280_SLEEP_MODE) {
				com_rslt += bme280_set_soft_rst();
				p_bme280->delay_msec(BME280_3MS_DELAY);
				/* write previous value of
				configuration register*/
				v_pre_config_value_u8 = p_bme280->config_reg;
				com_rslt += bme280_write_register(
					BME280_CONFIG_REG,
				&v_pre_config_value_u8,
				BME280_GEN_READ_WRITE_DATA_LENGTH);
				/* write the value of control humidity*/
				com_rslt += bme280_write_register(
					BME280_CTRL_HUMIDITY_REG,
				&v_data_u8, BME280_GEN_READ_WRITE_DATA_LENGTH);
				/* write previous value of
				control measurement register*/
				pre_ctrl_meas_value =
				p_bme280->ctrl_meas_reg;
				com_rslt += bme280_write_register(
					BME280_CTRL_MEAS_REG,
				&pre_ctrl_meas_value,
				BME280_GEN_READ_WRITE_DATA_LENGTH);
			} else {
				com_rslt +=
				p_bme280->BME280_BUS_WRITE_FUNC(
				p_bme280->dev_addr,
				BME280_CTRL_HUMIDITY_REG_OVERSAMP_HUMIDITY__REG,
				&v_data_u8, BME280_GEN_READ_WRITE_DATA_LENGTH);
				/* Control humidity write will effective only
				after the control measurement register*/
				pre_ctrl_meas_value =
				p_bme280->ctrl_meas_reg;
				com_rslt += bme280_write_register(
					BME280_CTRL_MEAS_REG,
				&pre_ctrl_meas_value,
				BME280_GEN_READ_WRITE_DATA_LENGTH);
			}
			p_bme280->oversamp_humidity = v_value_u8;
			/* read the control measurement register value*/
			com_rslt += bme280_read_register(BME280_CTRL_MEAS_REG,
			&v_data_u8, BME280_GEN_READ_WRITE_DATA_LENGTH);
			p_bme280->ctrl_meas_reg = v_data_u8;
			/* read the control humidity register value*/
			com_rslt += bme280_read_register(
			BME280_CTRL_HUMIDITY_REG,
			&v_data_u8, BME280_GEN_READ_WRITE_DATA_LENGTH);
			p_bme280->ctrl_hum_reg = v_data_u8;
			/* read the control configuration register value*/
			com_rslt += bme280_read_register(BME280_CONFIG_REG,
			&v_data_u8, BME280_GEN_READ_WRITE_DATA_LENGTH);
			p_bme280->config_reg = v_data_u8;
		}
	return com_rslt;
}
/*!
 *	@brief This API used to get the
 *	Operational Mode from the sensor in the register 0xF4 bit 0 and 1
 *
 *
 *
 *	@param v_power_mode_u8 : The value of power mode
 *  value           |    mode
 * -----------------|------------------
 *	0x00            | BME280_SLEEP_MODE
 *	0x01 and 0x02   | BME280_FORCED_MODE
 *	0x03            | BME280_NORMAL_MODE
 *
 *	@return results of bus communication function
 *	@retval 0 -> Success
 *	@retval -1 -> Error
 *
 *
*/
BME280_RETURN_FUNCTION_TYPE BME280::bme280_get_power_mode(u8 *v_power_mode_u8)
{
	/* used to return the communication result*/
	BME280_RETURN_FUNCTION_TYPE com_rslt = ERROR;
	u8 v_mode_u8r = BME280_INIT_VALUE;
	/* check the p_bme280 structure pointer as NULL*/
	if (p_bme280 == BME280_NULL) {
		return E_BME280_NULL_PTR;
		} else {
			com_rslt = p_bme280->BME280_BUS_READ_FUNC(
			p_bme280->dev_addr,
			BME280_CTRL_MEAS_REG_POWER_MODE__REG,
			&v_mode_u8r, BME280_GEN_READ_WRITE_DATA_LENGTH);
			*v_power_mode_u8 = BME280_GET_BITSLICE(v_mode_u8r,
			BME280_CTRL_MEAS_REG_POWER_MODE);
		}
	return com_rslt;
}
/*!
 *	@brief This API used to set the
 *	Operational Mode from the sensor in the register 0xF4 bit 0 and 1
 *
 *
 *
 *	@param v_power_mode_u8 : The value of power mode
 *  value           |    mode
 * -----------------|------------------
 *	0x00            | BME280_SLEEP_MODE
 *	0x01 and 0x02   | BME280_FORCED_MODE
 *	0x03            | BME280_NORMAL_MODE
 *
 *	@return results of bus communication function
 *	@retval 0 -> Success
 *	@retval -1 -> Error
 *
 *
*/
BME280_RETURN_FUNCTION_TYPE BME280::bme280_set_power_mode(u8 v_power_mode_u8)
{
	/* used to return the communication result*/
	BME280_RETURN_FUNCTION_TYPE com_rslt = ERROR;
	u8 v_mode_u8r = BME280_INIT_VALUE;
	u8 v_prev_pow_mode_u8 = BME280_INIT_VALUE;
	u8 v_pre_ctrl_hum_value_u8 = BME280_INIT_VALUE;
	u8 v_pre_config_value_u8 = BME280_INIT_VALUE;
	u8 v_data_u8 = BME280_INIT_VALUE;
	/* check the p_bme280 structure pointer as NULL*/
	if (p_bme280 == BME280_NULL) {
		return E_BME280_NULL_PTR;
		} else {
			if (v_power_mode_u8 <= BME280_NORMAL_MODE) {
				v_mode_u8r = p_bme280->ctrl_meas_reg;
				v_mode_u8r =
				BME280_SET_BITSLICE(v_mode_u8r,
				BME280_CTRL_MEAS_REG_POWER_MODE,
				v_power_mode_u8);
				com_rslt = bme280_get_power_mode(
					&v_prev_pow_mode_u8);
				if (v_prev_pow_mode_u8 != BME280_SLEEP_MODE) {
					com_rslt += bme280_set_soft_rst();
					p_bme280->delay_msec(BME280_3MS_DELAY);
					/* write previous value of
					configuration register*/
					v_pre_config_value_u8 =
					p_bme280->config_reg;
					com_rslt = bme280_write_register(
						BME280_CONFIG_REG,
					&v_pre_config_value_u8,
					BME280_GEN_READ_WRITE_DATA_LENGTH);
					/* write previous value of
					humidity oversampling*/
					v_pre_ctrl_hum_value_u8 =
					p_bme280->ctrl_hum_reg;
					com_rslt += bme280_write_register(
					BME280_CTRL_HUMIDITY_REG,
					&v_pre_ctrl_hum_value_u8,
					BME280_GEN_READ_WRITE_DATA_LENGTH);
					/* write previous and updated value of
					control measurement register*/
					com_rslt += bme280_write_register(
					BME280_CTRL_MEAS_REG,
					&v_mode_u8r,
					BME280_GEN_READ_WRITE_DATA_LENGTH);
				} else {
					com_rslt =
					p_bme280->BME280_BUS_WRITE_FUNC(
					p_bme280->dev_addr,
					BME280_CTRL_MEAS_REG_POWER_MODE__REG,
					&v_mode_u8r,
					BME280_GEN_READ_WRITE_DATA_LENGTH);
				}
				/* read the control measurement register value*/
				com_rslt = bme280_read_register(
					BME280_CTRL_MEAS_REG,
				&v_data_u8, BME280_GEN_READ_WRITE_DATA_LENGTH);
				p_bme280->ctrl_meas_reg = v_data_u8;
				/* read the control humidity register value*/
				com_rslt += bme280_read_register(
					BME280_CTRL_HUMIDITY_REG,
				&v_data_u8, BME280_GEN_READ_WRITE_DATA_LENGTH);
				p_bme280->ctrl_hum_reg = v_data_u8;
				/* read the config register value*/
				com_rslt += bme280_read_register(
					BME280_CONFIG_REG,
				&v_data_u8, BME280_GEN_READ_WRITE_DATA_LENGTH);
				p_bme280->config_reg = v_data_u8;
			} else {
			com_rslt = E_BME280_OUT_OF_RANGE;
			}
		}
	return com_rslt;
}
/*!
 * @brief Used to reset the sensor
 * The value 0xB6 is written to the 0xE0
 * register the device is reset using the
 * complete power-on-reset procedure.
 * @note Soft reset can be easily set using bme280_set_softreset().
 * @note Usage Hint : bme280_set_softreset()
 *
 *
 *	@return results of bus communication function
 *	@retval 0 -> Success
 *	@retval -1 -> Error
 *
 *
*/
BME280_RETURN_FUNCTION_TYPE BME280::bme280_set_soft_rst(void)
{
	/* used to return the communication result*/
	BME280_RETURN_FUNCTION_TYPE com_rslt = ERROR;
	u8 v_data_u8 = BME280_SOFT_RESET_CODE;
	/* check the p_bme280 structure pointer as NULL*/
	if (p_bme280 == BME280_NULL) {
		return E_BME280_NULL_PTR;
		} else {
			com_rslt = p_bme280->BME280_BUS_WRITE_FUNC(
			p_bme280->dev_addr,
			BME280_RST_REG, &v_data_u8,
			BME280_GEN_READ_WRITE_DATA_LENGTH);
		}
	return com_rslt;
}
/*!
 *	@brief This API used to get the sensor
 *	SPI mode(communication type) in the register 0xF5 bit 0
 *
 *
 *
 *	@param v_enable_disable_u8 : The value of SPI enable
 *	value  | Description
 * --------|--------------
 *   0     | Disable
 *   1     | Enable
 *
 *
 *
 *	@return results of bus communication function
 *	@retval 0 -> Success
 *	@retval -1 -> Error
 *
 *
*/
BME280_RETURN_FUNCTION_TYPE BME280::bme280_get_spi3(u8 *v_enable_disable_u8)
{
	/* used to return the communication result*/
	BME280_RETURN_FUNCTION_TYPE com_rslt = ERROR;
	u8 v_data_u8 = BME280_INIT_VALUE;
	/* check the p_bme280 structure pointer as NULL*/
	if (p_bme280 == BME280_NULL) {
		return E_BME280_NULL_PTR;
		} else {
			com_rslt = p_bme280->BME280_BUS_READ_FUNC(
			p_bme280->dev_addr,
			BME280_CONFIG_REG_SPI3_ENABLE__REG,
			&v_data_u8, BME280_GEN_READ_WRITE_DATA_LENGTH);
			*v_enable_disable_u8 = BME280_GET_BITSLICE(
			v_data_u8,
			BME280_CONFIG_REG_SPI3_ENABLE);
		}
	return com_rslt;
}
/*!
 *	@brief This API used to set the sensor
 *	SPI mode(communication type) in the register 0xF5 bit 0
 *
 *
 *
 *	@param v_enable_disable_u8 : The value of SPI enable
 *	value  | Description
 * --------|--------------
 *   0     | Disable
 *   1     | Enable
 *
 *
 *
 *	@return results of bus communication function
 *	@retval 0 -> Success
 *	@retval -1 -> Error
 *
 *
*/
BME280_RETURN_FUNCTION_TYPE BME280::bme280_set_spi3(u8 v_enable_disable_u8)
{
	/* used to return the communication result*/
	BME280_RETURN_FUNCTION_TYPE com_rslt = ERROR;
	u8 v_data_u8 = BME280_INIT_VALUE;
	u8 pre_ctrl_meas_value = BME280_INIT_VALUE;
	u8 v_prev_pow_mode_u8 = BME280_INIT_VALUE;
	u8 v_pre_ctrl_hum_value_u8 =  BME280_INIT_VALUE;
	/* check the p_bme280 structure pointer as NULL*/
	if (p_bme280 == BME280_NULL) {
		return E_BME280_NULL_PTR;
		} else {
			v_data_u8 = p_bme280->config_reg;
			v_data_u8 =
			BME280_SET_BITSLICE(v_data_u8,
			BME280_CONFIG_REG_SPI3_ENABLE, v_enable_disable_u8);
			com_rslt = bme280_get_power_mode(&v_prev_pow_mode_u8);
			if (v_prev_pow_mode_u8 != BME280_SLEEP_MODE) {
				com_rslt += bme280_set_soft_rst();
				p_bme280->delay_msec(BME280_3MS_DELAY);
				/* write previous and updated value of
				configuration register*/
				com_rslt += bme280_write_register(
					BME280_CONFIG_REG,
				&v_data_u8, BME280_GEN_READ_WRITE_DATA_LENGTH);
				/* write previous value of
				humidity oversampling*/
				v_pre_ctrl_hum_value_u8 =
				p_bme280->ctrl_hum_reg;
				com_rslt +=  bme280_write_register(
					BME280_CTRL_HUMIDITY_REG,
				&v_pre_ctrl_hum_value_u8,
				BME280_GEN_READ_WRITE_DATA_LENGTH);
				/* write previous value of
				control measurement register*/
				pre_ctrl_meas_value =
				p_bme280->ctrl_meas_reg;
				com_rslt += bme280_write_register(
					BME280_CTRL_MEAS_REG,
				&pre_ctrl_meas_value,
				BME280_GEN_READ_WRITE_DATA_LENGTH);
			} else {
				com_rslt =
				p_bme280->BME280_BUS_WRITE_FUNC(
				p_bme280->dev_addr,
				BME280_CONFIG_REG_SPI3_ENABLE__REG,
				&v_data_u8, BME280_GEN_READ_WRITE_DATA_LENGTH);
			}
			/* read the control measurement register value*/
			com_rslt += bme280_read_register(
				BME280_CTRL_MEAS_REG,
			&v_data_u8, BME280_GEN_READ_WRITE_DATA_LENGTH);
			p_bme280->ctrl_meas_reg = v_data_u8;
			/* read the control humidity register value*/
			com_rslt += bme280_read_register(
				BME280_CTRL_HUMIDITY_REG,
			&v_data_u8, BME280_GEN_READ_WRITE_DATA_LENGTH);
			p_bme280->ctrl_hum_reg = v_data_u8;
			/* read the control configuration register value*/
			com_rslt += bme280_read_register(
				BME280_CONFIG_REG,
			&v_data_u8, BME280_GEN_READ_WRITE_DATA_LENGTH);
			p_bme280->config_reg = v_data_u8;
		}
	return com_rslt;
}
/*!
 *	@brief This API is used to reads filter setting
 *	in the register 0xF5 bit 3 and 4
 *
 *
 *
 *	@param v_value_u8 : The value of IIR filter coefficient
 *
 *	value	    |	Filter coefficient
 * -------------|-------------------------
 *	0x00        | BME280_FILTER_COEFF_OFF
 *	0x01        | BME280_FILTER_COEFF_2
 *	0x02        | BME280_FILTER_COEFF_4
 *	0x03        | BME280_FILTER_COEFF_8
 *	0x04        | BME280_FILTER_COEFF_16
 *
 *	@return results of bus communication function
 *	@retval 0 -> Success
 *	@retval -1 -> Error
 *
 *
*/
BME280_RETURN_FUNCTION_TYPE BME280::bme280_get_filter(u8 *v_value_u8)
{
	/* used to return the communication result*/
	BME280_RETURN_FUNCTION_TYPE com_rslt = ERROR;
	u8 v_data_u8 = BME280_INIT_VALUE;
	/* check the p_bme280 structure pointer as NULL*/
	if (p_bme280 == BME280_NULL) {
		return E_BME280_NULL_PTR;
		} else {
			com_rslt = p_bme280->BME280_BUS_READ_FUNC(
			p_bme280->dev_addr,
			BME280_CONFIG_REG_FILTER__REG,
			&v_data_u8, BME280_GEN_READ_WRITE_DATA_LENGTH);
			*v_value_u8 = BME280_GET_BITSLICE(v_data_u8,
			BME280_CONFIG_REG_FILTER);
		}
	return com_rslt;
}
/*!
 *	@brief This API is used to write filter setting
 *	in the register 0xF5 bit 3 and 4
 *
 *
 *
 *	@param v_value_u8 : The value of IIR filter coefficient
 *
 *	value	    |	Filter coefficient
 * -------------|-------------------------
 *	0x00        | BME280_FILTER_COEFF_OFF
 *	0x01        | BME280_FILTER_COEFF_2
 *	0x02        | BME280_FILTER_COEFF_4
 *	0x03        | BME280_FILTER_COEFF_8
 *	0x04        | BME280_FILTER_COEFF_16
 *
 *	@return results of bus communication function
 *	@retval 0 -> Success
 *	@retval -1 -> Error
 *
 *
*/
BME280_RETURN_FUNCTION_TYPE BME280::bme280_set_filter(u8 v_value_u8)
{
	/* used to return the communication result*/
	BME280_RETURN_FUNCTION_TYPE com_rslt = ERROR;
	u8 v_data_u8 = BME280_INIT_VALUE;
	u8 pre_ctrl_meas_value = BME280_INIT_VALUE;
	u8 v_prev_pow_mode_u8 = BME280_INIT_VALUE;
	u8 v_pre_ctrl_hum_value_u8 =  BME280_INIT_VALUE;
	/* check the p_bme280 structure pointer as NULL*/
	if (p_bme280 == BME280_NULL) {
		return E_BME280_NULL_PTR;
		} else {
			v_data_u8 = p_bme280->config_reg;
			v_data_u8 =
			BME280_SET_BITSLICE(v_data_u8,
			BME280_CONFIG_REG_FILTER, v_value_u8);
			com_rslt = bme280_get_power_mode(&v_prev_pow_mode_u8);
			if (v_prev_pow_mode_u8 != BME280_SLEEP_MODE) {
				com_rslt += bme280_set_soft_rst();
				p_bme280->delay_msec(BME280_3MS_DELAY);
				/* write previous and updated value of
				configuration register*/
				com_rslt += bme280_write_register(
					BME280_CONFIG_REG,
				&v_data_u8, BME280_GEN_READ_WRITE_DATA_LENGTH);
				/* write previous value of
				humidity oversampling*/
				v_pre_ctrl_hum_value_u8 =
				p_bme280->ctrl_hum_reg;
				com_rslt += bme280_write_register(
					BME280_CTRL_HUMIDITY_REG,
				&v_pre_ctrl_hum_value_u8,
				BME280_GEN_READ_WRITE_DATA_LENGTH);
				/* write previous value of
				control measurement register*/
				pre_ctrl_meas_value =
				p_bme280->ctrl_meas_reg;
				com_rslt += bme280_write_register(
					BME280_CTRL_MEAS_REG,
				&pre_ctrl_meas_value,
				BME280_GEN_READ_WRITE_DATA_LENGTH);
			} else {
				com_rslt =
				p_bme280->BME280_BUS_WRITE_FUNC(
				p_bme280->dev_addr,
				BME280_CONFIG_REG_FILTER__REG,
				&v_data_u8, BME280_GEN_READ_WRITE_DATA_LENGTH);
			}
			/* read the control measurement register value*/
			com_rslt += bme280_read_register(BME280_CTRL_MEAS_REG,
			&v_data_u8, BME280_GEN_READ_WRITE_DATA_LENGTH);
			p_bme280->ctrl_meas_reg = v_data_u8;
			/* read the control humidity register value*/
			com_rslt += bme280_read_register(
			BME280_CTRL_HUMIDITY_REG,
			&v_data_u8, BME280_GEN_READ_WRITE_DATA_LENGTH);
			p_bme280->ctrl_hum_reg = v_data_u8;
			/* read the configuration register value*/
			com_rslt += bme280_read_register(BME280_CONFIG_REG,
			&v_data_u8, BME280_GEN_READ_WRITE_DATA_LENGTH);
			p_bme280->config_reg = v_data_u8;
		}
	return com_rslt;
}
/*!
 *	@brief This API used to Read the
 *	standby duration time from the sensor in the register 0xF5 bit 5 to 7
 *
 *	@param v_standby_durn_u8 : The value of standby duration time value.
 *  value       | standby duration
 * -------------|-----------------------
 *    0x00      | BME280_STANDBY_TIME_1_MS
 *    0x01      | BME280_STANDBY_TIME_63_MS
 *    0x02      | BME280_STANDBY_TIME_125_MS
 *    0x03      | BME280_STANDBY_TIME_250_MS
 *    0x04      | BME280_STANDBY_TIME_500_MS
 *    0x05      | BME280_STANDBY_TIME_1000_MS
 *    0x06      | BME280_STANDBY_TIME_2000_MS
 *    0x07      | BME280_STANDBY_TIME_4000_MS
 *
 *
 *	@return results of bus communication function
 *	@retval 0 -> Success
 *	@retval -1 -> Error
 *
 *
*/
BME280_RETURN_FUNCTION_TYPE BME280::bme280_get_standby_durn(u8 *v_standby_durn_u8)
{
	/* used to return the communication result*/
	BME280_RETURN_FUNCTION_TYPE com_rslt = ERROR;
	u8 v_data_u8 = BME280_INIT_VALUE;
	/* check the p_bme280 structure pointer as NULL*/
	if (p_bme280 == BME280_NULL) {
		return E_BME280_NULL_PTR;
		} else {
			com_rslt = p_bme280->BME280_BUS_READ_FUNC(
			p_bme280->dev_addr,
			BME280_CONFIG_REG_TSB__REG,
			&v_data_u8, BME280_GEN_READ_WRITE_DATA_LENGTH);
			*v_standby_durn_u8 = BME280_GET_BITSLICE(
			v_data_u8, BME280_CONFIG_REG_TSB);
		}
	return com_rslt;
}
/*!
 *	@brief This API used to write the
 *	standby duration time from the sensor in the register 0xF5 bit 5 to 7
 *
 *	@param v_standby_durn_u8 : The value of standby duration time value.
 *  value       | standby duration
 * -------------|-----------------------
 *    0x00      | BME280_STANDBY_TIME_1_MS
 *    0x01      | BME280_STANDBY_TIME_63_MS
 *    0x02      | BME280_STANDBY_TIME_125_MS
 *    0x03      | BME280_STANDBY_TIME_250_MS
 *    0x04      | BME280_STANDBY_TIME_500_MS
 *    0x05      | BME280_STANDBY_TIME_1000_MS
 *    0x06      | BME280_STANDBY_TIME_2000_MS
 *    0x07      | BME280_STANDBY_TIME_4000_MS
 *
 *	@note Normal mode comprises an automated perpetual
 *	cycling between an (active)
 *	Measurement period and an (inactive) standby period.
 *	@note The standby time is determined by
 *	the contents of the register t_sb.
 *	Standby time can be set using BME280_STANDBY_TIME_125_MS.
 *
 *	@note Usage Hint : bme280_set_standby_durn(BME280_STANDBY_TIME_125_MS)
 *
 *
 *
 *	@return results of bus communication function
 *	@retval 0 -> Success
 *	@retval -1 -> Error
 *
 *
*/
BME280_RETURN_FUNCTION_TYPE BME280::bme280_set_standby_durn(u8 v_standby_durn_u8)
{
	/* used to return the communication result*/
	BME280_RETURN_FUNCTION_TYPE com_rslt = ERROR;
	u8 v_data_u8 = BME280_INIT_VALUE;
	u8 pre_ctrl_meas_value = BME280_INIT_VALUE;
	u8 v_prev_pow_mode_u8 = BME280_INIT_VALUE;
	u8 v_pre_ctrl_hum_value_u8 = BME280_INIT_VALUE;
	/* check the p_bme280 structure pointer as NULL*/
	if (p_bme280 == BME280_NULL) {
		return E_BME280_NULL_PTR;
		} else {
			v_data_u8 = p_bme280->config_reg;
			v_data_u8 =
			BME280_SET_BITSLICE(v_data_u8,
			BME280_CONFIG_REG_TSB, v_standby_durn_u8);
			com_rslt = bme280_get_power_mode(&v_prev_pow_mode_u8);
			if (v_prev_pow_mode_u8 != BME280_SLEEP_MODE) {
				com_rslt += bme280_set_soft_rst();
				p_bme280->delay_msec(BME280_3MS_DELAY);
				/* write previous and updated value of
				configuration register*/
				com_rslt += bme280_write_register(
					BME280_CONFIG_REG,
				&v_data_u8, BME280_GEN_READ_WRITE_DATA_LENGTH);
				/* write previous value of
				humidity oversampling*/
				v_pre_ctrl_hum_value_u8 =
				p_bme280->ctrl_hum_reg;
				com_rslt += bme280_write_register(
					BME280_CTRL_HUMIDITY_REG,
				&v_pre_ctrl_hum_value_u8,
				BME280_GEN_READ_WRITE_DATA_LENGTH);
				/* write previous value of control
				measurement register*/
				pre_ctrl_meas_value =
				p_bme280->ctrl_meas_reg;
				com_rslt += bme280_write_register(
					BME280_CTRL_MEAS_REG,
				&pre_ctrl_meas_value,
				BME280_GEN_READ_WRITE_DATA_LENGTH);
			} else {
				com_rslt =
				p_bme280->BME280_BUS_WRITE_FUNC(
				p_bme280->dev_addr,
				BME280_CONFIG_REG_TSB__REG,
				&v_data_u8, BME280_GEN_READ_WRITE_DATA_LENGTH);
			}
			/* read the control measurement register value*/
			com_rslt += bme280_read_register(BME280_CTRL_MEAS_REG,
			&v_data_u8, BME280_GEN_READ_WRITE_DATA_LENGTH);
			p_bme280->ctrl_meas_reg = v_data_u8;
			/* read the control humidity register value*/
			com_rslt += bme280_read_register(
			BME280_CTRL_HUMIDITY_REG,
			&v_data_u8, BME280_GEN_READ_WRITE_DATA_LENGTH);
			p_bme280->ctrl_hum_reg = v_data_u8;
			/* read the configuration register value*/
			com_rslt += bme280_read_register(BME280_CONFIG_REG,
			&v_data_u8, BME280_GEN_READ_WRITE_DATA_LENGTH);
			p_bme280->config_reg = v_data_u8;
		}
	return com_rslt;
}
/*!
 * @brief This API used to read uncompensated
 * temperature,pressure and humidity in forced mode
 *
 *
 *	@param v_uncom_pressure_s32: The value of uncompensated pressure
 *	@param v_uncom_temperature_s32: The value of uncompensated temperature
 *	@param v_uncom_humidity_s32: The value of uncompensated humidity
 *
 *
 *	@return results of bus communication function
 *	@retval 0 -> Success
 *	@retval -1 -> Error
 *
 *
*/
BME280_RETURN_FUNCTION_TYPE
BME280::bme280_get_forced_uncomp_pressure_temperature_humidity(
s32 *v_uncom_pressure_s32,
s32 *v_uncom_temperature_s32, s32 *v_uncom_humidity_s32)
{
	/* used to return the communication result*/
	BME280_RETURN_FUNCTION_TYPE com_rslt = ERROR;
	u8 v_data_u8 = BME280_INIT_VALUE;
	u8 v_waittime_u8r = BME280_INIT_VALUE;
	u8 v_prev_pow_mode_u8 = BME280_INIT_VALUE;
	u8 v_mode_u8r = BME280_INIT_VALUE;
	u8 pre_ctrl_config_value = BME280_INIT_VALUE;
	u8 v_pre_ctrl_hum_value_u8 = BME280_INIT_VALUE;
	/* check the p_bme280 structure pointer as NULL*/
	if (p_bme280 == BME280_NULL) {
		return E_BME280_NULL_PTR;
		} else {
			v_mode_u8r = p_bme280->ctrl_meas_reg;
			v_mode_u8r =
			BME280_SET_BITSLICE(v_mode_u8r,
			BME280_CTRL_MEAS_REG_POWER_MODE, BME280_FORCED_MODE);
			com_rslt = bme280_get_power_mode(&v_prev_pow_mode_u8);
			if (v_prev_pow_mode_u8 != BME280_SLEEP_MODE) {
				com_rslt += bme280_set_soft_rst();
				p_bme280->delay_msec(BME280_3MS_DELAY);
				/* write previous and updated value of
				configuration register*/
				pre_ctrl_config_value = p_bme280->config_reg;
				com_rslt += bme280_write_register(
					BME280_CONFIG_REG,
				&pre_ctrl_config_value,
				BME280_GEN_READ_WRITE_DATA_LENGTH);
				/* write previous value of
				humidity oversampling*/
				v_pre_ctrl_hum_value_u8 =
				p_bme280->ctrl_hum_reg;
				com_rslt += bme280_write_register(
					BME280_CTRL_HUMIDITY_REG,
				&v_pre_ctrl_hum_value_u8,
				BME280_GEN_READ_WRITE_DATA_LENGTH);
				/* write the force mode  */
				com_rslt += bme280_write_register(
					BME280_CTRL_MEAS_REG,
				&v_mode_u8r, BME280_GEN_READ_WRITE_DATA_LENGTH);
			} else {
				/* write previous value of
				humidity oversampling*/
				v_pre_ctrl_hum_value_u8 =
				p_bme280->ctrl_hum_reg;
				com_rslt += bme280_write_register(
					BME280_CTRL_HUMIDITY_REG,
				&v_pre_ctrl_hum_value_u8,
				BME280_GEN_READ_WRITE_DATA_LENGTH);
				/* write the force mode  */
				com_rslt += bme280_write_register(
					BME280_CTRL_MEAS_REG,
				&v_mode_u8r, BME280_GEN_READ_WRITE_DATA_LENGTH);
			}
			bme280_compute_wait_time(&v_waittime_u8r);
			p_bme280->delay_msec(v_waittime_u8r);
			/* read the force-mode value of pressure
			temperature and humidity*/
			com_rslt +=
			bme280_read_uncomp_pressure_temperature_humidity(
			v_uncom_pressure_s32, v_uncom_temperature_s32,
			v_uncom_humidity_s32);

			/* read the control humidity register value*/
			com_rslt += bme280_read_register(
			BME280_CTRL_HUMIDITY_REG,
			&v_data_u8, BME280_GEN_READ_WRITE_DATA_LENGTH);
			p_bme280->ctrl_hum_reg = v_data_u8;
			/* read the configuration register value*/
			com_rslt += bme280_read_register(BME280_CONFIG_REG,
			&v_data_u8, BME280_GEN_READ_WRITE_DATA_LENGTH);
			p_bme280->config_reg = v_data_u8;

			/* read the control measurement register value*/
			com_rslt += bme280_read_register(BME280_CTRL_MEAS_REG,
			&v_data_u8, BME280_GEN_READ_WRITE_DATA_LENGTH);
			p_bme280->ctrl_meas_reg = v_data_u8;
		}
	return com_rslt;
}
/*!
 * @brief
 *	This API write the data to
 *	the given register
 *
 *
 *	@param v_addr_u8 -> Address of the register
 *	@param v_data_u8 -> The data from the register
 *	@param v_len_u8 -> no of bytes to read
 *
 *
 *	@return results of bus communication function
 *	@retval 0 -> Success
 *	@retval -1 -> Error
 *
 *
 */
BME280_RETURN_FUNCTION_TYPE BME280::bme280_write_register(u8 v_addr_u8,
u8 *v_data_u8, u8 v_len_u8)
{
	/* used to return the communication result*/
	BME280_RETURN_FUNCTION_TYPE com_rslt = ERROR;
	/* check the p_bme280 structure pointer as NULL*/
	if (p_bme280 == BME280_NULL) {
		return E_BME280_NULL_PTR;
		} else {
			com_rslt = p_bme280->BME280_BUS_WRITE_FUNC(
			p_bme280->dev_addr,
			v_addr_u8, v_data_u8, v_len_u8);
		}
	return com_rslt;
}
/*!
 * @brief
 *	This API reads the data from
 *	the given register
 *
 *
 *	@param v_addr_u8 -> Address of the register
 *	@param v_data_u8 -> The data from the register
 *	@param v_len_u8 -> no of bytes to read
 *
 *
 *	@return results of bus communication function
 *	@retval 0 -> Success
 *	@retval -1 -> Error
 *
 *
 */
BME280_RETURN_FUNCTION_TYPE BME280::bme280_read_register(u8 v_addr_u8,
u8 *v_data_u8, u8 v_len_u8)
{
	/* used to return the communication result*/
	BME280_RETURN_FUNCTION_TYPE com_rslt = ERROR;
	/* check the p_bme280 structure pointer as NULL*/
	if (p_bme280 == BME280_NULL) {
		return E_BME280_NULL_PTR;
		} else {
			com_rslt = p_bme280->BME280_BUS_READ_FUNC(
			p_bme280->dev_addr,
			v_addr_u8, v_data_u8, v_len_u8);
		}
	return com_rslt;
}
#ifdef BME280_ENABLE_FLOAT
/*!
 * @brief Reads actual temperature from uncompensated temperature
 * @note returns the value in Degree centigrade
 * @note Output value of "51.23" equals 51.23 DegC.
 *
 *
 *
 *  @param v_uncom_temperature_s32 : value of uncompensated temperature
 *
 *
 *
 *  @return  Return the actual temperature in floating point
 *
*/
double BME280::bme280_compensate_temperature_double(s32 v_uncom_temperature_s32)
{
	double v_x1_u32 = BME280_INIT_VALUE;
	double v_x2_u32 = BME280_INIT_VALUE;
	double temperature = BME280_INIT_VALUE;

	v_x1_u32  = (((double)v_uncom_temperature_s32) / 16384.0 -
	((double)p_bme280->cal_param.dig_T1) / 1024.0) *
	((double)p_bme280->cal_param.dig_T2);
	v_x2_u32  = ((((double)v_uncom_temperature_s32) / 131072.0 -
	((double)p_bme280->cal_param.dig_T1) / 8192.0) *
	(((double)v_uncom_temperature_s32) / 131072.0 -
	((double)p_bme280->cal_param.dig_T1) / 8192.0)) *
	((double)p_bme280->cal_param.dig_T3);
	p_bme280->cal_param.t_fine = (s32)(v_x1_u32 + v_x2_u32);
	temperature  = (v_x1_u32 + v_x2_u32) / 5120.0;


	return temperature;
}
/*!
 * @brief Reads actual pressure from uncompensated pressure
 * @note Returns pressure in Pa as double.
 * @note Output value of "96386.2"
 * equals 96386.2 Pa = 963.862 hPa.
 *
 *
 *  @param v_uncom_pressure_s32 : value of uncompensated pressure
 *
 *
 *  @return  Return the actual pressure in floating point
 *
*/
double BME280::bme280_compensate_pressure_double(s32 v_uncom_pressure_s32)
{
	double v_x1_u32 = BME280_INIT_VALUE;
	double v_x2_u32 = BME280_INIT_VALUE;
	double pressure = BME280_INIT_VALUE;

	v_x1_u32 = ((double)p_bme280->cal_param.t_fine /
	2.0) - 64000.0;
	v_x2_u32 = v_x1_u32 * v_x1_u32 *
	((double)p_bme280->cal_param.dig_P6) / 32768.0;
	v_x2_u32 = v_x2_u32 + v_x1_u32 *
	((double)p_bme280->cal_param.dig_P5) * 2.0;
	v_x2_u32 = (v_x2_u32 / 4.0) +
	(((double)p_bme280->cal_param.dig_P4) * 65536.0);
	v_x1_u32 = (((double)p_bme280->cal_param.dig_P3) *
	v_x1_u32 * v_x1_u32 / 524288.0 +
	((double)p_bme280->cal_param.dig_P2) * v_x1_u32) / 524288.0;
	v_x1_u32 = (1.0 + v_x1_u32 / 32768.0) *
	((double)p_bme280->cal_param.dig_P1);
	pressure = 1048576.0 - (double)v_uncom_pressure_s32;
	/* Avoid exception caused by division by zero */
	if (v_x1_u32 != BME280_INIT_VALUE)
		pressure = (pressure - (v_x2_u32 / 4096.0)) * 6250.0 / v_x1_u32;
	else
		return BME280_INVALID_DATA;
	v_x1_u32 = ((double)p_bme280->cal_param.dig_P9) *
	pressure * pressure / 2147483648.0;
	v_x2_u32 = pressure * ((double)p_bme280->cal_param.dig_P8) / 32768.0;
	pressure = pressure + (v_x1_u32 + v_x2_u32 +
	((double)p_bme280->cal_param.dig_P7)) / 16.0;

	return pressure;
}
/*!
 * @brief Reads actual humidity from uncompensated humidity
 * @note returns the value in relative humidity (%rH)
 * @note Output value of "42.12" equals 42.12 %rH
 *
 *  @param v_uncom_humidity_s32 : value of uncompensated humidity
 *
 *
 *
 *  @return Return the actual humidity in floating point
 *
*/
double BME280::bme280_compensate_humidity_double(s32 v_uncom_humidity_s32)
{
	double var_h = BME280_INIT_VALUE;

	var_h = (((double)p_bme280->cal_param.t_fine) - 76800.0);
	if (var_h != BME280_INIT_VALUE)
		var_h = (v_uncom_humidity_s32 -
		(((double)p_bme280->cal_param.dig_H4) * 64.0 +
		((double)p_bme280->cal_param.dig_H5) / 16384.0 * var_h))*
		(((double)p_bme280->cal_param.dig_H2) / 65536.0 *
		(1.0 + ((double) p_bme280->cal_param.dig_H6)
		/ 67108864.0 * var_h * (1.0 + ((double)
		p_bme280->cal_param.dig_H3) / 67108864.0 * var_h)));
	else
		return BME280_INVALID_DATA;
	var_h = var_h * (1.0 - ((double)
	p_bme280->cal_param.dig_H1)*var_h / 524288.0);
	if (var_h > 100.0)
		var_h = 100.0;
	else if (var_h < 0.0)
		var_h = 0.0;
	return var_h;

}
#endif
#if defined(BME280_ENABLE_INT64) && defined(BME280_64BITSUPPORT_PRESENT)
/*!
 * @brief Reads actual pressure from uncompensated pressure
 * @note Returns the value in Pa as unsigned 32 bit
 * integer in Q24.8 format (24 integer bits and
 * 8 fractional bits).
 * @note Output value of "24674867"
 * represents 24674867 / 256 = 96386.2 Pa = 963.862 hPa
 *
 *
 *
 *  @param  v_uncom_pressure_s32 : value of uncompensated temperature
 *
 *
 *  @return Return the actual pressure in u32
 *
*/
u32 BME280::bme280_compensate_pressure_int64(s32 v_uncom_pressure_s32)
{
	s64 v_x1_s64r = BME280_INIT_VALUE;
	s64 v_x2_s64r = BME280_INIT_VALUE;
	s64 pressure = BME280_INIT_VALUE;

	v_x1_s64r = ((s64)p_bme280->cal_param.t_fine)
	- 128000;
	v_x2_s64r = v_x1_s64r * v_x1_s64r *
	(s64)p_bme280->cal_param.dig_P6;
	v_x2_s64r = v_x2_s64r + ((v_x1_s64r *
	(s64)p_bme280->cal_param.dig_P5)
	<< BME280_SHIFT_BIT_POSITION_BY_17_BITS);
	v_x2_s64r = v_x2_s64r +
	(((s64)p_bme280->cal_param.dig_P4)
	<< BME280_SHIFT_BIT_POSITION_BY_35_BITS);
	v_x1_s64r = ((v_x1_s64r * v_x1_s64r *
	(s64)p_bme280->cal_param.dig_P3)
	>> BME280_SHIFT_BIT_POSITION_BY_08_BITS) +
	((v_x1_s64r * (s64)p_bme280->cal_param.dig_P2)
	<< BME280_SHIFT_BIT_POSITION_BY_12_BITS);
	v_x1_s64r = (((((s64)1)
	<< BME280_SHIFT_BIT_POSITION_BY_47_BITS) + v_x1_s64r)) *
	((s64)p_bme280->cal_param.dig_P1)
	>> BME280_SHIFT_BIT_POSITION_BY_33_BITS;
	pressure = 1048576 - v_uncom_pressure_s32;
	/* Avoid exception caused by division by zero */
	if (v_x1_s64r != BME280_INIT_VALUE)
		#if defined __KERNEL__
			pressure = div64_s64((((pressure
			<< BME280_SHIFT_BIT_POSITION_BY_31_BITS) - v_x2_s64r)
			* 3125), v_x1_s64r);
		#else
			pressure = (((pressure
			<< BME280_SHIFT_BIT_POSITION_BY_31_BITS) - v_x2_s64r)
			* 3125) / v_x1_s64r;
		#endif
	else
		return BME280_INVALID_DATA;
	v_x1_s64r = (((s64)p_bme280->cal_param.dig_P9) *
	(pressure >> BME280_SHIFT_BIT_POSITION_BY_13_BITS) *
	(pressure >> BME280_SHIFT_BIT_POSITION_BY_13_BITS))
	>> BME280_SHIFT_BIT_POSITION_BY_25_BITS;
	v_x2_s64r = (((s64)p_bme280->cal_param.dig_P8) *
	pressure) >> BME280_SHIFT_BIT_POSITION_BY_19_BITS;
	pressure = (((pressure + v_x1_s64r +
	v_x2_s64r) >> BME280_SHIFT_BIT_POSITION_BY_08_BITS) +
	(((s64)p_bme280->cal_param.dig_P7)
	<< BME280_SHIFT_BIT_POSITION_BY_04_BITS));

	return (u32)pressure;
}
/*!
 * @brief Reads actual pressure from uncompensated pressure
 * @note Returns the value in Pa.
 * @note Output value of "12337434"
 * @note represents 12337434 / 128 = 96386.2 Pa = 963.862 hPa
 *
 *
 *
 *  @param v_uncom_pressure_s32 : value of uncompensated pressure
 *
 *
 *  @return the actual pressure in u32
 *
*/
u32 BME280::bme280_compensate_pressure_int64_twentyfour_bit_output(
s32 v_uncom_pressure_s32)
{
	u32 pressure = BME280_INIT_VALUE;

	pressure = bme280_compensate_pressure_int64(
	v_uncom_pressure_s32);
	pressure = (u32)(pressure >> BME280_SHIFT_BIT_POSITION_BY_01_BIT);
	return pressure;
}
#endif
/*!
 * @brief Computing waiting time for sensor data read
 *
 *
 *
 *
 *  @param v_delaytime_u8 : The value of delay time for force mode
 *
 *
 *	@retval 0 -> Success
 *
 *
 */
BME280_RETURN_FUNCTION_TYPE BME280::bme280_compute_wait_time(u8
*v_delaytime_u8)
{
	/* used to return the communication result*/
	BME280_RETURN_FUNCTION_TYPE com_rslt = SUCCESS;

	*v_delaytime_u8 = (T_INIT_MAX +
	T_MEASURE_PER_OSRS_MAX *
	(((1 <<
	p_bme280->oversamp_temperature)
	>> BME280_SHIFT_BIT_POSITION_BY_01_BIT)
	+ ((1 << p_bme280->oversamp_pressure)
	>> BME280_SHIFT_BIT_POSITION_BY_01_BIT) +
	((1 << p_bme280->oversamp_humidity)
	>> BME280_SHIFT_BIT_POSITION_BY_01_BIT))
	+ (p_bme280->oversamp_pressure ?
	T_SETUP_PRESSURE_MAX : 0) +
	(p_bme280->oversamp_humidity ?
	T_SETUP_HUMIDITY_MAX : 0) + 15) / 16;
	return com_rslt;
}





/******** bme support code ***************/


/* This function is an example for reading sensor temperature
 *	\param: None
 *	\return: compensated temperature
 */

s32 BME280::getTemperatureInternal(void)
{
	/* The variable used to read uncompensated temperature*/
	s32 v_data_uncomp_tem_s32 =  getTemperatureRawInternal();

/*------------------------------------------------------------------*
************ START READ TRUE PRESSURE, TEMPERATURE AND HUMIDITY DATA ********
*---------------------------------------------------------------------*/
	/* API is used to read the true temperature*/
	/* Input value as uncompensated temperature and output format*/
	s32 v_actual_temp_s32 = bme280_compensate_temperature_int32(v_data_uncomp_tem_s32);
/*--------------------------------------------------------------------*
************ END READ TRUE TEMPERATURE  ********
*-------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------*
************************* START DE-INITIALIZATION ***********************
*-------------------------------------------------------------------------*/
	/*	For de-initialization it is required to set the mode of
	 *	the sensor as "SLEEP"
	 *	the device reaches the lowest power consumption only
	 *	In SLEEP mode no measurements are performed
	 *	All registers are accessible
	 *	by using the below API able to set the power mode as SLEEP*/
	 /* Set the power mode as SLEEP*/
	bme280_set_power_mode(BME280_SLEEP_MODE);
/*---------------------------------------------------------------------*
************************* END DE-INITIALIZATION **********************
*---------------------------------------------------------------------*/
return v_actual_temp_s32;
}

/* This function is an example for reading sensor pressure
 *	\param: None
 *	\return: communication result
 */

s32 BME280::getPressureInternal(void)
{
	/* The variable used to read uncompensated pressure*/
	s32 v_data_uncomp_pres_s32 = getPressureInternal();


/*------------------------------------------------------------------*
************ START READ TRUE PRESSURE DATA ********
*---------------------------------------------------------------------*/
	/* API is used to read the true pressure*/
	/* Input value as uncompensated pressure */
	u32 v_actual_press_u32 = bme280_compensate_pressure_int32(v_data_uncomp_pres_s32);

/*--------------------------------------------------------------------*
************ END READ TRUE PRESSURE, TEMPERATURE AND HUMIDITY ********
*-------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------*
************************* START DE-INITIALIZATION ***********************
*-------------------------------------------------------------------------*/
	/*	For de-initialization it is required to set the mode of
	 *	the sensor as "SLEEP"
	 *	the device reaches the lowest power consumption only
	 *	In SLEEP mode no measurements are performed
	 *	All registers are accessible
	 *	by using the below API able to set the power mode as SLEEP*/
	 /* Set the power mode as SLEEP*/
	bme280_set_power_mode(BME280_SLEEP_MODE);
/*---------------------------------------------------------------------*
************************* END DE-INITIALIZATION **********************
*---------------------------------------------------------------------*/
return v_actual_press_u32;
}


/* This function is an example for reading sensor humidity
 *	\param: None
 *	\return: communication result
 */

s32 BME280::getHumidityInternal(void)
{
	/* The variable used to read uncompensated pressure*/
	s32 v_data_uncomp_hum_s32 = getHumidityInternal();

/*------------------------------------------------------------------*
************ START READ TRUE PRESSURE, TEMPERATURE AND HUMIDITY DATA ********
*---------------------------------------------------------------------*/
	/* API is used to read the true humidity*/
	/* Input value as uncompensated humidity and output format*/
	 u32 v_actual_humity_u32 = bme280_compensate_humidity_int32(v_data_uncomp_hum_s32);

/*--------------------------------------------------------------------*
************ END READ TRUE PRESSURE, TEMPERATURE AND HUMIDITY ********
*-------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------*
************************* START DE-INITIALIZATION ***********************
*-------------------------------------------------------------------------*/
	/*	For de-initialization it is required to set the mode of
	 *	the sensor as "SLEEP"
	 *	the device reaches the lowest power consumption only
	 *	In SLEEP mode no measurements are performed
	 *	All registers are accessible
	 *	by using the below API able to set the power mode as SLEEP*/
	 /* Set the power mode as SLEEP*/
	bme280_set_power_mode(BME280_SLEEP_MODE);
/*---------------------------------------------------------------------*
************************* END DE-INITIALIZATION **********************
*---------------------------------------------------------------------*/
return v_actual_humity_u32;
}


/******** bme support code ***************/


/* This function is an example for reading sensor temperature
 *	\param: None
 *	\return: uncompensated temperature
 */

s32 BME280::getTemperatureRawInternal(void)
{
	/* The variable used to read uncompensated temperature*/
	s32 v_data_uncomp_tem_s32 = BME280_INIT_VALUE;
	/* The variable used to read real temperature*/
	s32 v_actual_temp_s32 = BME280_INIT_VALUE;

	/*	For initialization it is required to set the mode of
	 *	the sensor as "NORMAL"
	 *	data acquisition/read/write is possible in this mode
	 *	by using the below API able to set the power mode as NORMAL*/
	/* Set the power mode as NORMAL*/
	bme280_set_power_mode(BME280_NORMAL_MODE);
	/*	For reading the  temperature data it is required to
	 *	set the OSS setting of temperature
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
	bme280_read_uncomp_temperature(&v_data_uncomp_tem_s32);


/*--------------------------------------------------------------------*
************ END READ UNCOMPENSATED TEMPERATURE  ********
*-------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------*
************************* START DE-INITIALIZATION ***********************
*-------------------------------------------------------------------------*/
	/*	For de-initialization it is required to set the mode of
	 *	the sensor as "SLEEP"
	 *	the device reaches the lowest power consumption only
	 *	In SLEEP mode no measurements are performed
	 *	All registers are accessible
	 *	by using the below API able to set the power mode as SLEEP*/
	 /* Set the power mode as SLEEP*/
	bme280_set_power_mode(BME280_SLEEP_MODE);
/*---------------------------------------------------------------------*
************************* END DE-INITIALIZATION **********************
*---------------------------------------------------------------------*/
return v_data_uncomp_tem_s32;
}

/* This function is an example for reading sensor pressure
 *	\param: None
 *	\return: uncompensated pressure 
 */

s32 BME280::getPressureRawInternal(void)
{
	/* The variable used to read uncompensated pressure*/
	s32 v_data_uncomp_pres_s32 = BME280_INIT_VALUE;
	/* The variable used to read real pressure*/
	u32 v_actual_press_u32 = BME280_INIT_VALUE;





	/*	For initialization it is required to set the mode of
	 *	the sensor as "NORMAL"
	 *	data acquisition/read/write is possible in this mode
	 *	by using the below API able to set the power mode as NORMAL*/
	/* Set the power mode as NORMAL*/
	bme280_set_power_mode(BME280_NORMAL_MODE);
	/*	For reading the pressure data it is required to
	 *	set the OSS setting of humidity, pressure and temperature
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
	bme280_read_uncomp_pressure(&v_data_uncomp_pres_s32);

/*--------------------------------------------------------------------*
************ END READ UNCOMPENSATED PRESSURE ********
*-------------------------------------------------------------------------*/


/*-----------------------------------------------------------------------*
************************* START DE-INITIALIZATION ***********************
*-------------------------------------------------------------------------*/
	/*	For de-initialization it is required to set the mode of
	 *	the sensor as "SLEEP"
	 *	the device reaches the lowest power consumption only
	 *	In SLEEP mode no measurements are performed
	 *	All registers are accessible
	 *	by using the below API able to set the power mode as SLEEP*/
	 /* Set the power mode as SLEEP*/
	bme280_set_power_mode(BME280_SLEEP_MODE);
/*---------------------------------------------------------------------*
************************* END DE-INITIALIZATION **********************
*---------------------------------------------------------------------*/
return v_data_uncomp_pres_s32;
}


/* This function is an example for reading sensor humidity
 *	\param: None
 *	\return: uncompensated humidity
 */

s32 BME280::getHumidityRawInternal(void)
{
	/* The variable used to read uncompensated pressure*/
	s32 v_data_uncomp_hum_s32 = BME280_INIT_VALUE;
	/* The variable used to read real humidity*/
	u32 v_actual_humity_u32 = BME280_INIT_VALUE;






	/*	For initialization it is required to set the mode of
	 *	the sensor as "NORMAL"
	 *	data acquisition/read/write is possible in this mode
	 *	by using the below API able to set the power mode as NORMAL*/
	/* Set the power mode as NORMAL*/
	bme280_set_power_mode(BME280_NORMAL_MODE);
	/*	For reading  humidity  data it is required to
	 *	set the OSS setting of humidity
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
        bme280_read_uncomp_humidity(&v_data_uncomp_hum_s32);

/*--------------------------------------------------------------------*
************ END READ UNCOMPENSATED PRESSURE AND TEMPERATURE********
*-------------------------------------------------------------------------*/


/*-----------------------------------------------------------------------*
************************* START DE-INITIALIZATION ***********************
*-------------------------------------------------------------------------*/
	/*	For de-initialization it is required to set the mode of
	 *	the sensor as "SLEEP"
	 *	the device reaches the lowest power consumption only
	 *	In SLEEP mode no measurements are performed
	 *	All registers are accessible
	 *	by using the below API able to set the power mode as SLEEP*/
	 /* Set the power mode as SLEEP*/
	bme280_set_power_mode(BME280_SLEEP_MODE);
/*---------------------------------------------------------------------*
************************* END DE-INITIALIZATION **********************
*---------------------------------------------------------------------*/
return v_data_uncomp_hum_s32;
}





/*--------------------------------------------------------------------------*
*	The following function is used to map the I2C bus read, write, delay and
*	device address with global structure bme280
*-------------------------------------------------------------------------*/
s8 BME280::I2C_routine()
//s8 I2C_routine()
 {
/*--------------------------------------------------------------------------*
 *  By using bme280 the following structure parameter can be accessed
 *	Bus write function pointer: BME280_WR_FUNC_PTR
 *	Bus read function pointer: BME280_RD_FUNC_PTR
 *	Delay function pointer: delay_msec
 *	I2C address: dev_addr
 *--------------------------------------------------------------------------*/
//	bme280.bus_write = &BME280::BME280_I2C_bus_write;
	bme280.bus_write = BME280_I2C_bus_write;

	//bme280.bus_write = BME280_I2C_bus_write_dummy;
	bme280.bus_read = BME280_I2C_bus_read;
	bme280.dev_addr = BME280_I2C_ADDRESS1;
	bme280.delay_msec = BME280_delay_msek;

	return BME280_INIT_VALUE;
}



/*-------------------------------------------------------------------*
*	The device address defined in the bme280.h file
*-----------------------------------------------------------------------*/
//pravin
s32 BME280::i2c_write_string(u8 dev_addr,u8* ptr, u8 cnt)
//s32 i2c_write_string(u8 dev_addr,u8 * ptr, u8 cnt)

{
	mraa::Result ret;
	m_i2c->address(dev_addr);
	
	if((ret = m_i2c->write((const uint8_t*) (ptr), cnt)) != 0)
	//if((ret = m_i2c->writeReg((uint8_t) (*ptr), cnt)) != 0)
	{
	  	UPM_THROW("I2C write error");

	}

} 

/*	\Brief: The function is used as I2C bus write
*	\Return : Status of the I2C write
*	\param dev_addr : The device address of the sensor
*	\param reg_addr : Address of the first register, will data is going to be written
*	\param reg_data : It is a value hold in the array,
*		will be used for write the value into the register
*	\param cnt : The no of byte of data to be write
*/

//s8 BME280_I2C_bus_write(u8 dev_addr, u8 reg_addr, u8 *reg_data, u8 cnt)
s8 BME280::BME280_I2C_bus_write(u8 dev_addr, u8 reg_addr, u8 *reg_data, u8 cnt)

{
	s32 iError = BME280_INIT_VALUE;
	static u8 array[I2C_BUFFER_LEN];
        for (int i=0; i<I2C_BUFFER_LEN; i++) array[i]=0;
	
	u8 stringpos = BME280_INIT_VALUE;
	array[BME280_INIT_VALUE] = reg_addr;
	for (stringpos = BME280_INIT_VALUE; stringpos < cnt; stringpos++) {
		array[stringpos + BME280_ONE_U8X] = *(reg_data + stringpos);
	}
	iError = i2c_write_string(dev_addr,array, cnt+1);

	/*
	* Please take the below function as your reference for
	* write the data using I2C communication
	* "IERROR = I2C_WRITE_STRING(DEV_ADDR, ARRAY, CNT+1)"
	* add your I2C write function here
	* iError is an return value of I2C read function
	* Please select your valid return value
	* In the driver SUCCESS defined as 0
    * and FAILURE defined as -1
	* Note :
	* This is a full duplex operation,
	* The first read data is discarded, for that extra write operation
	* have to be initiated. For that cnt+1 operation done in the I2C write string function
	*/
	return (s8)iError;
}

s32 BME280::i2c_write_read_string(u8 dev_addr,u8 reg_addr , u8 * ptr, u8 cnt)
//s32 i2c_write_read_string(u8 dev_addr,u8 reg_addr , u8 * ptr, u8 cnt)

{
	 mraa::Result ret;

	m_i2c->address(dev_addr);
	
	if( m_i2c->readBytesReg(reg_addr, ptr, cnt) != cnt)
	{
		UPM_THROW("bme280 register read failed");

	}
	return 0;
}

 /*	\Brief: The function is used as I2C bus read
 *	\Return : Status of the I2C read
 *	\param dev_addr : The device address of the sensor
 *	\param reg_addr : Address of the first register, will data is going to be read
 *	\param reg_data : This data read from the sensor, which is hold in an array
 *	\param cnt : The no of data byte of to be read
 */
s8 BME280::BME280_I2C_bus_read(u8 dev_addr, u8 reg_addr, u8 *reg_data, u8 cnt)
//s8 BME280_I2C_bus_read(u8 dev_addr, u8 reg_addr, u8 *reg_data, u8 cnt)

{
	s32 iError = BME280_INIT_VALUE;
	u8 array[I2C_BUFFER_LEN] = {BME280_INIT_VALUE};
	u8 stringpos = BME280_INIT_VALUE;
	array[BME280_INIT_VALUE] = reg_addr;
	/* Please take the below function as your reference
	 * for read the data using I2C communication
	 * add your I2C rad function here.
	 * "IERROR = I2C_WRITE_READ_STRING(DEV_ADDR, ARRAY, ARRAY, 1, CNT)"
	 * Please select your valid return value
     * In the driver SUCCESS defined as 0
     * and FAILURE defined as -1
	 i*/

	i2c_write_read_string(dev_addr,reg_addr,array,cnt);
	for (stringpos = BME280_INIT_VALUE; stringpos < cnt; stringpos++) {
		*(reg_data + stringpos) = array[stringpos];
	}
	return (s8)iError;
}

/*	Brief : The delay routine
 *	\param : delay in ms
*/
void BME280::BME280_delay_msek(u32 mseconds)
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
        int BME280::getTemperatureCelcius (){ return BME280::getTemperatureInternal(); } 
        /**
         * Get relative humidity measurement.
         */
        uint16_t BME280::getHumidityRaw (){ return BME280::getHumidityRawInternal(); }

        /**
         * Get relative humidity measurement.
         */
        int BME280::getHumidityRelative (){ return BME280::getHumidityInternal(); }

        /**
         * Return pressure
         */
        uint32_t BME280::getPressureRaw(){  return BME280::getPressureRawInternal(); }

        /**
         * Return calculated pressure (Pa)
         */
        int BME280::getPressurePa(){ return BME280::getPressureInternal(); }



