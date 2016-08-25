%module javaupm_hlg150h
%include "../upm.i"
%include "arrays_java.i";
%include "../java_buffer.i"
%include "cpointer.i"
%include "typemaps.i"

%{
    #include "hlg150h.hpp"
%}

%include "hlg150h.hpp"

%pragma(java) jniclasscode=%{
    static {
        try {
            System.loadLibrary("javaupm_hlg150h");
        } catch (UnsatisfiedLinkError e) {
            System.err.println("Native code library failed to load. \n" + e);
            System.exit(1);
        }
    }
%}
