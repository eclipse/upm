%module javaupm_lol
%include "../upm.i"
%include "stdint.i"
%include "arrays_java.i";
%include "typemaps.i"

%typemap(jni) unsigned char* "jbyteArray";
%typemap(jtype) unsigned char* "byte[]";
%typemap(jstype) unsigned char* "byte[]";

%typemap(javaout) unsigned char* {
 return $jnicall;
}

%typemap(out) unsigned char* {
    $result = JCALL1(NewByteArray, jenv, LOL_X*LOL_Y);
    JCALL4(SetByteArrayRegion, jenv, $result, 0, LOL_X*LOL_Y-1, reinterpret_cast<jbyte*>($1));
    delete [] $1;
}

%{
    #include "lol.hpp"
%}

%include "lol.hpp"

%pragma(java) jniclasscode=%{
    static {
        try {
            System.loadLibrary("javaupm_lol");
        } catch (UnsatisfiedLinkError e) {
            System.err.println("Native code library failed to load. \n" + e);
            System.exit(1);
        }
    }
%}