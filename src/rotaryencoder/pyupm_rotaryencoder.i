%module pyupm_rotaryencoder
%include "../upm.i"

%feature("autodoc", "3");

%include "rotaryencoder.h"
%{
    #include "rotaryencoder.h"
%}
