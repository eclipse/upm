// Include doxygen-generated documentation
%include "pyupm_doxy2swig.i"
%module pyupm_isd1820
%include "../upm.i"

%{
    #include "isd1820.hpp"
%}

%include "isd1820.hpp"

