%module javaupm_button
%include "../upm.i"

%apply int {mraa::Edge}

%{
    #include "button.hpp"
%}

%include "button.hpp"

%pragma(java) jniclasscode=%{
    static {
        try {
            System.loadLibrary("javaupm_button");
        } catch (UnsatisfiedLinkError e) {
            System.err.println("Native code library failed to load. \n" + e);
            System.exit(1);
        }
    }
%}
