%module pyupm_mic
%include "../upm.i"

%include "stdint.i"

%feature("autodoc", "3");

%include "mic.h"
%{
    #include "mic.h"
%}
