%module javaupm_apds9930
%include "../upm.i"
%include "stdint.i"
%include "typemaps.i"

%feature("director") IsrCallback;

%ignore generic_callback_isr;
%include "../IsrCallback.h"

%{
    #include "apds9930.h"
%}
%include "apds9930.h"
