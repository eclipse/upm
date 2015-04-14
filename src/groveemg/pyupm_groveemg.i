%module pyupm_groveemg
%include "../upm.i"

%feature("autodoc", "3");

%include "groveemg.h"
%{
    #include "groveemg.h"
%}
