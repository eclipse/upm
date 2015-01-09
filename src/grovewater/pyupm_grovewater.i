%module pyupm_grovewater
%include "../upm.i"

%feature("autodoc", "3");

%include "grovewater.h"
%{
    #include "grovewater.h"
%}
