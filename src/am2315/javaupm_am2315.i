%module javaupm_am2315
%include "../upm.i"

%{
    #include "am2315.hpp"
%}

%include "am2315.hpp"

%pragma(java) jniclasscode=%{
    static {
        try {
            System.loadLibrary("javaupm_am2315");
        } catch (UnsatisfiedLinkError e) {
            System.err.println("Native code library failed to load. \n" + e);
            System.exit(1);
        }
    }
%}