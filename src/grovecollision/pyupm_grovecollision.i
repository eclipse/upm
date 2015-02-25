%module pyupm_grovecollision
%include "../upm.i"

%feature("autodoc", "3");

%include "grovecollision.h"
%{
    #include "grovecollision.h"
%}
