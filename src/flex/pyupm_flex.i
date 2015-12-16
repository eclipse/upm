// Include doxygen-generated documentation
%include "pyupm_doxy2swig.i"
%module pyupm_flex
%include "../upm.i"

%feature("autodoc", "3");

%include "flex.h"
%{
    #include "flex.h"
%}
