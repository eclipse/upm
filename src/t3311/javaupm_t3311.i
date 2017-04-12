%module javaupm_t3311
%include "../upm.i"
%include "typemaps.i"
%include "cpointer.i"
%include "arrays_java.i";
%include "../java_buffer.i"

%{
    #include "t3311.hpp"
%}

%include "t3311.hpp"

%pragma(java) jniclasscode=%{
    static {
        try {
            System.loadLibrary("javaupm_t3311");
        } catch (UnsatisfiedLinkError e) {
            System.err.println("Native code library failed to load. \n" + e);
            System.exit(1);
        }
    }
%}
