//Modified: Abhishek Malik <abhishek.malik@intel.com>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include "grovemoisture.h"

#include "upm_utilities.h"

int main()
{
    grovemoisture_context dev = grovemoisture_init(14);
    int val;
    while(1){
        if(grovemoisture_get_moisture(dev, &val) != UPM_SUCCESS){
            printf("Failed to get any values from the sensor\n");
        }
        printf("Moisture Value: %d\n", val);
        upm_delay(1);
    }
    grovemoisture_close(dev);

    return 0;
}

