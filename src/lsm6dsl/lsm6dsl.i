#ifdef SWIGPYTHON
%module (package="upm") lsm6dsl
#endif

%import "interfaces/new_interfaces.i"

%include "../common_top.i"

/* BEGIN Java syntax  ------------------------------------------------------- */
#ifdef SWIGJAVA
%typemap(javaimports) SWIGTYPE %{
import upm_new_interfaces.*;

import java.util.AbstractList;
import java.lang.Float;
%}
%ignore getAccelerometer(float *, float *, float *);
%ignore getGyroscope(float *, float *, float *);

%ignore installISR(LSM6DSL_INTERRUPT_PINS_T , int ,  mraa::Edge ,  void *, void *);

%define INTERRUPT LSM6DSL_INTERRUPT_PINS_T
%enddef

JAVA_ADD_INSTALLISR_INTERRUPT(upm::LSM6DSL)
JAVA_JNI_LOADLIBRARY(javaupm_lsm6dsl)
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
%include "std_vector.i"
%template(floatVector) std::vector<float>;
%{
#include "lsm6dsl_defs.h"
#include "lsm6dsl.hpp"
%}
%include "lsm6dsl_defs.h"
%include "lsm6dsl.hpp"
/* END Common SWIG syntax */
