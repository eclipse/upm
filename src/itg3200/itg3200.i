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

%typemap(jni) float* "jfloatArray"
%typemap(jstype) float* "float[]"
%typemap(jtype) float* "float[]"

%typemap(javaout) float* {
    return $jnicall;
}

%typemap(out) float *getRotation {
    $result = JCALL1(NewFloatArray, jenv, 3);
    JCALL4(SetFloatArrayRegion, jenv, $result, 0, 3, $1);
}

%typemap(jni) int16_t* "jshortArray"
%typemap(jstype) int16_t* "short[]"
%typemap(jtype) int16_t* "short[]"

%typemap(javaout) int16_t* {
    return $jnicall;
}

%typemap(out) int16_t *getRawValues {
    $result = JCALL1(NewShortArray, jenv, 3);
    JCALL4(SetShortArrayRegion, jenv, $result, 0, 3, (jshort*)$1);
}

JAVA_JNI_LOADLIBRARY(javaupm_itg3200)
#endif
/* END Java syntax */

/* BEGIN Javascript syntax  ------------------------------------------------- */
#ifdef SWIGJAVASCRIPT
%include "../carrays_int16_t.i"
%include "../carrays_float.i"

%typemap(out) int16_t * {
    $result = SWIG_NewPointerObj(SWIG_as_voidptr(result), SWIGTYPE_p_int16Array, 0 | 0 );
}

%typemap(out) float * {
    $result = SWIG_NewPointerObj(SWIG_as_voidptr(result), SWIGTYPE_p_floatArray, 0 | 0 );
}

%include <carrays.i>
#endif
/* END Javascript syntax */

/* BEGIN Python syntax  ----------------------------------------------------- */
#ifdef SWIGPYTHON
%include "../carrays_int16_t.i"
%include "../carrays_float.i"

%typemap(out) int16_t * {
    $result = SWIG_NewPointerObj(SWIG_as_voidptr(result), SWIGTYPE_p_int16Array, 0 | 0 );
}

%typemap(out) float * {
    $result = SWIG_NewPointerObj(SWIG_as_voidptr(result), SWIGTYPE_p_floatArray, 0 | 0 );
}
#endif
/* END Python syntax */

/* BEGIN Common SWIG syntax ------------------------------------------------- */
%{
#include "itg3200.hpp"
%}
%include "itg3200.hpp"
/* END Common SWIG syntax */
