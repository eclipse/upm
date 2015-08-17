%module javaupm_pulsensor
%include "../upm.i"
%include "arrays_java.i"

%ignore sample_thread;
%ignore pin_ctx;
%ignore do_sample;
%ignore callback; 

%{
    #include "pulsensor.h"
%}

%include "pulsensor.h"
