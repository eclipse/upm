%module javaupm_lcm1602
%include "../upm.i"
%include "stdint.i"
%include "typemaps.i"
%include "arrays_java.i";

%apply signed char[] {uint8_t []};
%ignore BasicFont;

%typemap(jni) (uint8_t *data, int bytes) "jbyteArray";
%typemap(jtype) (uint8_t *data, int bytes) "byte[]";
%typemap(jstype) (uint8_t *data, int bytes) "byte[]";

%typemap(javain) (uint8_t *data, int bytes) "$javainput";

%typemap(in) (uint8_t *data, int bytes) {
        $1 = (uint8_t *) JCALL2(GetByteArrayElements, jenv, $input, NULL);
        $2 = JCALL1(GetArrayLength, jenv, $input);
}

%typemap(freearg) (uint8_t *data, int bytes) {
        JCALL3(ReleaseByteArrayElements, jenv, $input, (jbyte *)$1, 0);
}

%include "lcm1602.h"
%include "lcm1602.hpp"

%{
    #include "lcm1602.hpp"
%}

%pragma(java) jniclasscode=%{
    static {
        try {
            System.loadLibrary("javaupm_lcm1602");
        } catch (UnsatisfiedLinkError e) {
            System.err.println("Native code library failed to load. \n" + e);
            System.exit(1);
        }
    }
%}
