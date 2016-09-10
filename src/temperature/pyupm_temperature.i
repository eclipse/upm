// Include doxygen-generated documentation
%include "pyupm_doxy2swig.i"
%module pyupm_temperature
%include "../upm.i"

%include "temperature.hpp"
%{
    #include "temperature.hpp"
%}
