%module pyupm_mpu9150

%include "stdint.i"

%feature("autodoc", "3");

%include "mpu9150.h"
%{
    #include "mpu9150.h"
%}
