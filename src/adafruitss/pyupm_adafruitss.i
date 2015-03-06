%module pyupm_adafruitss
%include "../upm.i"

%feature("autodoc", "3");

%include "adafruitss.h"
%{
    #include "adafruitss.h"
%}
