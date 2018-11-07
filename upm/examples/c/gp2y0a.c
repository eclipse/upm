//Modified: Abhishek Malik <abhishek.malik@intel.com>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include "gp2y0a.h"

#include "upm_utilities.h"

int main()
{
    gp2y0a_context dev = gp2y0a_init(14, 5.0);
    float val;
    while(1){
        if(gp2y0a_get_value(dev, 5.0, 20, &val) != UPM_SUCCESS){
            printf("Failed to get any values from the sensor\n");
        }
        printf("Moisture Value: %f\n", val);
        upm_delay(1);
    }
    gp2y0a_close(dev);

    return 0;
}

