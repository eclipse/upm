// Include doxygen-generated documentation
%include "pyupm_doxy2swig.i"
%module pyupm_hcsr04
%include "../upm.i"

%feature("autodoc", "3");

%include "hcsr04.hpp"
%{
    #include "hcsr04.hpp"
%}
