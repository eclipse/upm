/*
 * Author: Abhishek Malik <abhishek.malik@intel.com>
 * Copyright (c) 2017 Intel Corporation.
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
 * LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
 * OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
 * WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */
 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include "abpdrrt005pg2a5.h"

#include "upm_utilities.h"

int main()
{
    abpdrrt005pg2a5_context dev = abpdrrt005pg2a5_init(0, ABPDRRT005PG2A5_ADDRESS);
    if(dev == NULL) {
        printf("Unable to initialize sensor\n");
        return 0;
    }
    float psi = 0;
    float pascal = 0;
    while(1){
        if(abpdrrt005pg2a5_get_pressure_psi(dev, &psi) != UPM_SUCCESS){
            printf("error in retrieving the psi value\n");
        }
        if(abpdrrt005pg2a5_get_pressure_pascal(dev, &pascal) != UPM_SUCCESS){
            printf("error in retrieving the pascal value\n");
        }
        upm_delay(1);
        printf("PSI Value: %f and Pascal Value: %f\n", psi, pascal);
    }

    return 0;
}