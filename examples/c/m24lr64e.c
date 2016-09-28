#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include "m24lr64e.h"

#include "upm_utilities.h"

int main()
{
    m24lr64e_context dev = m24lr64e_init(0, M24LR64E_USER_MODE);
    int addr = M24LR64E_EEPROM_I2C_LENGTH-1;
    printf("address being accessed: %d\n", addr);
    uint8_t byte; 
    if(m24lr64e_read_byte(dev, addr, &byte) != UPM_SUCCESS)
        printf("error while reading value\n");
    printf("value read from the device: %d\n", byte);
    byte = ~byte;
    printf("byte to be written: %d\n", byte);
    if(m24lr64e_write_byte(dev, addr, byte) != UPM_SUCCESS)
        printf("error while writing byte to the device\n");
    uint8_t var;
    if(m24lr64e_read_byte(dev, addr, &var) != UPM_SUCCESS)
        printf("error while reading value back\n");
    printf("new value at %d: %d\n", addr, var);
    m24lr64e_close(dev);
    printf("all done!!\n");

    return 0;
}
