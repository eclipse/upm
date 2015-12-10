#include <mraa/i2c.hpp>
#include "upm/iPressureSensor.h"
#include "upm/iTemperatureSensor.h"
#include "upm/iHumiditySensor.h"
#ifndef __BME280_H__
#define __BME280_H__


//#define BME280_MDELAY_DATA_TYPE uint32_t 

/****************************************************/
/**\name        I2C ADDRESS DEFINITIONS  */
/***************************************************/
#define BME280_I2C_ADDRESS1                  (0x76)
#define BME280_I2C_ADDRESS2                  (0x77)
#define BME280_ONE_U8X   (1)

namespace upm {



class BME280 : public ITemperatureSensor, public IHumiditySensor, public IPressureSensor {
    public:



        /**
         * Instanciates a BME280 object
         *
         * @param bus number of used bus
         * @param devAddr address of used i2c device
         * @param mode BME280 mode
         */
	BME280();
        //BME280 (int bus, int devAddr= 0x77 , uint8_t mode = 0x03 );
	BME280 (int bus, int devAddr = BME280_I2C_ADDRESS1);

        /**
         * BME280 object destructor, basicaly it close i2c connection.
         */
        ~BME280 ();

        /**
         * Get temperature measurement.
         */
        uint16_t getTemperatureRaw ();

        /**
         * Get temperature measurement.
         */
        int getTemperatureCelcius ();

        /**
         * Get relative humidity measurement.
         */
        uint16_t getHumidityRaw ();

        /**
         * Get relative humidity measurement.
         */
        int getHumidityRelative ();

        /**
         * Returns sensor module name
         */
        const char* getModuleName() { return "BME280"; }

        /**
         * Return pressure
         */
        uint32_t getPressureRaw();

        /**
         * Return calculated pressure (Pa)
         */ 
        int getPressurePa();


        /**
         * Return temperature
         */
	int32_t getTemperatureInternal(void);

         /**
         * Return pressure
         */
	int32_t getPressureInternal(void);
	 /**
         * Return  humidity
         */
	int32_t getHumidityInternal(void);


	  /**
         * Return temperature
         */
	int32_t getTemperatureRawInternal(void);

         /**
         * Return pressure
         */
	int32_t getPressureRawInternal(void);
	 /**
         * Return  humidity
         */
	int32_t getHumidityRawInternal(void);
        
       
        
              
        //int32_t getTemperatureRaw(void);

        /**
         * Returns whether the sensor is configured.
         */
        bool isConfigured();

        /**
         * Returns whether the correct chip is present at the given address.
         */
        bool isAvailable();


    private:
        /**************************************************************/
        /**\name	STRUCTURE DEFINITIONS                         */
        /**************************************************************/
        /*!
         * @brief This structure holds all device specific calibration parameters
         */

        static mraa::I2c* m_i2c;
        static int m_bus;

        static int32_t i2c_write_string(uint8_t dev_addr,uint8_t*  ptr, uint8_t cnt);
        static int32_t i2c_write_read_string(uint8_t dev_addr,uint8_t reg_addr , uint8_t * ptr, uint8_t cnt);
        static int8_t I2C_routine();
	static int8_t BME280_I2C_bus_write(uint8_t dev_addr, uint8_t reg_addr, uint8_t *reg_data, uint8_t cnt);
	static void BME280_delay_msek(uint16_t msek);
	static int8_t BME280_I2C_bus_read(uint8_t dev_addr, uint8_t reg_addr, uint8_t *reg_data, uint8_t cnt);

};







}

#endif
