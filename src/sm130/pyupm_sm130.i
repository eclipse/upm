// Include doxygen-generated documentation
%include "pyupm_doxy2swig.i"
%module pyupm_sm130
%include "../upm.i"

%feature("autodoc", "3");

%include "sm130.h"
%{
    #include "sm130.h"
%}
