%module javaupm_gas
%include "../upm.i"
%include "typemaps.i"

%{
    #include "gas.hpp"
    #include "mq2.hpp"
    #include "mq3.hpp"
    #include "mq4.hpp"
    #include "mq5.hpp"
    #include "mq6.hpp"
    #include "mq7.hpp"
    #include "mq8.hpp"
    #include "mq9.hpp"
    #include "tp401.hpp"
%}

%typemap(jni) (uint16_t *buffer, int len) "jshortArray";
%typemap(jtype) (uint16_t *buffer, int len) "short[]";
%typemap(jstype) (uint16_t *buffer, int len) "short[]";

%typemap(javain) (uint16_t *buffer, int len) "$javainput";

%typemap(in) (uint16_t *buffer, int len) {
        $1 = (uint16_t *) JCALL2(GetShortArrayElements, jenv, $input, NULL);
        $2 = JCALL1(GetArrayLength, jenv, $input);
}

%typemap(freearg) (uint16_t *buffer, int len) {
        JCALL3(ReleaseShortArrayElements, jenv, $input, (jshort *)$1, 0);
}

%typemap(jni) (int numberOfSamples, uint16_t *buffer) "jshortArray";
%typemap(jtype) (int numberOfSamples, uint16_t *buffer) "short[]";
%typemap(jstype) (int numberOfSamples, uint16_t *buffer) "short[]";

%typemap(javain) (int numberOfSamples, uint16_t *buffer) "$javainput";

%typemap(in) (int numberOfSamples, uint16_t *buffer) {
        $2 = (uint16_t *) JCALL2(GetShortArrayElements, jenv, $input, NULL);
        $1 = JCALL1(GetArrayLength, jenv, $input);
}

%typemap(freearg) (int numberOfSamples, uint16_t *buffer) {
        JCALL3(ReleaseShortArrayElements, jenv, $input, (jshort *)$2, 0);
}

%include "gas.hpp"
%include "mq2.hpp"
%include "mq3.hpp"
%include "mq4.hpp"
%include "mq5.hpp"
%include "mq6.hpp"
%include "mq7.hpp"
%include "mq8.hpp"
%include "mq9.hpp"
%include "tp401.hpp"

%pragma(java) jniclasscode=%{
    static {
        try {
            System.loadLibrary("javaupm_gas");
        } catch (UnsatisfiedLinkError e) {
            System.err.println("Native code library failed to load. \n" + e);
            System.exit(1);
        }
    }
%}