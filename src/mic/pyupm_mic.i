// Include doxygen-generated documentation
%include "pyupm_doxy2swig.i"
%module pyupm_mic
%include "../upm.i"
%include "../carrays_uint16_t.i"

%{
    #include "mic.hpp"
%}
%include "mic.hpp"
