// Include doxygen-generated documentation
%include "pyupm_doxy2swig.i"
%module pyupm_apds9002
%include "../upm.i"

%include "apds9002.hpp"
%{
    #include "apds9002.hpp"
%}
