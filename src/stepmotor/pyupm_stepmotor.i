%module pyupm_stepmotor

%include "stdint.i"

%feature("autodoc", "3");

%include "stepmotor.h"
%{
    #include "stepmotor.h"
%}
