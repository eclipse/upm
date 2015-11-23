%module javaupm_grovecircularled
%include "../upm.i"
%include "arrays_java.i";

%{
    #include "grovecircularled.h"
%}

%include "grovecircularled.h"

%pragma(java) jniclasscode=%{
    static {
        try {
            System.loadLibrary("javaupm_grovecircularled");
        } catch (UnsatisfiedLinkError e) {
            System.err.println("Native code library failed to load. \n" + e);
            System.exit(1);
        }
    }
%}