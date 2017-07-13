// Include doxygen-generated documentation
%include "pyupm_doxy2swig.i"
%module pyupm_moisture
%include "../upm.i"

%include "moisture.hpp"
%{
    #include "moisture.hpp"
%}