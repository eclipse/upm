%module pyupm_max5487
%include "../upm.i"

%feature("autodoc", "3");

%include "max5487.h"
%{
    #include "max5487.h"
%}
