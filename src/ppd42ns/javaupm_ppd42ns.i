%module javaupm_ppd42ns
%include "../upm.i"

%{
    #include "ppd42ns.hpp"
%}

%include "ppd42ns.hpp"

%pragma(java) jniclasscode=%{
    static {
        try {
            System.loadLibrary("javaupm_ppd42ns");
        } catch (UnsatisfiedLinkError e) {
            System.err.println("Native code library failed to load. \n" + e);
            System.exit(1);
        }
    }
%}