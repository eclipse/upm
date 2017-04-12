%module pyupm_t6713
%include "../upm.i"
// Include doxygen-generated documentation
%include "pyupm_doxy2swig.i"

%include "stdint.i"

%feature("autodoc", "3");

%include "t6713.hpp"
%{
    #include "t6713.hpp"
%}
