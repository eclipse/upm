// Include doxygen-generated documentation
%include "pyupm_doxy2swig.i"
%module pyupm_grovewater
%include "../upm.i"

%feature("autodoc", "3");

%include "grovewater.h"
%{
    #include "grovewater.h"
%}
