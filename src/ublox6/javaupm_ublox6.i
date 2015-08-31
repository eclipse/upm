%module javaupm_ublox6
%include "../upm.i"

%{
    #include "ublox6.h"
    speed_t int_B9600 = B9600;
%}

%typemap(jni) (char *buffer, size_t len) "jbyteArray";
%typemap(jtype) (char *buffer, size_t len) "byte[]";
%typemap(jstype) (char *buffer, size_t len) "byte[]";

%typemap(javain) (char *buffer, size_t len) "$javainput";

%typemap(in) (char *buffer, size_t len) {
        $1 = (char *) JCALL2(GetByteArrayElements, jenv, $input, NULL);
        $2 = JCALL1(GetArrayLength, jenv, $input);
}

%typemap(freearg) (char *buffer, size_t len) {
        JCALL3(ReleaseByteArrayElements, jenv, $input, (jbyte *) $1, 0);
}

%include "ublox6.h"
speed_t int_B9600 = B9600;
