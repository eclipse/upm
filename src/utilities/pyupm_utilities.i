// Include doxygen-generated documentation
%include "pyupm_doxy2swig.i"
%module pyupm_utilities
%include "../upm.i"

%feature("autodoc", "3");

%include "upm_utilities.h"
%{
    #include "upm_utilities.h"
%}
