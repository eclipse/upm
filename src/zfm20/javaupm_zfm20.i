%module javaupm_zfm20
%include "../upm.i"
%include "stdint.i"
%include "typemaps.i"
%include "arrays_java.i";

%apply uint16_t *OUTPUT { uint16_t *id, uint16_t *score };

%{
    #include "zfm20.h"
    speed_t int_B57600 = B57600;
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
        JCALL3(ReleaseByteArrayElements, jenv, $input, (jbyte *)$1, 0);
}

%apply (char *buffer, size_t len) { (unsigned char *pkt, int len) };

%include "zfm20.h"
speed_t int_B57600 = B57600;
