// Include doxygen-generated documentation
%include "pyupm_doxy2swig.i"
%module pyupm_isd1820
%include "../upm.i"

%feature("autodoc", "3");

%{
    #include "isd1820.hpp"
%}

%include "isd1820.hpp"

