#ifdef SWIGPYTHON
%module (package="upm") lis3dh
#endif

%import "interfaces/new_interfaces.i"

%include "../common_top.i"

/* BEGIN Java syntax  ------------------------------------------------------- */
#ifdef SWIGJAVA

%ignore getAccelerometer(float *, float *, float *);
%ignore installISR(LIS3DH_INTERRUPT_PINS_T , int , mraa::Edge , void *, void* );

%define INTERRUPT LIS3DH_INTERRUPT_PINS_T
%enddef

%typemap(javaimports) SWIGTYPE %{
import upm_new_interfaces.*;

import java.util.AbstractList;
import java.lang.Float;
%}

JAVA_ADD_INSTALLISR_INTERRUPT(upm::LIS3DH)
JAVA_JNI_LOADLIBRARY(javaupm_lis3dh)
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
#include "lis3dh_defs.h"
#include "lis3dh.hpp"
%}
%include "lis3dh_defs.h"
%include "lis3dh.hpp"
/* END Common SWIG syntax */
