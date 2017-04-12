//Modified: Abhishek Malik <abhishek.malik@intel.com>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include "mq303a.h"

#include "upm_utilities.h"

int main() {
    /* --------- MQ303A EXAMPLE -------- */
    mq303a_context dev = mq303a_init(0, 15);
    printf("init done for mq303a\n");
    int value;
    mq303a_heater_enable(dev, true);
    upm_delay(12);
    while(1){
        mq303a_get_value(dev, &value);
        printf("returned value: %d\n", value);
        upm_delay(1);
    }

    return 0;
}
