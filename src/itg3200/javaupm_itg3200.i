%module javaupm_itg3200
%include "../upm.i"

%{
    #include "itg3200.h"
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


%typemap(jni) int16_t* "jintArray"
%typemap(jstype) int16_t* "int[]"
%typemap(jtype) int16_t* "int[]"

%typemap(javaout) int16_t* {
    return $jnicall;
}

%typemap(out) int16_t *getRawValues {
    $result = JCALL1(NewIntArray, jenv, 3);
    JCALL4(SetIntArrayRegion, jenv, $result, 0, 3, (const signed int*)$1);
    //delete [] $1;
}

%include "itg3200.h"
