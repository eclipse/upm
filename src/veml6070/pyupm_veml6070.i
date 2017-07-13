// Include doxygen-generated documentation
%include "pyupm_doxy2swig.i"
%module pyupm_veml6070
%include "../upm.i"

%include "veml6070.hpp"
%{
    #include "veml6070.hpp"
%}
