%module javaupm_smartdrive
%include "../upm.i"

%{
    #include "smartdrive.hpp"
%}

%include "smartdrive.hpp"

%pragma(java) jniclasscode=%{
    static {
        try {
            System.loadLibrary("javaupm_smartdrive");
        } catch (UnsatisfiedLinkError e) {
            System.err.println("Native code library failed to load. \n" + e);
            System.exit(1);
        }
    }
%}
