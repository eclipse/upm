%module javaupm_hwxpxx
%include "../upm.i"
%include "typemaps.i"

%{
    #include "hwxpxx.hpp"
%}

%include "hwxpxx.hpp"

%pragma(java) jniclasscode=%{
    static {
        try {
            System.loadLibrary("javaupm_hwxpxx");
        } catch (UnsatisfiedLinkError e) {
            System.err.println("Native code library failed to load. \n" + e);
            System.exit(1);
        }
    }
%}
