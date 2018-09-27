#ifdef SWIGPYTHON
%module (package="upm") lis2ds12
#endif

%import "interfaces/interfaces.i"

%include "../common_top.i"

/* BEGIN Java syntax  ------------------------------------------------------- */
#ifdef SWIGJAVA
%ignore getAccelerometer(float *, float *, float *);
#%ignore installISR(LIS2DS12_INTERRUPT_PINS_T , int ,  mraa::Edge, void *, void *);

%typemap(javaimports) SWIGTYPE %{
import upm_interfaces.*;

import java.util.AbstractList;
import java.lang.Float;
%}

%define INTERRUPT LIS2DS12_INTERRUPT_PINS_T
%enddef
JAVA_ADD_INSTALLISR_INTERRUPT(upm::LIS2DS12)

JAVA_JNI_LOADLIBRARY(javaupm_lis2ds12)
#endif
/* END Java syntax */

/* BEGIN Javascript syntax  ------------------------------------------------- */
#ifdef SWIGJAVASCRIPT
%include "../upm_vectortypes.i"
#endif
/* END Javascript syntax */

/* BEGIN Python syntax  ----------------------------------------------------- */
#ifdef SWIGPYTHON
%include "../upm_vectortypes.i"
#endif
/* END Python syntax */

/* BEGIN Common SWIG syntax ------------------------------------------------- */
%{
#include "lis2ds12.hpp"
%}
%include "lis2ds12_defs.h"
%include "lis2ds12.hpp"
/* END Common SWIG syntax */
