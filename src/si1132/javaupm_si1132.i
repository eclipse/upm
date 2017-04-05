#ifdef ANDROID
    %module javaupm_si1132
#else
    %module(directors="1") javaupm_si1132
#endif

%include "../upm.i"

%import "../interfaces/javaupm_iLightSensor.i"

%typemap(javaimports) SWIGTYPE %{
import upm_interfaces.*;
%}

%pragma(java) jniclasscode=%{
    static {
        try {
            System.loadLibrary("javaupm_si1132");
        } catch (UnsatisfiedLinkError e) {
            System.err.println("Native code library failed to load. \n" + e);
            System.exit(1);
        }
    }
%}

%include "si1132.hpp"
%{
    #include "si1132.hpp"
%}


