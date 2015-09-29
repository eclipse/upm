%module(directors="1") javaupm_hcsr04
%include "../upm.i"

%feature("director") IsrCallback;

%ignore generic_callback_isr;
%include "../IsrCallback.h"

%{
    #include "hcsr04.h"
%}

%include "hcsr04.h"
