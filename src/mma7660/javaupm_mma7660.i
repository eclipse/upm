%module javaupm_mma7660
%include "../upm.i"
%include "cpointer.i"
%include "typemaps.i"
%include "arrays_java.i";
%include "../java_buffer.i"
%include "std_vector.i"

%template(IntVector) std::vector<int>;
%template(FloatVector) std::vector<float>;

%apply int *OUTPUT { int *x, int *y, int *z };
%apply float *OUTPUT { float *ax, float *ay, float *az  };

%{
#include "mma7660.hpp"
#include "mma7660_regs.h"
%}
%include "mma7660_regs.h"
%include "mma7660.hpp"

%ignore installISR(int , void *, void *);

JAVA_ADD_INSTALLISR_PIN(upm::MMA7660)
JAVA_JNI_LOADLIBRARY(javaupm_mma7660)
