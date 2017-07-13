// Include doxygen-generated documentation
%include "pyupm_doxy2swig.i"
%module pyupm_hwxpxx
%include "../upm.i"
%include "stdint.i"

%include "hwxpxx.hpp"
%{
    #include "hwxpxx.hpp"
%}
