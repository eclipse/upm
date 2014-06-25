%module pyupm_servo
%include "../upm.i"

%feature("autodoc", "3");

%include "es08a.h"
%{
    #include "es08a.h"
%}
