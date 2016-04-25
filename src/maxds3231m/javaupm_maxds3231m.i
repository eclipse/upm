%module javaupm_maxds3231m
%include "../upm.i"

%{
    #include "maxds3231m.hpp"
%}

%include "maxds3231m.hpp"

%pragma(java) jniclasscode=%{
    static {
        try {
            System.loadLibrary("javaupm_maxds3231m");
        } catch (UnsatisfiedLinkError e) {
            System.err.println("Native code library failed to load. \n" + e);
            System.exit(1);
        }
    }
%}