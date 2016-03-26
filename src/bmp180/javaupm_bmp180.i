%module javaupm_bmp180
%include "../upm.i"
%include "arrays_java.i";
%include "../java_buffer.i"
%include "cpointer.i"
%include "typemaps.i"

%{
    #include "bmp180.h"
%}

%include "bmp180.h"

%pragma(java) jniclasscode=%{
    static {
        try {
            System.loadLibrary("javaupm_bmp180");
        } catch (UnsatisfiedLinkError e) {
            System.err.println("Native code library failed to load. \n" + e);
            System.exit(1);
        }
    }
%}
