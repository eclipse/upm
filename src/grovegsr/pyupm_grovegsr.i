// Include doxygen-generated documentation
%include "pyupm_doxy2swig.i"
%module pyupm_grovegsr
%include "../upm.i"

%include "grovegsr.hpp"
%{
    #include "grovegsr.hpp"
%}
