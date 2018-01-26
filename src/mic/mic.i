%include "../common_top.i"

/* BEGIN Java syntax  ------------------------------------------------------- */
#ifdef SWIGJAVA
%include "arrays_java.i"

%typemap(jni) (uint16_t *buffer, int len) "jshortArray";
%typemap(jtype) (uint16_t *buffer, int len) "short[]";
%typemap(jstype) (uint16_t *buffer, int len) "short[]";

%typemap(javain) (uint16_t *buffer, int len) "$javainput";

%typemap(in) (uint16_t *buffer, int len) {
        $1 = (uint16_t *) JCALL2(GetShortArrayElements, jenv, $input, NULL);
        $2 = JCALL1(GetArrayLength, jenv, $input);
}

%typemap(freearg) (uint16_t *buffer, int len) {
        JCALL3(ReleaseShortArrayElements, jenv, $input, (jshort *)$1, 0);
}

%typemap(jni) (int numberOfSamples, uint16_t *buffer) "jshortArray";
%typemap(jtype) (int numberOfSamples, uint16_t *buffer) "short[]";
%typemap(jstype) (int numberOfSamples, uint16_t *buffer) "short[]";

%typemap(javain) (int numberOfSamples, uint16_t *buffer) "$javainput";

%typemap(in) (int numberOfSamples, uint16_t *buffer) {
        $2 = (uint16_t *) JCALL2(GetShortArrayElements, jenv, $input, NULL);
        $1 = JCALL1(GetArrayLength, jenv, $input);
}

%typemap(freearg) (int numberOfSamples, uint16_t *buffer) {
        JCALL3(ReleaseShortArrayElements, jenv, $input, (jshort *)$2, 0);
}

JAVA_JNI_LOADLIBRARY(javaupm_mic)
#endif
/* END Java syntax */

/* BEGIN Javascript syntax  ------------------------------------------------- */
#ifdef SWIGJAVASCRIPT
%include "../carrays_uint16_t.i"
#endif
/* END Javascript syntax */

/* BEGIN Python syntax  ----------------------------------------------------- */
#ifdef SWIGPYTHON
%include "../carrays_uint16_t.i"
#endif
/* END Python syntax */

/* BEGIN Common SWIG syntax ------------------------------------------------- */
%{
#include "mic.hpp"
%}
%include "mic.hpp"
/* END Common SWIG syntax */
