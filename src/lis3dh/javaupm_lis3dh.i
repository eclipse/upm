%module javaupm_lis3dh
%include "../upm.i"
%include "typemaps.i"
%include "../upm_vectortypes.i"

%ignore getAccelerometer(float *, float *, float *);

%{
    #include "lis3dh.hpp"
    #include "lis3dh_defs.h"
%}
%include "lis3dh_defs.h"
%include "lis3dh.hpp"


%ignore installISR(LIS3DH_INTERRUPT_PINS_T , int , mraa::Edge , void *, void* );

%define INTERRUPT LIS3DH_INTERRUPT_PINS_T
%enddef
JAVA_ADD_INSTALLISR_INTERRUPT(upm::LIS3DH)

JAVA_JNI_LOADLIBRARY(javaupm_lis3dh)
