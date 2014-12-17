%module pyupm_grovetouch
%include "../upm.i"

%feature("autodoc", "3");

%include "grovetouch.h"
%{
    #include "grovetouch.h"
%}
