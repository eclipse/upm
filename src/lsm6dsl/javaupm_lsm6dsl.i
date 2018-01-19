%module javaupm_lsm6dsl
%include "../upm.i"
%include "typemaps.i"
%include "../upm_vectortypes.i"

%ignore getAccelerometer(float *, float *, float *);
%ignore getGyroscope(float *, float *, float *);

%{
    #include "lsm6dsl.hpp"
    #include "lsm6dsl_defs.h"
%}
%include "lsm6dsl_defs.h"
%include "lsm6dsl.hpp"


%ignore installISR(LSM6DSL_INTERRUPT_PINS_T , int ,  mraa::Edge ,  void *, void *);

%define INTERRUPT LSM6DSL_INTERRUPT_PINS_T
%enddef
JAVA_ADD_INSTALLISR_INTERRUPT(upm::LSM6DSL)
JAVA_JNI_LOADLIBRARY(javaupm_lsm6dsl)
