%module javaupm_mma7455
%include "../upm.i"
%include "typemaps.i"
%include "arrays_java.i";
%include "../java_buffer.i"
%include "std_vector.i"

%template(ShortVector) std::vector<short>;

%apply short *OUTPUT { short * ptrX, short * ptrY, short * ptrZ };

%{
#include "mma7455.hpp"
%}
%include "mma7455.hpp"

JAVA_JNI_LOADLIBRARY(javaupm_mma7455)
