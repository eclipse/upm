// Include doxygen-generated documentation
%include "pyupm_doxy2swig.i"
%module pyupm_i2clcd
%include "../upm.i"
%include "../carrays_uint8_t.i"

%feature("autodoc", "3");

%include "lcm1602.hpp"
%{
    #include "lcm1602.hpp"
%}
