%module pyupm_max44009
%include "../upm.i"

%include "stdint.i"

%feature("autodoc", "3");

%include "max44009.h"
%{
    #include "max44009.h"
%}
