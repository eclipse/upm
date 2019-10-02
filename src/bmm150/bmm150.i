#ifdef SWIGPYTHON
%module (package="upm") bmi160
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

%ignore getMagnetometer(float *, float *, float *);

%typemap(javaout) SWIGTYPE {
    return new $&javaclassname($jnicall, true);
}

%ignore installISR (BMM150_INTERRUPT_PINS_T , int ,  mraa::Edge ,  void *, void *);

%define INTERRUPT BMM150_INTERRUPT_PINS_T
%enddef
JAVA_ADD_INSTALLISR_INTERRUPT(upm::BMM150)

JAVA_JNI_LOADLIBRARY(javaupm_bmm150)
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
#include "bmm150_defs.h"
#include "bmm150.hpp"
%}
%include "bmm150_defs.h"
%include "bmm150.hpp"
/* END Common SWIG syntax */
