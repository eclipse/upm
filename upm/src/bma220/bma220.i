%include "../common_top.i"

/* BEGIN Java syntax  ------------------------------------------------------- */
#ifdef SWIGJAVA
%include "arrays_java.i";
%include "../java_buffer.i"

%apply int {mraa::Edge};
%apply float *INOUT { float *x, float *y, float *z };

%typemap(jni) float* "jfloatArray"
%typemap(jstype) float* "float[]"
%typemap(jtype) float* "float[]"

%typemap(javaout) float* {
    return $jnicall;
}

%typemap(out) float *getAccelerometer {
    $result = JCALL1(NewFloatArray, jenv, 3);
    JCALL4(SetFloatArrayRegion, jenv, $result, 0, 3, $1);
    delete [] $1;
}

%ignore getAccelerometer(float *, float *, float *);
%ignore installISR(int, mraa::Edge, void *, void *);

%define GETTER get_gpioIntr();
%enddef

JAVA_ADD_INSTALLISR_GPIO(upm::BMA220)
JAVA_JNI_LOADLIBRARY(javaupm_bma220)
#endif
/* END Java syntax */

/* BEGIN Javascript syntax  ------------------------------------------------- */
#ifdef SWIGJAVASCRIPT
%include "../upm_vectortypes.i"
%pointer_functions(float, floatp);
#endif
/* END Javascript syntax */

/* BEGIN Python syntax  ----------------------------------------------------- */
#ifdef SWIGPYTHON
%include "../upm_vectortypes.i"
%pointer_functions(float, floatp);
#endif
/* END Python syntax */
/* BEGIN Common SWIG syntax ------------------------------------------------- */
%{
#include "bma220.hpp"
%}
%include "bma220.hpp"
/* END Common SWIG syntax */
