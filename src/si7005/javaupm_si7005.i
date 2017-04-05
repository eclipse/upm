%module javaupm_si7005

%include "../upm.i"
%include "arrays_java.i";
%include "../java_buffer.i"
%include "cpointer.i"
%include "typemaps.i"

%import "../interfaces/javaupm_iTemperatureSensor.i"
%import "../interfaces/javaupm_iHumiditySensor.i"

%typemap(javaimports) SWIGTYPE %{
import upm_interfaces.*;
%}

%{
    #include "si7005.hpp"
%}

%include "si7005.hpp"

%pragma(java) jniclasscode=%{
    static {
        try {
            System.loadLibrary("javaupm_si7005");
        } catch (UnsatisfiedLinkError e) {
            System.err.println("Native code library failed to load. \n" + e);
            System.exit(1);
        }
    }
%}
