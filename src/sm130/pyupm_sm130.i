// Include doxygen-generated documentation
%include "pyupm_doxy2swig.i"
%module pyupm_sm130
%include "../upm.i"

%include "sm130.hpp"
%{
    #include "sm130.hpp"
%}
