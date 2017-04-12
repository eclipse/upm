%module javaupm_nunchuck
%include "../upm.i"
%include "../java_buffer.i"

%{
    #include "nunchuck.hpp"
%}

%include "nunchuck.hpp"

%pragma(java) jniclasscode=%{
    static {
        try {
            System.loadLibrary("javaupm_nunchuck");
        } catch (UnsatisfiedLinkError e) {
            System.err.println("Native code library failed to load. \n" + e);
            System.exit(1);
        }
    }
%}