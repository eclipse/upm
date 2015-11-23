%module javaupm_joystick12
%include "../upm.i"

%{
    #include "joystick12.h"
%}

%include "joystick12.h"

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