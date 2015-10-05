%module pyupm_lm35
%include "../upm.i"

%feature("autodoc", "3");

%include "lm35.h"
%{
    #include "lm35.h"
%}
