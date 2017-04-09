// Include doxygen-generated documentation
%include "pyupm_doxy2swig.i"
%module pyupm_lcm1602
%include "../upm.i"
%include "../upm_vectortypes.i"

%feature("autodoc", "3");

%include "lcm1602.hpp"
%{
    #include "lcm1602.hpp"
%}
