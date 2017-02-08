#ifdef ANDROID
    %module javaupm_rf22
#else
    %module(directors="1") javaupm_rf22
#endif

%include "../upm.i"
%include "arrays_java.i"
%include "typemaps.i"

%apply uint8_t *INOUT { uint8_t* len };
%apply signed char[] {uint8_t*};

%{
    #include "rf22.hpp"
%}

%include "rf22.hpp"

%pragma(java) jniclasscode=%{
    static {
        try {
            System.loadLibrary("javaupm_rf22");
        } catch (UnsatisfiedLinkError e) {
            System.err.println("Native code library failed to load. \n" + e);
            System.exit(1);
        }
    }
%}
