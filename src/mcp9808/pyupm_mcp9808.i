// Include doxygen-generated documentation
%include "pyupm_doxy2swig.i"
%module pyupm_mcp9808
%include "../upm.i"

%feature("autodoc", "3");

%include "mcp9808.h"
%{
    #include "mcp9808.h"
%}
