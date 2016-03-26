%module javaupm_max44009
%include "../upm.i"
%include "arrays_java.i";
%include "../java_buffer.i"
%include "cpointer.i"
%include "typemaps.i"

%{
    #include "max44009.h"
%}

%include "max44009.h"

%pragma(java) jniclasscode=%{
    static {
        try {
            System.loadLibrary("javaupm_max44009");
        } catch (UnsatisfiedLinkError e) {
            System.err.println("Native code library failed to load. \n" + e);
            System.exit(1);
        }
    }
%}
