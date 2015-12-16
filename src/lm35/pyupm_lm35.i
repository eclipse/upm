// Include doxygen-generated documentation
%include "pyupm_doxy2swig.i"
%module pyupm_lm35
%include "../upm.i"

%feature("autodoc", "3");

%include "lm35.h"
%{
    #include "lm35.h"
%}
