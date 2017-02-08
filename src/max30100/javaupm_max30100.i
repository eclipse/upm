#ifdef ANDROID
    %module javaupm_max30100
#else
    %module(directors="1", threads="1") javaupm_max30100
#endif

%include "../upm.i"

%{
    #include "max30100.hpp"
%}

#ifndef ANDROID
    %feature("director") upm::Callback;
#endif

%include "max30100_regs.h"
%include "max30100.hpp"

%pragma(java) jniclasscode=%{
    static {
        try {
            System.loadLibrary("javaupm_max30100");
        } catch (UnsatisfiedLinkError e) {
            System.err.println("Native code library failed to load. \n" + e);
            System.exit(1);
        }
    }
%}
