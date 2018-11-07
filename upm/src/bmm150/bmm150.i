%include "../common_top.i"

/* BEGIN Java syntax  ------------------------------------------------------- */
#ifdef SWIGJAVA
%include "../upm_javastdvector.i"

%ignore getMagnetometer(float *, float *, float *);

%typemap(javaimports) SWIGTYPE %{
import java.util.AbstractList;
import java.lang.Float;
%}

%typemap(javaout) SWIGTYPE {
    return new $&javaclassname($jnicall, true);
}
%typemap(javaout) std::vector<float> {
    return (AbstractList<Float>)(new $&javaclassname($jnicall, true));
}
%typemap(jstype) std::vector<float> "AbstractList<Float>"

%template(floatVector) std::vector<float>;

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
