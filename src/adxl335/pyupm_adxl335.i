// Include doxygen-generated documentation
%include "pyupm_doxy2swig.i"
%module pyupm_adxl335
%include "../upm.i"
%include "cpointer.i"

%pointer_functions(int, intPointer);
%pointer_functions(float, floatPointer);

%feature("autodoc", "3");

%include "adxl335.hpp"
%{
    #include "adxl335.hpp"
%}
