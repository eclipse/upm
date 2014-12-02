%module pyupm_grovemoisture
%include "../upm.i"

%feature("autodoc", "3");

%include "grovemoisture.h"
%{
    #include "grovemoisture.h"
%}
