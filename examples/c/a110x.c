//Modified: Abhishek Malik <abhishek.malik@intel.com>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include "a110x.h"

#include "upm_utilities.h"

int main()
{
    a110x_context dev = a110x_init(2);
    bool abc = 0;
    while(1){
        if(a110x_magnet_detected(dev, &abc) != UPM_SUCCESS){
            printf("an error has occured\n");
        }
        upm_delay(1);
        printf("value retrieved: %d\n", abc);
    }

    return 0;
}
