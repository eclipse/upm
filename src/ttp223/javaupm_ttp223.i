%module (docstring="TTP223 Touch Sensor") javaupm_ttp223

%include "../upm.i"

%apply int {mraa::Edge}


%{
    #include "ttp223.hpp"
%}

%include "ttp223.hpp"

%pragma(java) jniclasscode=%{
    static {
        try {
            System.loadLibrary("javaupm_ttp223");
        } catch (UnsatisfiedLinkError e) {
            System.err.println("Native code library failed to load. \n" + e);
            System.exit(1);
        }
    }
%}
