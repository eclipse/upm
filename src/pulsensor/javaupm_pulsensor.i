#ifdef ANDROID
    %module javaupm_pulsensor
#else
    %module(directors="1") javaupm_pulsensor
#endif

%include "../upm.i"
%include "arrays_java.i"

#ifndef ANDROID
    %feature("director") Callback;
    SWIG_DIRECTOR_OWNED(Callback)
#endif

%ignore sample_thread;
%ignore pin_ctx;
%ignore do_sample;
%ignore callback; 

%include "Callback.hpp"
%{
    #include "pulsensor.hpp"
%}

%include "pulsensor.hpp"

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
