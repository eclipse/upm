%module pyupm_max31855
%include "../upm.i"

%include "stdint.i"

%feature("autodoc", "3");

%include "max31855.h"
%{
    #include "max31855.h"
%}
