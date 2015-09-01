%module javaupm_mic
%include "../upm.i"
%include "stdint.i"
%include "arrays_java.i"

%{
    #include "mic.h"
%}

%typemap(jni) (uint16_t *buffer, unsigned int len) "jshortArray";
%typemap(jtype) (uint16_t *buffer, unsigned int len) "short[]";
%typemap(jstype) (uint16_t *buffer, unsigned int len) "short[]";

%typemap(javain) (uint16_t *buffer, unsigned int len) "$javainput";

%typemap(in) (uint16_t *buffer, unsigned int len) {
        $1 = (uint16_t *) JCALL2(GetShortArrayElements, jenv, $input, NULL);
        $2 = JCALL1(GetArrayLength, jenv, $input);
}

%typemap(freearg) (uint16_t *buffer, unsigned int len) {
        JCALL3(ReleaseShortArrayElements, jenv, $input, (jshort *)$1, 0);
}

%typemap(jni) (unsigned int numberOfSamples, uint16_t *buffer) "jshortArray";
%typemap(jtype) (unsigned int numberOfSamples, uint16_t *buffer) "short[]";
%typemap(jstype) (unsigned int numberOfSamples, uint16_t *buffer) "short[]";

%typemap(javain) (unsigned int numberOfSamples, uint16_t *buffer) "$javainput";

%typemap(in) (unsigned int numberOfSamples, uint16_t *buffer) {
        $2 = (uint16_t *) JCALL2(GetShortArrayElements, jenv, $input, NULL);
        $1 = JCALL1(GetArrayLength, jenv, $input);
}

%typemap(freearg) (unsigned int numberOfSamples, uint16_t *buffer) {
        JCALL3(ReleaseShortArrayElements, jenv, $input, (jshort *)$2, 0);
}

%include "mic.h"
