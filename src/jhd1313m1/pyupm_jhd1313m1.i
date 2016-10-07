// Include doxygen-generated documentation
%include "pyupm_doxy2swig.i"
%module pyupm_i2clcd
%include "../upm.i"
%include "../carrays_uint8_t.i"

%feature("autodoc", "3");

%include "jhd1313m1.hpp"
%{
    #include "jhd1313m1.hpp"
%}
