// Include doxygen-generated documentation
%include "pyupm_doxy2swig.i"
%module pyupm_mpr121
%include "../upm.i"

%include "mpr121.hpp"
%{
    #include "mpr121.hpp"
%}
