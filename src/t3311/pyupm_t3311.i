// Include doxygen-generated documentation
%include "pyupm_doxy2swig.i"
%module pyupm_t3311
%include "../upm.i"
%include "stdint.i"
%include "cpointer.i"

%feature("autodoc", "3");

%pointer_functions(float, floatp);

%include "t3311.hpp"
%{
    #include "t3311.hpp"
%}
