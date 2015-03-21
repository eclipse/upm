%module pyupm_groveeldriver
%include "../upm.i"

%feature("autodoc", "3");

%include "groveeldriver.h"
%{
    #include "groveeldriver.h"
%}
