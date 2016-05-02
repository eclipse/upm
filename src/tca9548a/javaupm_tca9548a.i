%module javaupm_tca9548a
%include "../upm.i"

%{
    #include "tca9548a.hpp"
%}

%include "tca9548a.hpp"

%pragma(java) jniclasscode=%{
    static {
        try {
            System.loadLibrary("javaupm_tca9548a");
        } catch (UnsatisfiedLinkError e) {
            System.err.println("Native code library failed to load. \n" + e);
            System.exit(1);
        }
    }
%}
