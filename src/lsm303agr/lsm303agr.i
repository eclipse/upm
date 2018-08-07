#ifdef SWIGPYTHON
%module (package="upm") lsm303agr
#endif

%import "interfaces/new_interfaces.i"

%include "../common_top.i"

/* BEGIN Java syntax  ------------------------------------------------------- */
#ifdef SWIGJAVA

%ignore getMagnetometer(float *, float *, float *);
%ignore getAccelerometer(float *, float *, float *);
%ignore installISR(LSM303AGR_INTERRUPT_PINS_T , int ,  mraa::Edge , void *, void *);

%typemap(javaimports) SWIGTYPE %{
import upm_new_interfaces.*;

import java.util.AbstractList;
import java.lang.Float;
%}

%define INTERRUPT LSM303AGR_INTERRUPT_PINS_T
%enddef

JAVA_ADD_INSTALLISR_INTERRUPT(upm::LSM303AGR)
JAVA_JNI_LOADLIBRARY(javaupm_lsm303agr)
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
#include "lsm303agr_defs.h"
#include "lsm303agr.hpp"
%}
%include "lsm303agr_defs.h"
%include "lsm303agr.hpp"
/* END Common SWIG syntax */
