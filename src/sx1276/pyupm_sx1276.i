// Include doxygen-generated documentation
%include "pyupm_doxy2swig.i"
%module pyupm_sx1276
%include "../upm.i"
%include "cpointer.i"

%include "stdint.i"

%feature("autodoc", "3");

%pointer_functions(float, floatp);

%include "sx1276.hpp"
%{
    #include "sx1276.hpp"
%}

