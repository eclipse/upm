%module pyupm_max31723
%include "../upm.i"

%feature("autodoc", "3");

%include "max31723.h"
%{
    #include "max31723.h"
%}
