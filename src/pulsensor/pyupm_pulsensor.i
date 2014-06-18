%module pyupm_pulsensor

%include "stdint.i"

%feature("autodoc", "3");

%include "pulsensor.h"
%{
    #include "pulsensor.h"
%}
