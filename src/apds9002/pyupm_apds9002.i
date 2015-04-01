%module pyupm_apds9002
%include "../upm.i"

%feature("autodoc", "3");

%include "apds9002.h"
%{
    #include "apds9002.h"
%}
