%include "../common_top.i"

/* BEGIN Java syntax  ------------------------------------------------------- */
#ifdef SWIGJAVA
%typemap(jni) int16_t* "jshortArray"
%typemap(jstype) int16_t* "short[]"
%typemap(jtype) int16_t* "short[]"

%typemap(javaout) int16_t* {
    return $jnicall;
}

%typemap(out) int16_t *getAccel {
    $result = JCALL1(NewShortArray, jenv, 3);
    JCALL4(SetShortArrayRegion, jenv, $result, 0, 3, (jshort*)$1);
}

%typemap(out) int16_t *getGyro {
    $result = JCALL1(NewShortArray, jenv, 3);
    JCALL4(SetShortArrayRegion, jenv, $result, 0, 3, (jshort*)$1);
}

%typemap(out) int16_t *getMotion {
    $result = JCALL1(NewShortArray, jenv, 6);
    JCALL4(SetShortArrayRegion, jenv, $result, 0, 6, (jshort*)$1);
}

JAVA_JNI_LOADLIBRARY(javaupm_curieimu)
#endif
/* END Java syntax */

/* BEGIN Javascript syntax  ------------------------------------------------- */
#ifdef SWIGJAVASCRIPT
%include "../carrays_int16_t.i"

// Adding this typemap because SWIG is converting int16 into a short by default
// This forces SWIG to convert it correctly
%typemap(out) int16_t* {
        jsresult = SWIG_NewPointerObj(SWIG_as_voidptr(result), SWIGTYPE_p_int16Array, 0 |  0 );
}
#endif
/* END Javascript syntax */

/* BEGIN Python syntax  ----------------------------------------------------- */
#ifdef SWIGPYTHON
%include "../carrays_int16_t.i"
%typemap(out) int16_t* {
  $result = SWIG_NewPointerObj(SWIG_as_voidptr(result), SWIGTYPE_p_int16Array, 0 |  0 );
}
#endif
/* END Python syntax */

/* BEGIN Common SWIG syntax ------------------------------------------------- */
%{
#include "curieimu.hpp"
%}
%include "curieimu.hpp"
/* END Common SWIG syntax */
