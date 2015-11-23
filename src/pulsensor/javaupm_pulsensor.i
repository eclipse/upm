%module(directors="1") javaupm_pulsensor
%include "../upm.i"
%include "arrays_java.i"

%feature("director") Callback;

%ignore sample_thread;
%ignore pin_ctx;
%ignore do_sample;
%ignore callback; 

%include "Callback.h"
%{
    #include "pulsensor.h"
%}

%include "pulsensor.h"

%pragma(java) jniclasscode=%{
    static {
        try {
            System.loadLibrary("javaupm_pulsensor");
        } catch (UnsatisfiedLinkError e) {
            System.err.println("Native code library failed to load. \n" + e);
            System.exit(1);
        }
    }
%}