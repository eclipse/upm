%module pyupm_hcsr04
%include "../upm.i"

%feature("autodoc", "3");

%include "hcsr04.h"
%{
    #include "hcsr04.h"
%}
