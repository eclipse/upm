// Include doxygen-generated documentation
%include "pyupm_doxy2swig.i"
%module pyupm_rf22
%include "../upm.i"
%include "../carrays_uint8_t.i"

%include "rf22.hpp"
%{
    #include "rf22.hpp"
%}
