%module pyupm_step_motor

%include "stdint.i"

%feature("autodoc", "3");

%include "step_motor.h"
%{
    #include "step_motor.h"
%}
