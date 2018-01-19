%module javaupm_lis2ds12
%include "../upm.i"
%include "typemaps.i"
%include "../upm_vectortypes.i"

%ignore getAccelerometer(float *, float *, float *);

%{
    #include "lis2ds12.hpp"
    #include "lis2ds12_defs.h"
%}
%include "lis2ds12_defs.h"
%include "lis2ds12.hpp"


#%ignore installISR(LIS2DS12_INTERRUPT_PINS_T , int ,  mraa::Edge, void *, void *);

%define INTERRUPT LIS2DS12_INTERRUPT_PINS_T
%enddef
JAVA_ADD_INSTALLISR_INTERRUPT(upm::LIS2DS12)

JAVA_JNI_LOADLIBRARY(javaupm_lis2ds12)
