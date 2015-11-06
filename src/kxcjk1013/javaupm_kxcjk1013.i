%module javaupm_kxcjk1013
%include "../upm.i"
%include "stdint.i"
%include "typemaps.i"

%feature("director") IsrCallback;

%ignore generic_callback_isr;
%include "../IsrCallback.h"

%{
    #include "kxcjk1013.h"
%}
%include "kxcjk1013.h"
