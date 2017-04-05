%module javaupm_ms5611
%include "../upm.i"

%import "../interfaces/javaupm_iTemperatureSensor.i"
%import "../interfaces/javaupm_iPressureSensor.i"

%typemap(javaimports) SWIGTYPE %{
import upm_interfaces.*;
%}

%{
    #include "ms5611.hpp"
%}

%include "ms5611.hpp"

%pragma(java) jniclasscode=%{
    static {
        try {
            System.loadLibrary("javaupm_ms5611");
        } catch (UnsatisfiedLinkError e) {
            System.err.println("Native code library failed to load. \n" + e);
            System.exit(1);
        }
    }
%}
