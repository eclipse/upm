%module javaupm_rgbringcoder
%include "../upm.i"

%{
    #include "rgbringcoder.hpp"
%}

%include "rgbringcoder.hpp"

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