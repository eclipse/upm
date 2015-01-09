%module pyupm_groveloudness
%include "../upm.i"

%feature("autodoc", "3");

%include "groveloudness.h"
%{
    #include "groveloudness.h"
%}
