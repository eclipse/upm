%include "../common_top.i"

/* BEGIN Java syntax  ------------------------------------------------------- */
#ifdef SWIGJAVA
%include "../upm_vectortypes.i"

%ignore getAccelerometer(float *, float *, float *);
%ignore getGyroscope(float *, float *, float *);

%ignore installISR(LSM6DSL_INTERRUPT_PINS_T , int ,  mraa::Edge ,  void *, void *);

%define INTERRUPT LSM6DSL_INTERRUPT_PINS_T
%enddef

JAVA_ADD_INSTALLISR_INTERRUPT(upm::LSM6DSL)
JAVA_JNI_LOADLIBRARY(javaupm_lsm6dsl)
#endif
/* END Java syntax */

/* BEGIN Common SWIG syntax ------------------------------------------------- */
%include "../upm_vectortypes.i"
%{
#include "lsm6dsl_defs.h"
#include "lsm6dsl.hpp"
%}
%include "lsm6dsl_defs.h"
%include "lsm6dsl.hpp"
/* END Common SWIG syntax */
