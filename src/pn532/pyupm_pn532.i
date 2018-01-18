// Include doxygen-generated documentation
%include "pyupm_doxy2swig.i"
%module pyupm_pn532
%include "../upm.i"
%include "../carrays_uint8_t.i"

%include "pn532.hpp"
%{
    #include "pn532.hpp"
%}
