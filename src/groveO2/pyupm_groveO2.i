%module pyupm_groveO2
%include "../upm.i"

%feature("autodoc", "3");

%include "groveO2.h"
%{
    #include "groveO2.h"
%}
