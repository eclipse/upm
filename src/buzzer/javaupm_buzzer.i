%module javaupm_buzzer
%include "../upm.i"

%{
    #include "buzzer.h"
%}

%include "buzzer.h"

%pragma(java) jniclasscode=%{
    static {
        try {
            System.loadLibrary("javaupm_buzzer");
        } catch (UnsatisfiedLinkError e) {
            System.err.println("Native code library failed to load. \n" + e);
            System.exit(1);
        }
    }
%}