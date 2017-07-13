// Include doxygen-generated documentation
%include "pyupm_doxy2swig.i"
%module pyupm_rotary
%include "../upm.i"

%include "rotary.hpp"
%{
    #include "rotary.hpp"
%}
