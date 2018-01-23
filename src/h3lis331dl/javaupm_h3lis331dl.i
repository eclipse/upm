%module javaupm_h3lis331dl
%include "../upm.i"
%include "cpointer.i"
%include "typemaps.i"
%include "std_vector.i"

%template(IntVector) std::vector<int>;
%template(FloatVector) std::vector<float>;

%apply int *OUTPUT { int *x, int *y, int*z };
%apply float *OUTPUT { float *aX, float *aY, float *aZ  };

%ignore i2cContext;

%{
#include "h3lis331dl.hpp"
%}
%include "h3lis331dl.hpp"

JAVA_JNI_LOADLIBRARY(javaupm_h3lis331dl)
