%module javaupm_mpr121
%include "../upm.i"
%include "arrays_java.i";
%include "../java_buffer.i"

%{
    #include "mpr121.hpp"
%}

%include "mpr121.hpp"

%pragma(java) jniclasscode=%{
    static {
        try {
            System.loadLibrary("javaupm_mpr121");
        } catch (UnsatisfiedLinkError e) {
            System.err.println("Native code library failed to load. \n" + e);
            System.exit(1);
        }
    }
%}