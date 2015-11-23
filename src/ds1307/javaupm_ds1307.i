%module javaupm_ds1307
%include "../upm.i"
%include "arrays_java.i";
%include "../java_buffer.i"

%{
    #include "ds1307.h"
%}

%include "ds1307.h"

%pragma(java) jniclasscode=%{
    static {
        try {
            System.loadLibrary("javaupm_ds1307");
        } catch (UnsatisfiedLinkError e) {
            System.err.println("Native code library failed to load. \n" + e);
            System.exit(1);
        }
    }
%}