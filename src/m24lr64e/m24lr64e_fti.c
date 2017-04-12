#include "m24lr64e.h"
#include "upm_fti.h"

/** 
 * This file implements the Function Table Interface (FTI) for this sensor
 */

const char upm_m24lr64e_name[] = "M24LR64E";
const char upm_m24lr64e_description[] = "Grove NFC Tag";
const upm_protocol_t upm_m24lr64e_protocol[] = {UPM_I2C};
const upm_sensor_t upm_m24lr64e_category[] = {UPM_NFC};

// forward declarations
const void* upm_m24lr64e_get_ft(upm_sensor_t sensor_type);
void* upm_m24lr64e_init_name();
void upm_m24lr64e_close(void* dev);

const upm_sensor_descriptor_t upm_m24lr64e_get_descriptor(){
    upm_sensor_descriptor_t usd;
    usd.name = upm_m24lr64e_name;
    usd.description = upm_m24lr64e_description;
    usd.protocol_size = 1;
    usd.protocol = upm_m24lr64e_protocol;
    usd.category_size = 1;
    usd.category = upm_m24lr64e_category;
    return usd;
}

static const upm_sensor_ft ft =
{
    .upm_sensor_init_name = &upm_m24lr64e_init_name,
    .upm_sensor_close = &upm_m24lr64e_close,
    .upm_sensor_get_descriptor = &upm_m24lr64e_get_descriptor
};

const void* upm_m24lr64e_get_ft(upm_sensor_t sensor_type){
    if(sensor_type == UPM_SENSOR){
        return &ft;
    }
    return NULL;
}

void* upm_m24lr64e_init_name(){
    return NULL;
}

void upm_m24lr64e_close(void* dev){
    m24lr64e_close((m24lr64e_context)dev);
}
