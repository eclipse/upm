%module pyupm_loudness
%include "../upm.i"

%feature("autodoc", "3");

%include "loudness.h"
%{
    #include "loudness.h"
%}
