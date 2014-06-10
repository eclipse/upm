%module pyupm_max44000

%include "stdint.i"

%feature("autodoc", "3");

%include "max44000.h"
%{
    #include "max44000.h"
%}
