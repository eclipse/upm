// Include doxygen-generated documentation
%include "pyupm_doxy2swig.i"
%module pyupm_bmi160
%include "../upm.i"
%include "cpointer.i"
%include "std_string.i"

%include "stdint.i"

%feature("autodoc", "3");

%pointer_functions(float, floatp);

%include "bmi160.hpp"
%{
    #include "bmi160.hpp"
%}
