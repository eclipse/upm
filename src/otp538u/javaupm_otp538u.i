%module javaupm_otp538u
%include "../upm.i"

%{
    #include "otp538u.hpp"
%}

%include "otp538u.hpp"

%pragma(java) jniclasscode=%{
    static {
        try {
            System.loadLibrary("javaupm_otp538u");
        } catch (UnsatisfiedLinkError e) {
            System.err.println("Native code library failed to load. \n" + e);
            System.exit(1);
        }
    }
%}