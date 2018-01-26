%include "../common_top.i"

/* BEGIN Java syntax  ------------------------------------------------------- */
#ifdef SWIGJAVA
%ignore getAccelerometer(float *, float *, float *);
#%ignore installISR(LIS2DS12_INTERRUPT_PINS_T , int ,  mraa::Edge, void *, void *);

%define INTERRUPT LIS2DS12_INTERRUPT_PINS_T
%enddef
JAVA_ADD_INSTALLISR_INTERRUPT(upm::LIS2DS12)

JAVA_JNI_LOADLIBRARY(javaupm_lis2ds12)
#endif
/* END Java syntax */

/* BEGIN Common SWIG syntax ------------------------------------------------- */
%include "../upm_vectortypes.i"
%{
#include "lis2ds12.hpp"
%}
%include "lis2ds12_defs.h"
%include "lis2ds12.hpp"
/* END Common SWIG syntax */
