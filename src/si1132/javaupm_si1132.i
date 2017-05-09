%module(directors="1") javaupm_si1132
%include "../upm.i"

%include "../interfaces/javaupm_iLight.i"

%pragma(java) jniclasscode=%{
    static {
        try {
            System.loadLibrary("javaupm_si1132");
        } catch (UnsatisfiedLinkError e) {
            System.err.println("Native code library failed to load. \n" + e);
            System.exit(1);
        }
    }
%}

%{
    #include "si1132.hpp"
%}

%include "si1132.hpp"

