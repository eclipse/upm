%module pyupm_adxl335
%include "../upm.i"
%include "cpointer.i"

%pointer_functions(int, intPointer);
%pointer_functions(float, floatPointer);

%feature("autodoc", "3");

%include "adxl335.h"
%{
    #include "adxl335.h"
%}
