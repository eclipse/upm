%module(directors="1") javaupm_rpr220
%include "../upm.i"


%feature("director") IsrCallback;

%ignore generic_callback_isr;
%include "../IsrCallback.h"

%{
    #include "rpr220.h"
%}
%include "rpr220.h"
