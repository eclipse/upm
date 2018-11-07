%include "../common_top.i"

/* BEGIN Java syntax  ------------------------------------------------------- */
#ifdef SWIGJAVA
%include "arrays_java.i"
%include "../java_buffer.i"

%typemap(jni) float * "jfloatArray"
%typemap(jstype) float * "float[]"
%typemap(jtype) float * "float[]"

%typemap(javaout) float * {
    return $jnicall;
}

%typemap(out) float * {
    $result = JCALL1(NewFloatArray, jenv, 3);
    JCALL4(SetFloatArrayRegion, jenv, $result, 0, 3, $1);
}

%ignore getAccelerometer(float *, float *, float *);
%ignore getGyroscope(float *, float *, float *);
%ignore getMagnetometer(float *, float *, float *);

JAVA_JNI_LOADLIBRARY(javaupm_bmi160)
#endif
/* END Java syntax */

/* BEGIN Javascript syntax  ------------------------------------------------- */
#ifdef SWIGJAVASCRIPT
%pointer_functions(float, floatp);
#endif
/* END Javascript syntax */

/* BEGIN Python syntax  ----------------------------------------------------- */
#ifdef SWIGPYTHON
%pointer_functions(float, floatp);
#endif
/* END Python syntax */

/* BEGIN Common SWIG syntax ------------------------------------------------- */
%{
#include "bmi160_defs.h"
#include "bmi160.hpp"
%}
%include "bmi160_defs.h"
%include "bmi160.hpp"
/* END Common SWIG syntax */
