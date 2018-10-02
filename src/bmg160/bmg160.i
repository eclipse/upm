#ifdef SWIGPYTHON
%module (package="upm") a110x
#endif

%import "interfaces/interfaces.i"

%include "../common_top.i"

/* BEGIN Java syntax  ------------------------------------------------------- */
#ifdef SWIGJAVA
%typemap(javaimports) SWIGTYPE %{
import upm_interfaces.*;

import java.util.AbstractList;
import java.lang.Float;
%}

%ignore installISR (BMG160_INTERRUPT_PINS_T , int   mraa::Edge ,  void *, void *);
%ignore getGyroscope(float *, float *, float *);

%define INTERRUPT BMG160_INTERRUPT_PINS_T
%enddef

JAVA_ADD_INSTALLISR_INTERRUPT(upm::BMG160)
JAVA_JNI_LOADLIBRARY(javaupm_bmg160)
#endif
/* END Java syntax */

/* BEGIN Javascript syntax  ------------------------------------------------- */
#ifdef SWIGJAVASCRIPT
%include "../upm_vectortypes.i"
%pointer_functions(int, intp);
%pointer_functions(float, floatp);
#endif
/* END Javascript syntax */

/* BEGIN Python syntax  ----------------------------------------------------- */
#ifdef SWIGPYTHON
%include "../upm_vectortypes.i"
%pointer_functions(int, intp);
%pointer_functions(float, floatp);
#endif
/* END Python syntax */

/* BEGIN Common SWIG syntax ------------------------------------------------- */
%{
#include "bmg160_defs.h"
#include "bmg160.hpp"
%}
%include "bmg160_defs.h"
%include "bmg160.hpp"
/* END Common SWIG syntax */
