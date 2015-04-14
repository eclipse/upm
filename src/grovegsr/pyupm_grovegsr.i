%module pyupm_grovegsr
%include "../upm.i"

%feature("autodoc", "3");

%include "grovegsr.h"
%{
    #include "grovegsr.h"
%}
