%module javaupm_hmc5883l
%include "../upm.i"

%{
    #include "hmc5883l.h"
%}

%typemap(jni) int16_t* "jintArray"
%typemap(jstype) int16_t* "int[]"
%typemap(jtype) int16_t* "int[]"

%typemap(javaout) int16_t* {
    return $jnicall;
}

%typemap(out) int16_t *coordinates {
    $result = JCALL1(NewIntArray, jenv, 3);
    JCALL4(SetIntArrayRegion, jenv, $result, 0, 3, (const signed int*)$1);
    //delete [] $1;
}

%include "hmc5883l.h"
