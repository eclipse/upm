%module javaupm_t6713
%include "../upm.i"
%include "arrays_java.i";
%include "../java_buffer.i"
%include "cpointer.i"
%include "typemaps.i"

%{
    #include "t6713.h"
%}

%include "t6713.h"

%pragma(java) jniclasscode=%{
    static {
        try {
            System.loadLibrary("javaupm_t6713");
        } catch (UnsatisfiedLinkError e) {
            System.err.println("Native code library failed to load. \n" + e);
            System.exit(1);
        }
    }
%}
