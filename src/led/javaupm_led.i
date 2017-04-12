%module javaupm_led
%include "../upm.i"

%apply int {mraa::Edge}

%include "led.hpp"
%{
    #include "led.hpp"
%}

%pragma(java) jniclasscode=%{
    static {
        try {
            System.loadLibrary("javaupm_led");
        } catch (UnsatisfiedLinkError e) {
            System.err.println("Native code library failed to load. \n" + e);
            System.exit(1);
        }
    }
%}
