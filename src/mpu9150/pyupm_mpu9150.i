%module pyupm_mpu9150
%include "../upm.i"
%include "cpointer.i"

%include "stdint.i"

%feature("autodoc", "3");

%pointer_functions(float, floatp);

%include "ak8975.h"
%{
    #include "ak8975.h"
%}

%include "mpu60x0.h"
%{
    #include "mpu60x0.h"
%}

%include "mpu9150.h"
%{
    #include "mpu9150.h"
%}

%include "mpu9250.h"
%{
    #include "mpu9250.h"
%}

