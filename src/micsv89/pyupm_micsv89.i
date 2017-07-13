// Include doxygen-generated documentation
%include "pyupm_doxy2swig.i"
%module pyupm_micsv89
%include "../upm.i"

%include "micsv89.hpp"
%{
    #include "micsv89.hpp"
%}
