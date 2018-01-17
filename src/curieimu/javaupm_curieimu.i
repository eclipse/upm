%module javaupm_curieimu
%include "../upm.i"

%{
    #include "curieimu.hpp"
%}

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

%include "curieimu.hpp"

JAVA_JNI_LOADLIBRARY(javaupm_curieimu)
