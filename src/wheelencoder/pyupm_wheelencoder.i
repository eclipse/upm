%module pyupm_wheelencoder
%include "../upm.i"

%feature("autodoc", "3");

%include "wheelencoder.h"
%{
    #include "wheelencoder.h"
%}
