// Include doxygen-generated documentation
%include "pyupm_doxy2swig.i"
%module pyupm_st7735
%include "../upm.i"

%feature("autodoc", "3");

%include "st7735.h"
%{
    #include "st7735.h"
%}
