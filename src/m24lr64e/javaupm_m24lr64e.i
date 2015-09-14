%module javaupm_m24lr64e
%include "../upm.i"
%include "stdint.i"
%include "arrays_java.i";
%include "../java_buffer.i"

%typemap(jni) uint8_t * "jbyteArray"
%typemap(jtype) uint8_t * "byte[]"
%typemap(jstype) uint8_t * "byte[]"

%typemap(javaout) uint8_t * {
    return $jnicall;
}

%typemap(out) uint8_t *{
    int length = upm::M24LR64E::UID_LENGTH;
    $result = JCALL1(NewByteArray, jenv, length);
    JCALL4(SetByteArrayRegion, jenv, $result, 0, length, reinterpret_cast<signed char *>($1));
    delete [] $1;
}

%{
    #include "m24lr64e.h"
%}

%include "m24lr64e.h"
