%module javaupm_bmp280
%include "../upm.i"

%include "../interfaces/javaupm_iTemperatureSensor.i"
%include "../interfaces/javaupm_iHumiditySensor.i"
%include "../interfaces/javaupm_iPressureSensor.i"

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
