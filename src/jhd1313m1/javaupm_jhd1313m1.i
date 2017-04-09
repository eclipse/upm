%module javaupm_jhd1313m1
%include "../upm.i"
%include "../upm_vectortypes.i"
%include "typemaps.i"

%include "jhd1313m1.hpp"
%{
    #include "jhd1313m1.hpp"
%}

%pragma(java) jniclasscode=%{
    static {
        try {
            System.loadLibrary("javaupm_jhd1313m1");
        } catch (UnsatisfiedLinkError e) {
            System.err.println("Native code library failed to load. \n" + e);
            System.exit(1);
        }
    }
%}
