// Include doxygen-generated documentation
%include "pyupm_doxy2swig.i"
%module pyupm_groveelectromagnet
%include "../upm.i"

%feature("autodoc", "3");

%include "groveelectromagnet.h"
%{
    #include "groveelectromagnet.h"
%}
