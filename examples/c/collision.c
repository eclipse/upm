//Modified: Abhishek Malik <abhishek.malik@intel.com>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include "collision.h"

#include "upm_utilities.h"

int main()
{
    collision_context dev = collision_init(2);
    bool abc = 0;
    while(1){
        if(collision_is_colliding(dev, &abc) != UPM_SUCCESS){
            printf("an error has occured\n");
        }
        upm_delay(1);
        printf("value retrieved: %d\n", abc);
    }
    collision_close(dev);
}
