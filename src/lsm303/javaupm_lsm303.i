%module javaupm_lsm303
%include "../upm.i"

%{
    #include "lsm303.h"
%}

%typemap(jni) int16_t* "jintArray"
%typemap(jstype) int16_t* "int[]"
%typemap(jtype) int16_t* "int[]"

%typemap(javaout) int16_t* {
    return $jnicall;
}

%typemap(out) int16_t *getRawAccelData {
    $result = JCALL1(NewIntArray, jenv, 3);
    JCALL4(SetIntArrayRegion, jenv, $result, 0, 3, (const signed int*)$1);
    //delete [] $1;
}

%include "lsm303.h"
