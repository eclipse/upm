%module pyupm_groveultrasonic
%include "../upm.i"

%feature("autodoc", "3");

%include "groveultrasonic.h"
%{
    #include "groveultrasonic.h"
%}
