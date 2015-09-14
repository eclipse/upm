%module jsupm_mpu9150
%include "../upm.i"
%include "cpointer.i"

%pointer_functions(float, floatp);

%{
    #include "mpu9150.h"
%}

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

