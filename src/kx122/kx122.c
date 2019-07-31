/*
* The MIT License (MIT)
*
* Author: Samuli Rissanen <samuli.rissanen@hotmail.com>
* Copyright (c) 2018 Rohm Semiconductor.
*
* This program and the accompanying materials are made available under the
* terms of the The MIT License which is available at
* https://opensource.org/licenses/MIT.
*
* SPDX-License-Identifier: MIT
*/

#include "kx122.h"

//Used to set the bit required for SPI reading
#define SPI_READ 0x80

//Used to mask the read bit required for SPI writing
#define SPI_WRITE 0x7F

//Used to determine amount of samples in the buffer.
#define LOW_RES_SAMPLE_MODIFIER 3
#define HIGH_RES_SAMPLE_MODIFIER 6

//Mask for software reset
#define KX122_CNTL2_SRST_MASK 0x80

//Masks for interrupt control registers
#define KX122_INC1_MASK 0xFB
#define KX122_INC4_MASK 0xF7
#define KX122_INC5_MASK 0xFB
#define KX122_INC6_MASK 0xF7

//Acceleration data buffer length
#define BUFFER_LEN 6

//Acceleration per decimal value for each acceleration ranges
#define RANGE_2G_G 0.00006f
#define RANGE_4G_G 0.00012f
#define RANGE_8G_G 0.00024f

//Acceleration scaling between high and low resolution modes
#define RANGE_RES_SCALE 260

//Maximum loop iterations, used in software reset and self testing
#define MAX_LOOP_COUNT 100

//Sensor self-test defines
#define SELF_TEST_LOOP_WAIT_TIME 10000
#define SELF_TEST_MAX_DIFFERENCE 0.75f
#define SELF_TEST_MIN_XY_DIFFERENCE 0.25f
#define SELF_TEST_MIN_Z_DIFFERENCE 0.20f

//Maximum amount of samples in the buffer for high and low resolutions
#define MAX_BUFFER_SAMPLES_LOW_RES 681
#define MAX_BUFFER_SAMPLES_HIGH_RES 340

//Earth gravity constant (m/s^2)
#define GRAVITY 9.81f
/**
Enables the chip select on the chip_select GPIO pin.
Chip select is active low.

@param dev The device context.
*/
static void kx122_chip_select_on(const kx122_context dev);

/**
Disables the chip select on the chip_select GPIO pin.

@param dev The device context.
*/
static void kx122_chip_select_off(const kx122_context dev);

/**
Reads a register.

@param dev The device context.
@param reg The register to read.
@param data Pointer to a uint8_t variable to store the value.
@return UPM result
*/
static upm_result_t kx122_read_register(const kx122_context dev, uint8_t reg, uint8_t *data);

/**
Reads multiple continous registers.

@param dev The device context.
@param reg The start register.
@param buffer Pointer to a uint8_t buffer to store the values.
@param len Amount of bytes to read.
@return UPM result
*/
static upm_result_t kx122_read_registers(const kx122_context dev, uint8_t reg, uint8_t *buffer, uint len);

/**
Writes to a register.

@param dev The device context.
@param reg The register to write to.
@param val The value to write to the register.
@return UPM result
*/
static upm_result_t kx122_write_register(const kx122_context dev, uint8_t reg, uint8_t val);

/**
Writes to a register using a bitmask.

@param dev The device context.
@param reg The register to write to.
@param val The value to write to the register.
@param bit_mask The bitmask to be used.
@return UPM result.
*/
static upm_result_t kx122_set_bit_pattern(const kx122_context dev, uint8_t reg, uint8_t val, uint8_t bit_mask);

/**
Sets a specific bit on (1) in a register.

@param dev The device context.
@param reg The register to write to.
@param val The value to write to the register.
@return UPM result.
*/
static upm_result_t kx122_set_bit_on(const kx122_context dev, uint8_t reg, uint8_t val);

/**
Sets a specific bit off (0) in a register.

@param dev The device context.
@param reg The register to write to.
@param val The value to write to the register.
@return UPM result.
*/
static upm_result_t kx122_set_bit_off(const kx122_context dev, uint8_t reg, uint8_t val);

/**
Sets the default internal variable values.

@param dev The device context.
*/
static void kx122_set_default_values(const kx122_context dev);

/**
Sets acceleration scale of sensor and the buffer.

@param dev The device context.
@param grange One of the KX122_RANGE_T values.
*/
static void kx122_map_grange(const kx122_context dev, KX122_RANGE_T grange);

kx122_context kx122_init(int bus, int addr, int chip_select_pin, int spi_bus_frequency)
{
  kx122_context dev = (kx122_context)malloc(sizeof(struct _kx122_context));

  if(!dev){
    return NULL;
  }

  dev->using_spi = false;

  dev->i2c = NULL;
  dev->spi = NULL;
  dev->chip_select = NULL;

  dev->gpio1 = NULL;
  dev->gpio2 = NULL;

  if(mraa_init() != MRAA_SUCCESS){
    printf("%s: mraa_init() failed.\n", __FUNCTION__);
    kx122_close(dev);
    return NULL;
  }

  if(addr == -1){
    dev->using_spi = true;
  }

  if(dev->using_spi){

    if (spi_bus_frequency > 10000000){	// KX122 has a maximum SPI bus speed of 10MHz
      printf("%s: bus frequency too high - KX122 has a maximum SPI bus speed of 10MHz.\n", __FUNCTION__);
      kx122_close(dev);
      return NULL;
    }

    if (!(dev->spi = mraa_spi_init(bus))){
      printf("%s: mraa_spi_init() failed.\n", __FUNCTION__);
      kx122_close(dev);
      return NULL;
    }

    if (!(dev->chip_select = mraa_gpio_init(chip_select_pin))){
      printf("%s: mraa_gpio_init() failed.\n", __FUNCTION__);
      kx122_close(dev);
      return NULL;
    }

    mraa_gpio_dir(dev->chip_select,MRAA_GPIO_OUT);
    mraa_spi_mode(dev->spi,MRAA_SPI_MODE0);

    if (mraa_spi_frequency(dev->spi, spi_bus_frequency)){
      printf("%s: mraa_spi_frequency() failed.\n", __FUNCTION__);
      kx122_close(dev);
      return NULL;
    }
  }
  else{ //Using I2C
    if (!(dev->i2c = mraa_i2c_init(bus))){
      printf("%s: mraa_i2c_init() failed, used bus: %d\n", __FUNCTION__,bus);
      kx122_close(dev);
      return NULL;
    }

    if (mraa_i2c_address(dev->i2c, addr)){
      printf("%s: mraa_i2c_address() failed.\n", __FUNCTION__);
      kx122_close(dev);
      return NULL;
    }
  }

  uint8_t who_am_i;
  kx122_get_who_am_i(dev,&who_am_i);

  if(who_am_i != KX122_WHO_AM_I_WIA_ID){
    printf("%s: Wrong WHO AM I received, expected: 0x%x | got: 0x%x\n", __FUNCTION__,KX122_WHO_AM_I_WIA_ID,who_am_i);
    kx122_close(dev);
    return NULL;
  }

  kx122_set_default_values(dev);

  kx122_device_init(dev,KX122_ODR_50,HIGH_RES,KX122_RANGE_2G);
  return dev;
}

void kx122_close(kx122_context dev)
{
  assert(dev != NULL);
  kx122_uninstall_isr(dev,INT1);
  kx122_uninstall_isr(dev,INT2);

  if(dev->i2c){
    mraa_i2c_stop(dev->i2c);
  }
  if(dev->spi){
    mraa_spi_stop(dev->spi);
  }
  if(dev->chip_select){
    mraa_gpio_close(dev->chip_select);
  }
  free(dev);
}

static void kx122_chip_select_on(const kx122_context dev)
{
  if(dev->chip_select){
    mraa_gpio_write(dev->chip_select,0);
  }
}

static void kx122_chip_select_off(const kx122_context dev)
{
  if(dev->chip_select){
    mraa_gpio_write(dev->chip_select,1);
  }
}

//Register operations
static upm_result_t kx122_read_register(const kx122_context dev, uint8_t reg, uint8_t *data)
{
  if(dev->using_spi){
    reg |= SPI_READ;

    uint8_t spi_data[2] = {reg,0};

    kx122_chip_select_on(dev);

    if(mraa_spi_transfer_buf(dev->spi,spi_data,spi_data,(sizeof(spi_data) / sizeof(uint8_t))) != MRAA_SUCCESS){
      printf("%s: mraa_spi_transfer_buf() failed.\n", __FUNCTION__);

      kx122_chip_select_off(dev);
      return UPM_ERROR_OPERATION_FAILED;
    }
      *data = spi_data[1];

      kx122_chip_select_off(dev);
      return UPM_SUCCESS;
  }
  else{
    int value = mraa_i2c_read_byte_data(dev->i2c,reg);

    if(value != -1){
      *data = (uint8_t) value;
      return UPM_SUCCESS;
    }

    printf("%s: mraa_i2c_read_byte_data() failed, reading from register 0x%x\n",__FUNCTION__, reg);
    return UPM_ERROR_OPERATION_FAILED;
  }
}

static upm_result_t kx122_read_registers(const kx122_context dev, uint8_t reg, uint8_t *buffer, uint len)
{
  if(dev->using_spi){
    reg |= SPI_READ;

    uint8_t spi_data_buffer[len + 1];
    spi_data_buffer[0] = reg;

    kx122_chip_select_on(dev);

    if(mraa_spi_transfer_buf(dev->spi,spi_data_buffer,spi_data_buffer,
    (sizeof(spi_data_buffer) / sizeof(uint8_t))) != MRAA_SUCCESS){
      printf("%s: mraa_spi_transfer_buf() failed.\n", __FUNCTION__);

      kx122_chip_select_off(dev);
      return UPM_ERROR_OPERATION_FAILED;
    }

    for (size_t i = 0; i < len; i++) {
      buffer[i] = spi_data_buffer[i + 1];
    }

    kx122_chip_select_off(dev);
    return UPM_SUCCESS;
  }
  else{
      uint8_t data_buffer[len];
      if(mraa_i2c_read_bytes_data(dev->i2c,reg,data_buffer,len) != (int)len){
        return UPM_ERROR_OPERATION_FAILED;
      }
      else{
        for(size_t i = 0; i < len; i++){
          buffer[i] = data_buffer[i];
        }
        return UPM_SUCCESS;
      }
  }
}

static upm_result_t kx122_write_register(const kx122_context dev, uint8_t reg, uint8_t val)
{
  if(dev->using_spi){
    reg &= SPI_WRITE;
    uint8_t spi_data[2] = {reg,val};

    kx122_chip_select_on(dev);
    if(mraa_spi_transfer_buf(dev->spi,spi_data,NULL,(sizeof(spi_data) / sizeof(uint8_t))) != MRAA_SUCCESS){
      printf("%s: mraa_spi_transfer_buf() failed.\n", __FUNCTION__);

      kx122_chip_select_off(dev);
      return UPM_ERROR_OPERATION_FAILED;
    }

    kx122_chip_select_off(dev);
    return UPM_SUCCESS;
  }
  else{
    if(mraa_i2c_write_byte_data(dev->i2c,val,reg) != MRAA_SUCCESS){
      printf("%s: mraa_i2c_write_byte_data() failed.\n",__FUNCTION__);
      return UPM_ERROR_OPERATION_FAILED;
    }
    return UPM_SUCCESS;
  }
}

static upm_result_t kx122_set_bit_pattern(const kx122_context dev, uint8_t reg, uint8_t val, uint8_t bit_mask)
{
  uint8_t reg_val;

  if(kx122_read_register(dev,reg,&reg_val) != UPM_SUCCESS){
    return UPM_ERROR_OPERATION_FAILED;
  }

  reg_val &= ~bit_mask;
  reg_val |= val;
  return kx122_write_register(dev,reg,reg_val);
}

static upm_result_t kx122_set_bit_on(const kx122_context dev, uint8_t reg, uint8_t val)
{
  uint8_t reg_val;

  if(kx122_read_register(dev,reg,&reg_val) != UPM_SUCCESS){
    return UPM_ERROR_OPERATION_FAILED;
  }

  reg_val |= val;
  return kx122_write_register(dev,reg,reg_val);
}

static upm_result_t kx122_set_bit_off(const kx122_context dev, uint8_t reg, uint8_t val)
{
  uint8_t reg_val;

  if(kx122_read_register(dev,reg,&reg_val) != UPM_SUCCESS){
    return UPM_ERROR_OPERATION_FAILED;
  }

  reg_val &= ~val;
  return kx122_write_register(dev,reg,reg_val);
}

static void kx122_set_default_values(const kx122_context dev)
{
  dev->accel_scale = RANGE_2G_G;
  dev->grange_mode = KX122_RANGE_2G;
  dev->res_mode = HIGH_RES;

  dev->buffer_accel_scale = RANGE_2G_G;
  dev->buffer_res = LOW_RES;
  dev->buffer_mode = KX122_FIFO_MODE;
}

static void kx122_map_grange(const kx122_context dev, KX122_RANGE_T grange)
{
  switch (grange) {
    case KX122_RANGE_2G:
      dev->accel_scale = RANGE_2G_G;
      dev->buffer_accel_scale = RANGE_2G_G;
      break;
    case KX122_RANGE_4G:
      dev->accel_scale = RANGE_4G_G;
      dev->buffer_accel_scale = RANGE_4G_G;
      break;
    case KX122_RANGE_8G:
      dev->accel_scale = RANGE_8G_G;
      dev->buffer_accel_scale = RANGE_8G_G;
      break;
  }

  if(dev->res_mode == LOW_RES){
    dev->accel_scale *= RANGE_RES_SCALE;
  }
  if(dev->buffer_res == LOW_RES){
    dev->buffer_accel_scale *= RANGE_RES_SCALE;
  }

  dev->grange_mode = grange;
}

float kx122_get_sample_period(const kx122_context dev)
{
  assert(dev != NULL);

  odr_item odr_map[16] = {
    {KX122_ODR_0P781,0.781f},{KX122_ODR_1P563,1.563f},{KX122_ODR_3P125,3.125f},{KX122_ODR_6P25,6.25f},
    {KX122_ODR_12P5,12.5f},{KX122_ODR_25,25},{KX122_ODR_50,50},{KX122_ODR_100,100},{KX122_ODR_200,200},
    {KX122_ODR_400,400},{KX122_ODR_800,800},{KX122_ODR_1600,1600},{KX122_ODR_3200,3200},{KX122_ODR_6400,6400},
    {KX122_ODR_12800,128000},{KX122_ODR_25600,25600}
  };

  uint8_t reg_val = 0;

  kx122_read_register(dev,KX122_ODCNTL, &reg_val);
  reg_val = reg_val & KX122_ODCNTL_OSA_MASK;

  for (size_t i = 0; i < (sizeof(odr_map) / sizeof(odr_item)); i++) {
    if(reg_val == odr_map[i].odr_value){
      return (1 / odr_map[i].odr_decimal);
    }
  }
  return -1; //Return -1 if failed
}
//Driver functions
upm_result_t kx122_device_init(const kx122_context dev, KX122_ODR_T odr, KX122_RES_T res, KX122_RANGE_T grange)
{
  assert(dev != NULL);
  kx122_set_sensor_standby(dev);

  if(kx122_set_odr(dev,odr) != UPM_SUCCESS){
    return UPM_ERROR_OPERATION_FAILED;
  }
  if(kx122_set_resolution(dev,res) != UPM_SUCCESS){
    return UPM_ERROR_OPERATION_FAILED;
  }
  if(kx122_set_grange(dev,grange) != UPM_SUCCESS){
    return UPM_ERROR_OPERATION_FAILED;
  }

  kx122_set_sensor_active(dev);
  return UPM_SUCCESS;
}

upm_result_t kx122_get_who_am_i(const kx122_context dev, uint8_t *data)
{
  assert(dev != NULL);
  return kx122_read_register(dev,KX112_WHO_AM_I,data);
}

upm_result_t kx122_set_sensor_standby(const kx122_context dev)
{
	assert(dev != NULL);
	return kx122_set_bit_off(dev,KX122_CNTL1,KX122_CNTL1_PC1);
}

upm_result_t kx122_set_sensor_active(const kx122_context dev)
{
  assert(dev != NULL);
  upm_result_t result = kx122_set_bit_on(dev,KX122_CNTL1,KX122_CNTL1_PC1);

  usleep((kx122_get_sample_period(dev) * 1.5f) * MICRO_S); //Sleep 1.5/ODR

  return result;
}

upm_result_t kx122_get_acceleration_data_raw(const kx122_context dev, float *x, float *y, float *z)
{
  assert(dev != NULL);

  uint8_t buffer[BUFFER_LEN];

  if(kx122_read_registers(dev,KX122_XOUT_L,buffer,BUFFER_LEN) != UPM_SUCCESS){
    printf("%s: kx122_read_registers() failed to read %d bytes\n",__FUNCTION__, BUFFER_LEN);
    return UPM_ERROR_OPERATION_FAILED;
  }

  if(dev->res_mode == HIGH_RES){
    if(x){
      *x = (float)((int16_t)(buffer[1] << 8) | buffer[0]);
    }
    if(y){
      *y = (float)((int16_t)(buffer[3] << 8) | buffer[2]);
    }
    if(z){
      *z = (float)((int16_t)(buffer[5] << 8) | buffer[4]);
    }
  }
  else{
    if(x){
      *x = (float)(int8_t)buffer[1];
    }
    if(y){
      *y = (float)(int8_t)buffer[3];
    }
    if(z){
      *z = (float)(int8_t)buffer[5];
    }
  }

  return UPM_SUCCESS;
}

upm_result_t kx122_get_acceleration_data(const kx122_context dev, float *x, float *y, float *z)
{
  float x_raw,y_raw,z_raw;

  if(kx122_get_acceleration_data_raw(dev,&x_raw,&y_raw,&z_raw) != UPM_SUCCESS){
    return UPM_ERROR_OPERATION_FAILED;
  }

  if(x){
    *x = (x_raw * dev->accel_scale) * GRAVITY;
  }
  if(y){
    *y = (y_raw * dev->accel_scale) * GRAVITY;
  }
  if(z){
    *z = (z_raw * dev->accel_scale) * GRAVITY;
  }

  return UPM_SUCCESS;
}

upm_result_t kx122_sensor_software_reset(const kx122_context dev)
{
  assert(dev != NULL);
  if(kx122_set_bit_on(dev,KX122_CNTL2,KX122_CNTL2_SRST) != UPM_SUCCESS){
    return UPM_ERROR_OPERATION_FAILED;
  }

  int counter = 0;
  uint8_t reg_val;

  kx122_read_register(dev,KX122_CNTL2, &reg_val);

  while((reg_val & KX122_CNTL2_SRST_MASK) != 0x00 && counter < MAX_LOOP_COUNT){
    if(kx122_read_register(dev,KX122_CNTL2, &reg_val) != UPM_SUCCESS){
        return UPM_ERROR_OPERATION_FAILED;
    }
    usleep(100000);
    counter++;
  }

  if(counter == MAX_LOOP_COUNT){
    return UPM_ERROR_OPERATION_FAILED;
  }

  kx122_set_default_values(dev);

  return UPM_SUCCESS;
}

upm_result_t kx122_enable_iir(const kx122_context dev)
{
  assert(dev != NULL);
  return kx122_set_bit_off(dev,KX122_ODCNTL,KX122_ODCNTL_IIR_BYPASS_BYPASS);
}

upm_result_t kx122_disable_iir(const kx122_context dev)
{
  assert(dev != NULL);
  return kx122_set_bit_on(dev,KX122_ODCNTL,KX122_ODCNTL_IIR_BYPASS_BYPASS);
}

upm_result_t kx122_self_test(const kx122_context dev)
{
  assert(dev != NULL);
  float x,y,z;

  float x_before_test_avg = 0, x_during_test_avg = 0;
  float y_before_test_avg = 0, y_during_test_avg = 0;
  float z_before_test_avg = 0, z_during_test_avg = 0;

  for (size_t i = 0; i < MAX_LOOP_COUNT; i++) {
    kx122_get_acceleration_data(dev,&x,&y,&z);
    x_before_test_avg += fabs((x / GRAVITY));
    y_before_test_avg += fabs((y / GRAVITY));
    z_before_test_avg += fabs((z / GRAVITY));
    usleep(SELF_TEST_LOOP_WAIT_TIME);
  }

  kx122_write_register(dev,KX122_SELF_TEST,KX122_SELF_TEST_MEMS_TEST_ON);

  for (size_t i = 0; i < MAX_LOOP_COUNT; i++) {
    kx122_get_acceleration_data(dev,&x,&y,&z);
    x_during_test_avg += fabs((x / GRAVITY));
    y_during_test_avg += fabs((y / GRAVITY));
    z_during_test_avg += fabs((z / GRAVITY));
    usleep(SELF_TEST_LOOP_WAIT_TIME);
  }

  kx122_write_register(dev,KX122_SELF_TEST,KX122_SELF_TEST_MEMS_TEST_OFF);

  x_before_test_avg /= MAX_LOOP_COUNT;
  x_during_test_avg /= MAX_LOOP_COUNT;

  y_before_test_avg /= MAX_LOOP_COUNT;
  y_during_test_avg /= MAX_LOOP_COUNT;

  z_before_test_avg /= MAX_LOOP_COUNT;
  z_during_test_avg /= MAX_LOOP_COUNT;

  bool success = true;
  float difference;
  //X-AXIS
  difference = fabs(x_during_test_avg - x_before_test_avg);
  difference = ceilf(difference * 100) / 100;

  if(difference >= SELF_TEST_MIN_XY_DIFFERENCE && difference <= SELF_TEST_MAX_DIFFERENCE){
    printf("X-AXIS OK DIFFERENCE %.02f\n",difference);
  }
  else{
    printf("X-AXIS FAILED, DIFFERENCE %.02f\n", difference);
    success = false;
  }
  //Y-AXIS
  difference = fabs(y_during_test_avg - y_before_test_avg);
  difference = ceilf(difference * 100) / 100;
  if(difference >= SELF_TEST_MIN_XY_DIFFERENCE && difference <= SELF_TEST_MAX_DIFFERENCE){
    printf("Y-AXIS OK DIFFERENCE %.02f\n",difference);
  }
  else{
    printf("Y-AXIS FAILED, DIFFERENCE %.02f\n", difference);
    success = false;
  }
  //Z-AXIS
  difference = fabs(z_during_test_avg - z_before_test_avg);
  difference = ceilf(difference * 100) / 100;
  if(difference >= SELF_TEST_MIN_Z_DIFFERENCE && difference <= SELF_TEST_MAX_DIFFERENCE){
    printf("Z-AXIS OK DIFFERENCE %.02f\n",difference);
  }
  else{
    printf("Z-AXIS FAILED, DIFFERENCE %.02f\n", difference);
    success = false;
  }

  if(success){
    return UPM_SUCCESS;
  }

  return UPM_ERROR_OPERATION_FAILED;
}

upm_result_t kx122_set_odr(const kx122_context dev, KX122_ODR_T odr)
{
  assert(dev != NULL);
  return kx122_set_bit_pattern(dev,KX122_ODCNTL,odr,KX122_ODCNTL_OSA_MASK);
}

upm_result_t kx122_set_grange(const kx122_context dev, KX122_RANGE_T grange)
{
  assert(dev != NULL);
  kx122_map_grange(dev,grange);

  return kx122_set_bit_pattern(dev,KX122_CNTL1,grange,KX122_CNTL1_GSEL_MASK);
}

upm_result_t kx122_set_resolution(const kx122_context dev, KX122_RES_T res)
{
  assert(dev != NULL);

  dev->res_mode = res;
  kx122_map_grange(dev,dev->grange_mode);

  if(res == HIGH_RES){
    return kx122_set_bit_on(dev,KX122_CNTL1,KX122_CNTL1_RES);
  }
  else{
    return kx122_set_bit_off(dev,KX122_CNTL1,KX122_CNTL1_RES);
  }
}

upm_result_t kx122_set_bw(const kx122_context dev, LPRO_STATE_T lpro)
{
  assert(dev != NULL);
  if(lpro == ODR_9){
    return kx122_set_bit_off(dev, KX122_ODCNTL, KX122_ODCNTL_LPRO_ODR_2);
  }
  else{
    return kx122_set_bit_on(dev, KX122_ODCNTL, KX122_ODCNTL_LPRO_ODR_2);
  }
}

upm_result_t kx122_set_average(const kx122_context dev, KX122_AVG_T avg)
{
  assert(dev != NULL);
  return kx122_set_bit_pattern(dev,KX122_LP_CNTL,avg,KX122_LP_CNTL_AVC_MASK);
}

upm_result_t kx122_install_isr(const kx122_context dev, mraa_gpio_edge_t edge,KX122_INTERRUPT_PIN_T intp, int pin, void (*isr)(void *), void *arg)
{
  assert(dev != NULL);
  mraa_gpio_context isr_gpio = NULL;

  if (!(isr_gpio = mraa_gpio_init(pin))){
    printf("%s: mraa_gpio_init() failed.\n", __FUNCTION__);
    return UPM_ERROR_OPERATION_FAILED;
  }

  mraa_gpio_dir(isr_gpio, MRAA_GPIO_IN);

  if (mraa_gpio_isr(isr_gpio, edge, isr, arg)){
    mraa_gpio_close(isr_gpio);
    printf("%s: mraa_gpio_isr() failed.\n", __FUNCTION__);
    return UPM_ERROR_OPERATION_FAILED;
  }

  if(intp == INT1){
    dev->gpio1 = isr_gpio;
  }
  else{
    dev->gpio2 = isr_gpio;
  }

  return UPM_SUCCESS;
}

void kx122_uninstall_isr(const kx122_context dev, KX122_INTERRUPT_PIN_T intp)
{
  assert(dev != NULL);
  if(intp == INT1 && dev->gpio1){
    mraa_gpio_isr_exit(dev->gpio1);
    mraa_gpio_close(dev->gpio1);
    dev->gpio1 = NULL;
  }
  else if(intp == INT2 && dev->gpio2){
    mraa_gpio_isr_exit(dev->gpio2);
    mraa_gpio_close(dev->gpio2);
    dev->gpio2 = NULL;
  }
}

upm_result_t kx122_enable_interrupt1(const kx122_context dev, KX122_INTERRUPT_POLARITY_T polarity)
{
  assert(dev != NULL);

  uint8_t bits = KX122_INC1_PWSEL1_50US_10US | KX122_INC1_IEL1 | KX122_INC1_IEN1;
  if(polarity == ACTIVE_HIGH){
    bits |= KX122_INC1_IEA1;
  }

  return kx122_set_bit_pattern(dev,KX122_INC1,bits,KX122_INC1_MASK);
}

upm_result_t kx122_enable_interrupt2(const kx122_context dev, KX122_INTERRUPT_POLARITY_T polarity)
{
  assert(dev != NULL);

  uint8_t bits = KX122_INC5_PWSEL2_50US_10US | KX122_INC5_IEL2 | KX122_INC5_IEN2;
  if(polarity == ACTIVE_HIGH){
    bits |= KX122_INC5_IEA2;
  }

  return kx122_set_bit_pattern(dev,KX122_INC5,bits,KX122_INC5_MASK);
}

upm_result_t kx122_disable_interrupt1(const kx122_context dev)
{
  assert(dev != NULL);
  return kx122_set_bit_off(dev,KX122_INC1,KX122_INC1_IEN1);
}

upm_result_t kx122_disable_interrupt2(const kx122_context dev)
{
  assert(dev != NULL);
  return kx122_set_bit_off(dev,KX122_INC5,KX122_INC5_IEN2);
}

upm_result_t kx122_route_interrupt1(const kx122_context dev, uint8_t bits)
{
  assert(dev != NULL);
  return kx122_set_bit_pattern(dev,KX122_INC4,bits,KX122_INC4_MASK);
}

upm_result_t kx122_route_interrupt2(const kx122_context dev, uint8_t bits)
{
  assert(dev != NULL);
  return kx122_set_bit_pattern(dev,KX122_INC6,bits,KX122_INC6_MASK);
}

upm_result_t kx122_enable_data_ready_interrupt(const kx122_context dev)
{
  assert(dev != NULL);
  return kx122_set_bit_on(dev,KX122_CNTL1,KX122_CNTL1_DRDYE);
}

upm_result_t kx122_disable_data_ready_interrupt(const kx122_context dev)
{
  assert(dev != NULL);
  return kx122_set_bit_off(dev,KX122_CNTL1,KX122_CNTL1_DRDYE);
}

upm_result_t kx122_enable_buffer_full_interrupt(const kx122_context dev)
{
  assert(dev != NULL);
  return kx122_set_bit_on(dev,KX122_BUF_CNTL2,KX122_BUF_CNTL2_BFIE);
}

upm_result_t kx122_disable_buffer_full_interrupt(const kx122_context dev)
{
  assert(dev != NULL);
  return kx122_set_bit_off(dev,KX122_BUF_CNTL2,KX122_BUF_CNTL2_BFIE);
}

upm_result_t kx122_get_interrupt_source(const kx122_context dev, uint8_t *data)
{
  assert(dev != NULL);
  return kx122_read_register(dev,KX122_INS2,data);
}

upm_result_t kx122_clear_interrupt(const kx122_context dev)
{
  assert(dev != NULL);
  uint8_t reg_val;

  return kx122_read_register(dev,KX122_INT_REL,&reg_val);
}

bool kx122_get_interrupt_status(const kx122_context dev)
{
  assert(dev != NULL);
  uint8_t reg_val;

  if(kx122_read_register(dev,KX122_STATUS_REG,&reg_val) != UPM_SUCCESS){
    printf("%s: kx122_read_register() failed.\n", __FUNCTION__);
    return false;
  }

  if(reg_val != 0x00){
    return true;
  }
  return false;
}

upm_result_t kx122_enable_buffer(const kx122_context dev)
{
  assert(dev != NULL);
  return kx122_set_bit_on(dev,KX122_BUF_CNTL2,KX122_BUF_CNTL2_BUFE);
}

upm_result_t kx122_disable_buffer(const kx122_context dev)
{
  assert(dev != NULL);
  return kx122_set_bit_off(dev,KX122_BUF_CNTL2,KX122_BUF_CNTL2_BUFE);
}

upm_result_t kx122_buffer_init(const kx122_context dev, uint samples, KX122_RES_T res, KX122_BUFFER_MODE_T mode)
{
  assert(dev != NULL);
  kx122_set_sensor_standby(dev);

  if(kx122_set_buffer_threshold(dev,samples) != UPM_SUCCESS){
    return UPM_ERROR_OPERATION_FAILED;
  }
  if(kx122_set_buffer_resolution(dev,res) != UPM_SUCCESS){
    return UPM_ERROR_OPERATION_FAILED;
  }
  if(kx122_set_buffer_mode(dev,mode) != UPM_SUCCESS){
    return UPM_ERROR_OPERATION_FAILED;
  }

  kx122_enable_buffer(dev);
  kx122_set_sensor_active(dev);

  return UPM_SUCCESS;
}

upm_result_t kx122_set_buffer_resolution(const kx122_context dev, KX122_RES_T res)
{
  assert(dev != NULL);
  dev->buffer_res = res;
  kx122_map_grange(dev,dev->grange_mode);

  if(res == HIGH_RES){
    return kx122_set_bit_on(dev,KX122_BUF_CNTL2,KX122_BUF_CNTL2_BRES);
  }
  return kx122_set_bit_off(dev, KX122_BUF_CNTL2, KX122_BUF_CNTL2_BRES);
}

upm_result_t kx122_set_buffer_threshold(const kx122_context dev, uint samples)
{
  assert(dev != NULL);
  if(dev->buffer_res == LOW_RES && samples > MAX_BUFFER_SAMPLES_LOW_RES){
    samples = MAX_BUFFER_SAMPLES_LOW_RES;
  }
  else if(dev->buffer_res == HIGH_RES && samples > MAX_BUFFER_SAMPLES_HIGH_RES){
    samples = MAX_BUFFER_SAMPLES_HIGH_RES;
  }

  uint16_t bits = (uint16_t)samples;

  uint8_t temp = (uint8_t) (bits & 0x00FF);//Get first 8 bits and set them to BUF_CNTL1

  if(kx122_write_register(dev,KX122_BUF_CNTL1,temp) != UPM_SUCCESS){
    return UPM_ERROR_OPERATION_FAILED;
  }

  temp = (uint8_t)((bits & 0x0300) >> 6); //Get last 2 bits and set them to BUF_CNTL2

  return kx122_set_bit_pattern(dev,KX122_BUF_CNTL2,temp,KX122_BUF_CNTL2_SMP_TH8_9_MASK);
}

upm_result_t kx122_set_buffer_mode(const kx122_context dev, KX122_BUFFER_MODE_T mode)
{
  assert(dev != NULL);
  dev->buffer_mode = mode;

  return kx122_set_bit_pattern(dev,KX122_BUF_CNTL2,mode,KX122_BUF_CNTL2_BUF_M_MASK);
}

upm_result_t kx122_clear_buffer(const kx122_context dev)
{
  assert(dev != NULL);
  return kx122_write_register(dev,KX122_BUF_CLEAR,0xFF); //Writing anything to the register clears the buffer
}

upm_result_t kx122_get_buffer_status(const kx122_context dev, uint *samples)
{
  uint8_t reg_val;
  uint16_t temp; //Using only 10 bits

  if(kx122_read_register(dev,KX122_BUF_STATUS_2,&reg_val) != UPM_SUCCESS){
    return UPM_ERROR_OPERATION_FAILED;
  }

  temp = (reg_val & 0x0007) << 8; //Get 3 MSb from BUF_STATUS 2

  if(kx122_read_register(dev,KX122_BUF_STATUS_1, &reg_val) != UPM_SUCCESS){
    return UPM_ERROR_OPERATION_FAILED;
  }

  temp = (temp + (reg_val & 0x00FF)); //Get rest of the bits from BUF_STATUS 1

  //Get the amount of samples
  if(dev->buffer_res == LOW_RES){
    //3 axis / sample
    *samples = ((uint)(int16_t)temp) / 3;
  }
  else{
    //3 axis * MSB/LSB / sample
    *samples = ((uint)(int16_t)temp) / 6;
  }

  return UPM_SUCCESS;
}

upm_result_t kx122_read_buffer_samples_raw(const kx122_context dev, uint len, float *x_array, float *y_array, float *z_array)
{
  assert(dev != NULL);
  if(dev->buffer_res == LOW_RES){
    len *= LOW_RES_SAMPLE_MODIFIER; //3 axis / sample
  }
  else{
    len *= HIGH_RES_SAMPLE_MODIFIER; //3 axis * 2 bytes / sample
  }

  uint8_t buffer[len];

  if(kx122_read_registers(dev,KX122_BUF_READ,buffer,len) != UPM_SUCCESS){
    return UPM_ERROR_OPERATION_FAILED;
  }

  if(dev->buffer_res == HIGH_RES){
    for (size_t i = 0; i < len; i+= HIGH_RES_SAMPLE_MODIFIER) {
      if(dev->buffer_mode != KX122_FILO_MODE){
        if(x_array){
          x_array[i / HIGH_RES_SAMPLE_MODIFIER] = (float)((int16_t) (buffer[i + 1] << 8) | buffer[i]);
        }
        if(y_array){
          y_array[i / HIGH_RES_SAMPLE_MODIFIER] = (float)((int16_t) (buffer[i + 3] << 8) | buffer[i + 2]);
        }
        if(z_array){
          z_array[i / HIGH_RES_SAMPLE_MODIFIER] = (float)((int16_t) (buffer[i + 5] << 8) | buffer[i + 4]);
        }
      }
      else{
        if(x_array){
          x_array[i / HIGH_RES_SAMPLE_MODIFIER] = (float)((int16_t) (buffer[i + 4] << 8) | buffer[i + 5]);
        }
        if(y_array){
          y_array[i / HIGH_RES_SAMPLE_MODIFIER] = (float)((int16_t) (buffer[i + 2] << 8) | buffer[i + 3]);
        }
        if(z_array){
          z_array[i / HIGH_RES_SAMPLE_MODIFIER] = (float)((int16_t) (buffer[i] << 8) | buffer[i + 1]);
        }
      }
    }
  }
  else{ //Low resolution
    for (size_t i = 0; i < len; i+= LOW_RES_SAMPLE_MODIFIER) {
      if(dev->buffer_mode != KX122_FILO_MODE){
        if(x_array){
          x_array[i / LOW_RES_SAMPLE_MODIFIER] = (float)(int8_t)buffer[i];
        }
        if(y_array){
          y_array[i / LOW_RES_SAMPLE_MODIFIER] = (float)(int8_t)buffer[i + 1];
        }
        if(z_array){
          z_array[i / LOW_RES_SAMPLE_MODIFIER] = (float)(int8_t)buffer[i + 2];
        }
      }
      else{
        if(x_array){
          x_array[i / LOW_RES_SAMPLE_MODIFIER] = (float)(int8_t)buffer[i + 2];
        }
        if(y_array){
          y_array[i / LOW_RES_SAMPLE_MODIFIER] = (float)(int8_t)buffer[i + 1];
        }
        if(z_array){
          z_array[i / LOW_RES_SAMPLE_MODIFIER] = (float)(int8_t)buffer[i];
        }
      }
    }
  }

  return UPM_SUCCESS;
}

upm_result_t kx122_read_buffer_samples(const kx122_context dev, uint len, float *x_array, float *y_array, float *z_array)
{
  assert(dev != NULL);
  if(kx122_read_buffer_samples_raw(dev,len,x_array,y_array,z_array) != UPM_SUCCESS){
    return UPM_ERROR_OPERATION_FAILED;
  }

  for (size_t i = 0; i < len; i++) {
    if(x_array){
      x_array[i] = (x_array[i] * dev->buffer_accel_scale) * GRAVITY;
    }
    if(y_array){
      y_array[i] = (y_array[i] * dev->buffer_accel_scale) * GRAVITY;
    }
    if(z_array){
      z_array[i] = (z_array[i] * dev->buffer_accel_scale) * GRAVITY;
    }
  }

  return UPM_SUCCESS;
}
