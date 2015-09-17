%module (directors=1, docstring="TTP223 Touch Sensor") javaupm_ttp223

%include "../upm.i"

%feature("director") IsrCallback;

%ignore generic_callback_isr;
%include "../IsrCallback.h"

%apply int {mraa::Edge}


%{
    #include "ttp223.h"
%}

%include "ttp223.h"
