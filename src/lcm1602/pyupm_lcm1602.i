// Include doxygen-generated documentation
%include "pyupm_doxy2swig.i"
%module pyupm_lcm1602
%include "../upm.i"
%include "../carrays_uint8_t.i"

%feature("autodoc", "3");

%include "lcm1602.h"
%include "lcm1602.hpp"
%{
    #include "lcm1602.hpp"
%}
