%module pyupm_groveo2
%include "../upm.i"

%feature("autodoc", "3");

%include "groveo2.h"
%{
    #include "groveo2.h"
%}
