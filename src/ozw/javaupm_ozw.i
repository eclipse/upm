%module javaupm_ozw
%include "../upm.i"
%include "cpointer.i"
%include "typemaps.i"
%include "arrays_java.i";
%include "../java_buffer.i"

%{
    #include "ozw.h"
%}

%include "ozw.h"

%pragma(java) jniclasscode=%{
    static {
        try {
            System.loadLibrary("javaupm_ozw");
        } catch (UnsatisfiedLinkError e) {
            System.err.println("Native code library failed to load. \n" + e);
            System.exit(1);
        }
    }
%}
