%module javaupm_mpu9150
%include "../upm.i"
%include "typemaps.i"
%include "arrays_java.i"

%feature("director") IsrCallback;

%ignore generic_callback_isr;
%include "../IsrCallback.h"

%apply int {mraa::Edge};
%apply float *INOUT { float *x, float *y, float *z  };
%apply unsigned char[] {uint8_t *};

%{
    #include "mpu60x0.h"
%}

%include "mpu60x0.h"


%{
    #include "mpu9150.h"
%}

%include "mpu9150.h"
