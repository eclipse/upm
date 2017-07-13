// Include doxygen-generated documentation
%include "pyupm_doxy2swig.i"
%module pyupm_mcp9808
%include "../upm.i"

%include "mcp9808.hpp"
%{
    #include "mcp9808.hpp"
%}
