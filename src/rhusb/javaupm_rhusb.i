%module javaupm_rhusb
%include "../upm.i"
%include "typemaps.i"
%include "cpointer.i"
%include "arrays_java.i";
%include "../java_buffer.i"

%{
    #include "rhusb.hpp"
%}

%include "rhusb.hpp"

%pragma(java) jniclasscode=%{
    static {
        try {
            System.loadLibrary("javaupm_rhusb");
        } catch (UnsatisfiedLinkError e) {
            System.err.println("Native code library failed to load. \n" + e);
            System.exit(1);
        }
    }
%}
