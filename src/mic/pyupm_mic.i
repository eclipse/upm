%module pyupm_mic

%include "stdint.i"

%feature("autodoc", "3");

%include "mic.h"
%{
    #include "mic.h"
%}
