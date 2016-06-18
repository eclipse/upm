// Include doxygen-generated documentation
%include "pyupm_doxy2swig.i"
%module pyupm_adis16448
%include "../upm.i"

%{
    #include "adis16448.hpp"
%}

%include "adis16448.hpp"
