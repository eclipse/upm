%module javaupm_grovescam
%include "../upm.i"

%{
    #include "grovescam.h"
%}

%typemap(jni) (uint8_t *buffer, size_t len) "jbyteArray";
%typemap(jtype) (uint8_t *buffer, size_t len) "byte[]";
%typemap(jstype) (uint8_t *buffer, size_t len) "byte[]";

%typemap(javain) (uint8_t *buffer, size_t len) "$javainput";

%typemap(in) (uint8_t *buffer, size_t len) {
        $1 = (uint8_t *) JCALL2(GetByteArrayElements, jenv, $input, NULL);
        $2 = JCALL1(GetArrayLength, jenv, $input);
}

%typemap(freearg) (uint8_t *buffer, size_t len) {
        JCALL3(ReleaseByteArrayElements, jenv, $input, (jbyte *)$1, 0);
}

%include "grovescam.h"
