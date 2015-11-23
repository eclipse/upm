%module(directors="1") javaupm_hcsr04
%include "../upm.i"

%feature("director") IsrCallback;

%ignore generic_callback_isr;
%include "../IsrCallback.h"

%{
    #include "hcsr04.h"
%}

%include "hcsr04.h"

%pragma(java) jniclasscode=%{
    static {
        try {
            System.loadLibrary("javaupm_hcsr04");
        } catch (UnsatisfiedLinkError e) {
            System.err.println("Native code library failed to load. \n" + e);
            System.exit(1);
        }
    }
%}