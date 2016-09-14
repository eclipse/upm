%module javaupm_scam
%include "../upm.i"
%include "../java_buffer.i"

%{
    #include "scam.hpp"
%}

%include "scam.hpp"

%pragma(java) jniclasscode=%{
    static {
        try {
            System.loadLibrary("javaupm_scam");
        } catch (UnsatisfiedLinkError e) {
            System.err.println("Native code library failed to load. \n" + e);
            System.exit(1);
        }
    }
%}
