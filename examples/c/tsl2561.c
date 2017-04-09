//Modified: Abhishek Malik <abhishek.malik@intel.com>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include "tsl2561.h"

#include "upm_utilities.h"

int main()
{
    tsl2561_context dev = tsl2561_init(0, TSL2561_Address, GAIN_0X, INTEGRATION_TIME1_101MS);
    float abc = 0;
    if(tsl2561_get_lux(dev, &abc) != UPM_SUCCESS){
        printf("ERROR !! ERROR !! ERROR!!");
    }
    printf("value retrieved: %f\n", abc);

    return 0;
}
