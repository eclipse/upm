%module javaupm_rgbringcoder
%include "../upm.i"

%{
    #include "rgbringcoder.h"
%}

%include "rgbringcoder.h"

%pragma(java) jniclasscode=%{
    static {
        try {
            System.loadLibrary("javaupm_rgbringcoder");
        } catch (UnsatisfiedLinkError e) {
            System.err.println("Native code library failed to load. \n" + e);
            System.exit(1);
        }
    }
%}