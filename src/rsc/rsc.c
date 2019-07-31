/*
 * Author: Abhishek Malik <abhishek.malik@intel.com>
 * Copyright (c) 2017 Intel Corporation.
 *
 * This program and the accompanying materials are made available under the
 * terms of the The MIT License which is available at
 * https://opensource.org/licenses/MIT.
 *
 * SPDX-License-Identifier: MIT
 */
#include "rsc.h"
#include "upm_utilities.h"

upm_result_t rsc_eeprom_read(rsc_context dev, uint16_t address, uint8_t* buf, int len, uint8_t arglen);

upm_result_t rsc_adc_write(rsc_context dev, uint8_t reg, uint8_t num_bytes, uint8_t* write_data);

upm_result_t rsc_adc_read(rsc_context dev, READING_T type, uint8_t* data);

upm_result_t rsc_add_dr_delay(rsc_context dev);

void rsc_set_access_type(rsc_context dev, ACCESS_T type);
 
rsc_context rsc_init(int bus, int cs_ee_pin, int cs_adc_pin) {
    // make sure MRAA is initialized
    int mraa_rv;
    if ((mraa_rv = mraa_init()) != MRAA_SUCCESS)
    {
        printf("%s: mraa_init() failed (%d).\n", __FUNCTION__, mraa_rv);
        return NULL;
    }

    rsc_context dev = (rsc_context) malloc(sizeof(struct _rsc_context));
    if (!dev) {
        return NULL;
    }

    dev->spi_bus_number = bus;

    dev->spi = mraa_spi_init(dev->spi_bus_number);
    if(dev->spi == NULL)
        printf("RSC: SPI context not initialized\n");

    // initializing the EEPROM chip select
    dev->cs_ee = mraa_gpio_init(cs_ee_pin);
    if(dev->cs_ee == NULL)
        printf("RSC: EEPROM GPIO context not initialized\n");

    if(mraa_gpio_dir(dev->cs_ee, MRAA_GPIO_OUT) != MRAA_SUCCESS)
        printf("RSC: EEPROM GPIO direction could not be set\n");
    mraa_gpio_write(dev->cs_ee, 1);

    // initializing the ADC chip select
    dev->cs_adc = mraa_gpio_init(cs_adc_pin);
    if(dev->cs_adc == NULL)
        printf("RSC: ADC GPIO context not initialized\n");

    if(mraa_gpio_dir(dev->cs_adc, MRAA_GPIO_OUT) != MRAA_SUCCESS)
        printf("RSC: ADC GPIO direction could not be set\n");
    mraa_gpio_write(dev->cs_adc, 1);

    // setting the frequency and spi mode
    mraa_spi_frequency(dev->spi, 1250000);
    //mraa_spi_mode(dev->spi, MRAA_SPI_MODE0);
    //rsc_set_access_type(dev, EEPEROM);
	
    upm_delay_ms(100);

    uint8_t sensor_name[RSC_SENSOR_NAME_LEN]={0};
    rsc_get_sensor_name(dev, sensor_name);
    //printf("sensor name: %s\n", sensor_name);

    upm_delay_ms(10);
    uint8_t serial_number[RSC_SENSOR_NUMBER_LEN]={0};
    rsc_get_sensor_serial_number(dev, serial_number);
    //printf("sensor serial number: %s\n", serial_number);

    upm_delay_ms(10);
    float range = rsc_get_pressure_range(dev);
    dev->pressure_range = range;

    upm_delay_ms(10);
    float min_pressure = rsc_get_minimum_pressure(dev);
    dev->min_pressure_val = min_pressure;

    upm_delay_ms(10);
    rsc_get_pressure_unit(dev);

    upm_delay_ms(10);
    rsc_get_pressure_type(dev);

    uint8_t adc_init_values[4];
    rsc_get_initial_adc_values(dev, adc_init_values);

    rsc_retrieve_coefficients(dev);

    //mraa_spi_frequency(dev->spi, 1250000);
    //mraa_spi_mode(dev->spi, MRAA_SPI_MODE1);
    //rsc_set_access_type(dev, ADC);
    rsc_setup_adc(dev, adc_init_values);

    rsc_set_data_rate(dev, N_DR_20_SPS);
    rsc_set_mode(dev, NORMAL_MODE);

    rsc_get_temperature(dev);
    upm_delay_ms(50);

    return dev;
}

upm_result_t rsc_close(rsc_context dev) {
    free(dev);
    return UPM_SUCCESS;
}

void rsc_set_access_type(rsc_context dev, ACCESS_T type) {
    switch(type) {
        case EEPROM:
            mraa_spi_mode(dev->spi, MRAA_SPI_MODE0);
        break;
        case ADC:
            mraa_spi_mode(dev->spi, MRAA_SPI_MODE1);
        break;
        default:
            // default mode is EEPROM
            mraa_spi_mode(dev->spi, MRAA_SPI_MODE0);
    }
}

upm_result_t rsc_eeprom_read(rsc_context dev, uint16_t address, uint8_t* buf, int len, uint8_t arglen) {
    uint8_t lbuf[len+arglen];
    lbuf[0] = RSC_READ_EEPROM_INSTRUCTION;
    lbuf[1] = address & 0xff;
    lbuf[0] = ((address & RSC_EEPROM_ADDRESS_9TH_BIT_MASK) >> 5) | lbuf[0];

    mraa_gpio_write(dev->cs_ee, 0);
    if(mraa_spi_transfer_buf(dev->spi, lbuf, lbuf, len+arglen) != MRAA_SUCCESS) {
        printf("RSC: ISsues in SPI transfer\n");
        return UPM_ERROR_OPERATION_FAILED;
    }

    mraa_gpio_write(dev->cs_ee, 1);
    int i=0;
    for(i=arglen;i<len+arglen;i++)
        buf[i-2] = lbuf[i];

    return UPM_SUCCESS;
}

upm_result_t rsc_get_sensor_name(rsc_context dev, uint8_t* sensor_name) {
    rsc_set_access_type(dev, EEPROM);
    if(rsc_eeprom_read(dev, RSC_CATALOG_LISTING_MSB, sensor_name, RSC_SENSOR_NAME_LEN, RSC_EEPROM_STANDARD_ARGUMENT_LENGTH) != UPM_SUCCESS) {
        return UPM_ERROR_OPERATION_FAILED;
    }

    sensor_name[RSC_SENSOR_NAME_LEN-1]='\0'; 

    return UPM_SUCCESS;
}

upm_result_t rsc_get_sensor_serial_number(rsc_context dev, uint8_t* rsc_number) {
    rsc_set_access_type(dev, EEPROM);
    if(rsc_eeprom_read(dev, RSC_SERIAL_NO_YYYY_MSB, rsc_number, RSC_SENSOR_NUMBER_LEN, RSC_EEPROM_STANDARD_ARGUMENT_LENGTH) != UPM_SUCCESS) {
        return UPM_ERROR_OPERATION_FAILED;
    }

    rsc_number[RSC_SENSOR_NUMBER_LEN-1]='\0';

    return UPM_SUCCESS;
}

float rsc_get_pressure_range(rsc_context dev) {
    uint32_t alt_range;
    uint8_t buf[RSC_PRESSURE_RANGE_LEN]={0};

    rsc_set_access_type(dev, EEPROM);

    rsc_eeprom_read(dev, RSC_PRESSURE_RANGE_LSB, buf, RSC_PRESSURE_RANGE_LEN, RSC_EEPROM_STANDARD_ARGUMENT_LENGTH);

    alt_range = (buf[0] | (buf[1]<<8) | (buf[2]<<16) | (buf[3]<<24));

    return *(float*)&alt_range;
}

float rsc_get_minimum_pressure(rsc_context dev) {
    uint32_t alt_range;
    uint8_t buf[RSC_PRESSURE_MINIMUM_LEN]={0};

    rsc_set_access_type(dev, EEPROM);

    rsc_eeprom_read(dev, RSC_PRESSURE_MINIMUM_LSB, buf, RSC_PRESSURE_MINIMUM_LEN, RSC_EEPROM_STANDARD_ARGUMENT_LENGTH);

    alt_range = (buf[0] | (buf[1]<<8) | (buf[2]<<16) | (buf[3]<<24));

    return *(float*)&alt_range;
}

PRESSURE_U rsc_get_pressure_unit(rsc_context dev) {
    uint8_t pressure_unit[RSC_PRESSURE_UNIT_LEN]={0};

    rsc_set_access_type(dev, EEPROM);

    rsc_eeprom_read(dev, RSC_PRESSURE_UNIT_MSB, pressure_unit, RSC_PRESSURE_UNIT_LEN, RSC_EEPROM_STANDARD_ARGUMENT_LENGTH);

    pressure_unit[RSC_PRESSURE_UNIT_LEN-1]='\0';

    if(pressure_unit[RSC_PRESSURE_UNIT_LEN-2] == 'O')
        dev->unit = INH2O;
    else if(pressure_unit[RSC_PRESSURE_UNIT_LEN-2] == 'a')
        if(pressure_unit[RSC_PRESSURE_UNIT_LEN-4] == 'K')
            dev->unit = KPASCAL;
        else if (pressure_unit[RSC_PRESSURE_UNIT_LEN-4] == 'M')
            dev->unit = MPASCAL;
        else
            dev->unit = PASCAL;
    else if(pressure_unit[RSC_PRESSURE_UNIT_LEN-2] == 'r')
        if(pressure_unit[RSC_PRESSURE_UNIT_LEN-5] == 'm')
            dev->unit = mBAR;
        else
            dev->unit = BAR;
    else if(pressure_unit[RSC_PRESSURE_UNIT_LEN-2] == 'i')
        dev->unit = PSI;

    return dev->unit;
}

PRESSURE_T rsc_get_pressure_type(rsc_context dev) {
    uint8_t type;

    rsc_set_access_type(dev, EEPROM);

    rsc_eeprom_read(dev, RSC_PRESSURE_REFERENCE, &type, RSC_SENSOR_TYPE_LEN, RSC_EEPROM_STANDARD_ARGUMENT_LENGTH);

    switch (type) {
        case 'D':
            dev->type = DIFFERENTIAL;
        break;
        case 'A':
            dev->type = ABSOLUTE;
        break;
        case 'G':
            dev->type = GAUGE;
        break;
        default:
            dev->type = DIFFERENTIAL;
    }

    return dev->type;
}

upm_result_t rsc_get_initial_adc_values(rsc_context dev, uint8_t* adc_init_values) {
    rsc_set_access_type(dev, EEPROM);

    rsc_eeprom_read(dev, RSC_ADC_CONDIG_00, &adc_init_values[0], 1, RSC_EEPROM_STANDARD_ARGUMENT_LENGTH);
    upm_delay_ms(2);
    rsc_eeprom_read(dev, RSC_ADC_CONDIG_01, &adc_init_values[1], 1, RSC_EEPROM_STANDARD_ARGUMENT_LENGTH);
    upm_delay_ms(2);
    rsc_eeprom_read(dev, RSC_ADC_CONDIG_02, &adc_init_values[2], 1, RSC_EEPROM_STANDARD_ARGUMENT_LENGTH);
    upm_delay_ms(2);
    rsc_eeprom_read(dev, RSC_ADC_CONDIG_03, &adc_init_values[3], 1, RSC_EEPROM_STANDARD_ARGUMENT_LENGTH);

    return UPM_SUCCESS;
}

upm_result_t rsc_retrieve_coefficients(rsc_context dev) {
    int base_address = RSC_OFFSET_COEFFICIENT_0_LSB;
    uint8_t l_coeffs[RSC_COEFF_ADDRESS_SPACE_SIZE]={0};
    int i,j = 0;
    uint32_t temp;

    rsc_set_access_type(dev, EEPROM);

    for (i=0; i<RSC_COEFF_T_ROW_NO; i++) {
        // 80 is the number of bytes that separate the beginning
        // of the address spaces of all the 3 coefficient groups
        // refer the datasheet for more info
        base_address = RSC_OFFSET_COEFFICIENT_0_LSB + i*80;
        if(rsc_eeprom_read(dev, base_address, l_coeffs, (RSC_COEFF_ADDRESS_SPACE_SIZE), RSC_EEPROM_STANDARD_ARGUMENT_LENGTH) != UPM_SUCCESS) {
            return UPM_ERROR_OPERATION_FAILED;
        }

        // storing all the coefficients
        for(j=0; j<RSC_COEFF_T_COL_NO; j++) {
            temp = l_coeffs[j*4+0] | 
                   (l_coeffs[j*4+1]<<8) |
                   (l_coeffs[j*4+2]<<16) |
                   (l_coeffs[j*4+3]<<24);
            dev->coeff_matrix[i][j] = *(float*)&temp;
        }
    }

    return UPM_SUCCESS;
}

upm_result_t rsc_adc_write(rsc_context dev, uint8_t reg, uint8_t num_bytes, uint8_t* write_data) {
    // The number of bytes to write has to be - 1,2,3,4
    if(num_bytes <= 0 || num_bytes >4)
        return UPM_ERROR_UNSPECIFIED;

    // the ADC registers are 0,1,2,3
    if(reg >3)
        return UPM_ERROR_UNSPECIFIED;

    uint8_t tx[num_bytes+1];
    // The ADC REG Write command is as follows: 0100 RRNN
    // R - Register Number (0,1,2,3) N - Number of Bytes (0,1,2,3) (0 means 1)
    tx[0] = RSC_ADC_WREG|((reg<<2)&RSC_ADC_REG_MASK)|((num_bytes-1)&RSC_ADC_NUM_BYTES_MASK);
    int cnt = 0;
    for(cnt=0; cnt<num_bytes; cnt++)
        tx[cnt+1] = write_data[cnt];

    mraa_gpio_write(dev->cs_adc, 0);
    if(mraa_spi_transfer_buf(dev->spi, tx, NULL, num_bytes+1) != MRAA_SUCCESS) {
        printf("RSC: ISsues in SPI transfer\n");
        return UPM_ERROR_OPERATION_FAILED;
    }
    mraa_gpio_write(dev->cs_adc, 1);
    return UPM_SUCCESS;
}

upm_result_t rsc_set_data_rate(rsc_context dev, RSC_DATA_RATE dr) {
    dev->data_rate = dr;
    switch (dr) {
        case N_DR_20_SPS:
        case N_DR_45_SPS:
        case N_DR_90_SPS:
        case N_DR_175_SPS:
        case N_DR_330_SPS:
        case N_DR_600_SPS:
        case N_DR_1000_SPS:
            rsc_set_mode(dev, NORMAL_MODE);
        break;
        case F_DR_40_SPS:
        case F_DR_90_SPS:
        case F_DR_180_SPS:
        case F_DR_350_SPS:
        case F_DR_660_SPS:
        case F_DR_1200_SPS:
        case F_DR_2000_SPS:
            rsc_set_mode(dev, FAST_MODE);
        break;
        default:
            rsc_set_mode(dev, NA_MODE);
    }
    return UPM_SUCCESS;
}

upm_result_t rsc_set_mode(rsc_context dev, RSC_MODE mode) {
    RSC_MODE l_mode;

    switch(mode) {
        case NORMAL_MODE:
            if(dev->data_rate < N_DR_20_SPS || dev->data_rate > N_DR_1000_SPS) {
                printf("RSC: Normal mode not supported with the current selection of data rate\n");
                printf("RSC: You will see erronous readings\n");
                l_mode = NA_MODE;
            } else
                l_mode = NORMAL_MODE;
        break;
        case FAST_MODE:
            if(dev->data_rate < F_DR_40_SPS || dev->data_rate > F_DR_2000_SPS) {
                printf("RSC: Fast mode not supported with the current selection of data rate\n");
                printf("RSC: You will see erronous readings\n");
                l_mode = NA_MODE;
            } else
                l_mode = FAST_MODE;
        break;
        default:
            l_mode = NA_MODE;
    }
    dev->mode = l_mode;

    return UPM_SUCCESS;
}

upm_result_t rsc_adc_read(rsc_context dev, READING_T type, uint8_t* data) { 
    uint8_t tx[2]={0};
    tx[0] = RSC_ADC_WREG|((1<<2)&RSC_ADC_REG_MASK);

    // Composing tx[1], which includes Mode, DataRate, Pressure/Temperature choice
    tx[1] = (((dev->data_rate << RSC_DATA_RATE_SHIFT)&RSC_DATA_RATE_MASK) |
            ((dev->mode << RSC_OPERATING_MODE_SHIFT)&RSC_OPERATING_MODE_MASK) |
            (((type&0x01)<<1)|RSC_SET_BITS_MASK));

    mraa_gpio_write(dev->cs_adc, 0);
    if(mraa_spi_transfer_buf(dev->spi, tx, NULL, 2) != MRAA_SUCCESS) {
        printf("RSC: ISsues in SPI transfer\n");
        return UPM_ERROR_OPERATION_FAILED;
    }
    mraa_gpio_write(dev->cs_adc, 1);

    // delay would depend on data rate
    rsc_add_dr_delay(dev);
    uint8_t tx_1[4]={0x10, 0, 0, 0};
    mraa_gpio_write(dev->cs_adc, 0);
    if(mraa_spi_transfer_buf(dev->spi, tx_1, data, 4) != MRAA_SUCCESS) {
        printf("RSC: ISsues in SPI transfer\n");
        return UPM_ERROR_OPERATION_FAILED;
    }
    mraa_gpio_write(dev->cs_adc, 1);

    return UPM_SUCCESS;
}

float rsc_get_temperature(rsc_context dev) {
    uint8_t sec_arr[4]={0};
    float temp;

    rsc_set_access_type(dev, ADC);

    rsc_adc_read(dev, TEMPERATURE, sec_arr);
    dev->t_raw = ((sec_arr[1]<<8) | sec_arr[2]) >> 2;
    temp = dev->t_raw*0.03125;

    return temp;
}

float rsc_get_pressure(rsc_context dev) {
    uint8_t sec_arr[4]={0};

    rsc_set_access_type(dev, ADC);

    rsc_adc_read(dev, PRESSURE, sec_arr);

    uint32_t p_raw = (sec_arr[1]<<16)|(sec_arr[2]<<8)|sec_arr[3];

    uint32_t t_raw = dev->t_raw;
    float x = (dev->coeff_matrix[0][3]*t_raw*t_raw*t_raw);
    float y = (dev->coeff_matrix[0][2]*t_raw*t_raw);
    float z = (dev->coeff_matrix[0][1]*t_raw);
    float p_int1 = p_raw - (x + y + z + dev->coeff_matrix[0][0]);

    x = (dev->coeff_matrix[1][3]*t_raw*t_raw*t_raw);
    y = (dev->coeff_matrix[1][2]*t_raw*t_raw);
    z = (dev->coeff_matrix[1][1]*t_raw);
    float p_int2 = p_int1/(x + y + z + dev->coeff_matrix[1][0]);

    x = (dev->coeff_matrix[2][3]*p_int2*p_int2*p_int2);
    y = (dev->coeff_matrix[2][2]*p_int2*p_int2);
    z = (dev->coeff_matrix[2][1]*p_int2);
    float p_comp_fs = x + y + z + dev->coeff_matrix[2][0];

    float p_comp = (p_comp_fs*dev->pressure_range) + dev->min_pressure_val;

    return p_comp;
}

upm_result_t rsc_setup_adc(rsc_context dev, uint8_t* adc_init_values) {
    uint8_t tx=RSC_ADC_RESET_COMMAND;

    rsc_set_access_type(dev, ADC);

    mraa_gpio_write(dev->cs_adc, 0);
    if(mraa_spi_transfer_buf(dev->spi, &tx, NULL, 1) != MRAA_SUCCESS) {
        printf("RSC: ISsues in SPI transfer\n");
        return UPM_ERROR_OPERATION_FAILED;
    }
    upm_delay_ms(5);

    uint8_t arr[4] = {adc_init_values[0], adc_init_values[1], adc_init_values[2], adc_init_values[3]};
    rsc_adc_write(dev, 0, 4, arr);

    mraa_gpio_write(dev->cs_adc, 1);
    upm_delay_ms(5);

    return UPM_SUCCESS;
}

upm_result_t rsc_add_dr_delay(rsc_context dev) {
    float delay = 0;
    // calculating delay based on the Data Rate
    switch(dev->data_rate){
        case N_DR_20_SPS:
            delay = MSEC_PER_SEC/20;
        break;
        case N_DR_45_SPS:
            delay = MSEC_PER_SEC/45;
        break;
        case N_DR_90_SPS:
            delay = MSEC_PER_SEC/90;
        break;
        case N_DR_175_SPS:
            delay = MSEC_PER_SEC/175;
        break;
        case N_DR_330_SPS:
            delay = MSEC_PER_SEC/330;
        break;
        case N_DR_600_SPS:
            delay = MSEC_PER_SEC/600;
        break;
        case N_DR_1000_SPS:
            delay = MSEC_PER_SEC/1000;
        break;
        case F_DR_40_SPS:
            delay = MSEC_PER_SEC/40;
        break;
        case F_DR_90_SPS:
            delay = MSEC_PER_SEC/90;
        break;
        case F_DR_180_SPS:
            delay = MSEC_PER_SEC/180;
        break;
        case F_DR_350_SPS:
            delay = MSEC_PER_SEC/350;
        break;
        case F_DR_660_SPS:
            delay = MSEC_PER_SEC/660;
        break;
        case F_DR_1200_SPS:
            delay = MSEC_PER_SEC/1200;
        break;
        case F_DR_2000_SPS:
            delay = MSEC_PER_SEC/2000;
        break;
        default:
            delay = 50;
    }
    upm_delay_ms((int)delay + 2);

    return UPM_SUCCESS;
}
