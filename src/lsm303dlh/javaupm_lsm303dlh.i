%module javaupm_lsm303dlh
%include "../upm.i"

%{
    #include "lsm303dlh.hpp"
%}

%typemap(jni) int16_t* "jshortArray"
%typemap(jstype) int16_t* "short[]"
%typemap(jtype) int16_t* "short[]"

%typemap(javaout) int16_t* {
    return $jnicall;
}

%typemap(out) int16_t *getRawAccelData {
    $result = JCALL1(NewShortArray, jenv, 3);
    JCALL4(SetShortArrayRegion, jenv, $result, 0, 3, (jshort*)$1);
}

%include "lsm303dlh.hpp"

JAVA_JNI_LOADLIBRARY(javaupm_lsm303dlh)
