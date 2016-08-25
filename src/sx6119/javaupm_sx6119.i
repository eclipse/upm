%module javaupm_sx6119
%include "../upm.i"

%{
    #include "sx6119.hpp"
%}

%include "sx6119.hpp"


%pragma(java) jniclasscode=%{
    static {
        try {
            System.loadLibrary("javaupm_sx6119");
        } catch (UnsatisfiedLinkError e) {
            System.err.println("Native code library failed to load. \n" + e);
            System.exit(1);
        }
    }
%}