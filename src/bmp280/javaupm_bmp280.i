%module javaupm_bmp280

%import "../interfaces/javaupm_iTemperatureSensor.i"
%import "../interfaces/javaupm_iHumiditySensor.i"
%import "../interfaces/javaupm_iPressureSensor.i"


%include "../upm.i"
%include "cpointer.i"
%include "typemaps.i"
%include "arrays_java.i";
%include "../java_buffer.i"

%typemap(javaimports) SWIGTYPE %{
    import upm_interfaces.*;
%}

%include "bmp280_regs.h"
%include "bmp280.hpp"
%{
    #include "bmp280.hpp"
%}

%include "bme280.hpp"
%{
    #include "bme280.hpp"
%}

%pragma(java) jniclasscode=%{
    static {
        try {
            System.loadLibrary("javaupm_bmp280");
        } catch (UnsatisfiedLinkError e) {
            System.err.println("Native code library failed to load. \n" + e);
            System.exit(1);
        }
    }
%}
