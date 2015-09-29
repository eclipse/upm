%module(directors="1") javaupm_a110x
%include "../upm.i"
%include "stdint.i"
%include "typemaps.i"

%feature("director") IsrCallback;

%ignore generic_callback_isr;
%include "../IsrCallback.h"

%{
    #include "a110x.h"
%}
%include "a110x.h"
