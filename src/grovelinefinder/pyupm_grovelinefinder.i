%module pyupm_grovelinefinder
%include "../upm.i"

%feature("autodoc", "3");

%include "grovelinefinder.h"
%{
    #include "grovelinefinder.h"
%}
