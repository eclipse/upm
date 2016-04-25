%module javaupm_joystick12
%include "../upm.i"

%{
    #include "joystick12.hpp"
%}

%include "joystick12.hpp"

%pragma(java) jniclasscode=%{
    static {
        try {
            System.loadLibrary("javaupm_joystick12");
        } catch (UnsatisfiedLinkError e) {
            System.err.println("Native code library failed to load. \n" + e);
            System.exit(1);
        }
    }
%}