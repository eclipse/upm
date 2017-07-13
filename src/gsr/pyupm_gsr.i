// Include doxygen-generated documentation
%include "pyupm_doxy2swig.i"
%module pyupm_gsr
%include "../upm.i"

%include "gsr.hpp"
%{
    #include "gsr.hpp"
%}
