%module (docstring="Temperature sensor") javaupm_temperature

%include "../upm.i"

%include "temperature.hpp"
%{
    #include "temperature.hpp"
%}

%pragma(java) jniclasscode=%{
    static {
        try {
            System.loadLibrary("javaupm_temperature");
        } catch (UnsatisfiedLinkError e) {
            System.err.println("Native code library failed to load. \n" + e);
            System.exit(1);
        }
    }
%}
