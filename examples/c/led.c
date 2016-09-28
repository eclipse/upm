//Modified: Abhishek Malik <abhishek.malik@intel.com>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include "led.h"

#include "upm_utilities.h"

int main(void)
{
    led_context dev = led_init(2);
    while(1){
        if(led_on(dev) != UPM_SUCCESS){
            printf("problem turning the LED on\n");
        }
        upm_delay(1);
        if(led_off(dev) != UPM_SUCCESS){
            printf("problem turning the LED off\n");
        }
        upm_delay(1);
    }
    led_close(dev);
    return 0;
}
