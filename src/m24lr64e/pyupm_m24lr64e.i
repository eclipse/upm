// Include doxygen-generated documentation
%include "pyupm_doxy2swig.i"
%module pyupm_m24lr64e
%include "../upm.i"

%include "stdint.i"

%include "m24lr64e.hpp"
%{
    #include "m24lr64e.hpp"
%}
