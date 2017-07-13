// Include doxygen-generated documentation
%include "pyupm_doxy2swig.i"
%module pyupm_grovevdiv
%include "../upm.i"

%include "grovevdiv.hpp"
%{
    #include "grovevdiv.hpp"
%}
