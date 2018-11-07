//Modified: Abhishek Malik <abhishek.malik@intel.com>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include "mpr121.h"

#include "upm_utilities.h"

int main()
{
    mpr121_context dev = mpr121_init(MPR121_I2C_BUS, MPR121_DEFAULT_I2C_ADDR);

    if(mpr121_config_an3944(dev) != UPM_SUCCESS){
        printf("unable to configure device\n");
    }
    uint32_t states;
    while(1){
        if(mpr121_read_buttons(dev, &states, 0) != UPM_SUCCESS){
            printf("Error while reading button values\n");
        }
        printf("retrieved button states: %d\n", states);
        upm_delay(1);
    }

    mpr121_close(dev);
    printf("all done!!\n");

    return 0;
}
