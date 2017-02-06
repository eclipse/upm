%module javaupm_bmp280
%include "../upm.i"

#if SWIG_VERSION >= 0x030009
%include "../interfaces/javaupm_iTemperatureSensor.i"
%include "../interfaces/javaupm_iHumiditySensor.i"
%include "../interfaces/javaupm_iPressureSensor.i"
#else
%include "iModuleStatus.hpp"
%include "iTemperatureSensor.hpp"
%include "iPressureSensor.hpp"
%include "iHumiditySensor.hpp"
#endif

%include "cpointer.i"
%include "typemaps.i"
%include "arrays_java.i";
%include "../java_buffer.i"

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
