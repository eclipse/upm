%module javaupm_nunchuck
%include "../upm.i"

%{
    #include "nunchuck.h"
%}

%typemap(jni) (uint8_t *buffer, int len) "jbyteArray";
%typemap(jtype) (uint8_t *buffer, int len) "byte[]";
%typemap(jstype) (uint8_t *buffer, int len) "byte[]";

%typemap(javain) (uint8_t *buffer, int len) "$javainput";

%typemap(in) (uint8_t *buffer, int len) {
        $1 = (uint8_t *) JCALL2(GetByteArrayElements, jenv, $input, NULL);
        $2 = JCALL1(GetArrayLength, jenv, $input);
}

%typemap(freearg) (uint8_t *buffer, int len) {
        JCALL3(ReleaseByteArrayElements, jenv, $input, (jbyte *)$1, 0);
}

%include "nunchuck.h"
